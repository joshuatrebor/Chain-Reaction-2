//PROGRAM COPYRIGHT ©JoCas
#include<conio.h>
#include<stdlib.h>
#include"board.h"

//colors for players turnover
int k;
enum colors{
	BLUE,
	RED,
	GREEN,
	EMPTY
};
//Dot properties structure
struct Dot{
	int x;
	int y;
	int space;
	int chain;
	colors Color;
};  
Dot dot[15][15];  //declare virtual board matrix <- V

int r = 0, b = 0, g = 0;
bool colored = true;
bool stop = false;
COORD vcoord;

//initialize vboard
void initialize(){
	int X, Y;
	for(Y = 0; Y < VsizeXY; Y++){ // <- V
		for(X = 0; X < VsizeXY; X++){ // <- V
			dot[Y][X].x = X;
			dot[Y][X].y = Y;
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
			if((X == 0 && Y == 0) || (X == 0 && Y == VsizeXY-1) || (X == VsizeXY-1 && Y == 0) || (X == VsizeXY-1 && Y == VsizeXY-1)) // <- V-1
				dot[Y][X].space = 1;
			else if(X == 0 || Y == 0 || X == VsizeXY-1 || Y == VsizeXY-1) // <- V-1
				dot[Y][X].space = 2;
			else
				dot[Y][X].space = 3;
		}
	}
	
	colored = true;
	stop = false;
	r = 0; b = 0; g = 0;
	cheat = false;
	speed = 1;
	cheat = ' ';
	pause = false;
}

void GameLoop(int nplayers);
void GetVcoord(colors colr);
void DotProc(int x, int y, colors colr);
void GetPhycoord(int x, int y, colors colr, int chain);
void sweep(int x, int y);
colors winscan(); 
void delay();
void control();
void picksize();

HANDLE hwnd;
int main(){
	SMALL_RECT rect = {0, 0, 77, 23};
	hwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(hwnd, 1, &rect);
	//virtual coordinates initialization
	vcoord.X = 0;
	vcoord.Y = 0;
	//Menu Layout
	system("color F");
	gotoxy(32,10);
	printf("CHAIN REACTION");
	gotoxy(29, 12);
	char c;
	int n = 2;
	printf("2 Players / 3 Players");
	gotoxy(30, 13);
	printf("        < >");
	gotoxy(30, 14);
	printf("       space");
	gotoxy(29, 12);
	//2 player / 3 player picks
	while(c != 32){
		c = getch();
		if(c == 75){
			gotoxy(29, 12);
			n = 2;
		}
		else if(c == 77){
			gotoxy(41, 12);
			n = 3;
		}
	}
	picksize();
	//Entering game
	system("cls");
	drawboard();
	drawhud();
	initialize();
	GameLoop(n);
	//End Game
	getch();
}
//Player turnovers loop
void GameLoop(int nplayers){
	int n = 0; //nplayers is no. of players
	colors WinnerColor = EMPTY, turn;
	while(1){
		 switch(n % nplayers){ //Players Turnover using modulo = n to nplayers;
		 	case 0: turn = BLUE; break;
		 	case 1: turn = RED; break;
		 	case 2: turn = GREEN; break;
		 }
		 
		 //cheat
		 if(cheats == true){
		 	switch(cheat){
		 		case 'b': turn = BLUE; break;
		 		case 'r': turn = RED; break;
		 		case 'g': turn = GREEN; break;
		 	}
		 }
		 
		 //foreground color
		 if(colored == true){
		 	if(turn == BLUE)
		 		system("color 9");
		 	else if(turn == RED)
		 		system("color C");
		 	else if(turn == GREEN)
		 		system("color A");
		 }
		 //Check for elimination and winner
		 if(n >= nplayers){
		 	winscan();
		 	if(turn == BLUE && b == 0){
		 		gotoxy(VsizeXY*3+18, 20);
		 		printf("Blue is Eliminated     ");
		 		getch();
		 		n++;
		 		continue;
		 	}
		 	if(turn == RED && r == 0){
		 		gotoxy(VsizeXY*3+18, 20);
		 		printf("Red is Eliminated     ");
		 		getch();
		 		n++;
		 		continue;
		 	}
		 	if(turn == GREEN && g == 0){
		 		gotoxy(VsizeXY*3+18, 20);
		 		printf("Green is Eliminated     ");
		 		getch();
		 		n++;
		 		continue;
		 	}
		 	
		 	if(r != 0 && g == 0 && b == 0){
		 		gotoxy(VsizeXY*3+18, 20);
		 		printf("RED WINS!!          ");
		 		break;
		 	}
		 	else if(r == 0 && g != 0 && b == 0){
		 		gotoxy(VsizeXY*3+18, 20);
		 		printf("GREEN WINS!!          ");
		 		break;
		 	}
		 	else if(r == 0 && g == 0 && b != 0){
		 		gotoxy(VsizeXY*3+18, 20);
		 		printf("BLUE WINS!!          ");
		 		break;
		 	}
		 }
		 drawhud();
		 //input function
		 GetVcoord(turn);
		 n++;
	}
	system("cls");
	initialize();
	main();
	return;
}
//Input and control
void GetVcoord(colors colr){
	int X, Y;
	colors turnover = EMPTY; //true is valid turn, false if invalid, repeat
	while(1){
		gotoxy(VsizeXY*3+18, 20);
		printf("                          ");
		gotoxy(VsizeXY*3+18, 20);
		switch(colr){
			case BLUE: printf("BLUE'S TURN "); break;
			case RED: printf("RED'S TURN "); break;
			case GREEN: printf("GREEN'S TURN "); 
		}
		control();
		X = vcoord.X;
		Y = vcoord.Y;
		turnover = dot[Y][X].Color;
		if(turnover == colr || turnover == EMPTY)
			break; //break if valid move
		gotoxy(VsizeXY*3+18, 20); //else
		printf("Invalid Move!       ");
		getch();
		gotoxy(VsizeXY*3+18, 20);
		printf("              ");
	}
	DotProc(X, Y, colr);
	
}

void DotProc(int X, int Y, colors colr){
	GetPhycoord(X, Y, colr, dot[Y][X].chain);
	dot[Y][X].Color = colr;	
	dot[Y][X].chain++;
	
	//explode
	if(stop == true){
		return;
	}
	
	if(dot[Y][X].x == 0 && dot[Y][X].y == 0){
		if(dot[Y][X].chain == 2){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y+1, colr);
		}
	}
	
	else if(dot[Y][X].x == 0 && dot[Y][X].y == VsizeXY-1){ // <- V-1
		if(dot[Y][X].chain == 2){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
		}
	}
	
	else if(dot[Y][X].x == VsizeXY-1 && dot[Y][X].y == 0){ // <- V-1
		if(dot[Y][X].chain == 2){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y+1, colr);
		}
	}
	
	else if(dot[Y][X].x == VsizeXY-1 && dot[Y][X].y == VsizeXY-1){ // <- V-1
		if(dot[Y][X].chain == 2){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
		}
	}
	
	else if(dot[Y][X].x == 0){
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X, Y+1, colr);
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
		}
	}
	
	else if(dot[Y][X].x == VsizeXY-1){ // <- V-1
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X, Y+1, colr);
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
		}
	}
	
	else if(dot[Y][X].y == 0){
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y+1, colr);
		}
	}
	
	else if(dot[Y][X].y == VsizeXY-1){ // <- V-1
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
				delay();
			DotProc(X-1, Y, colr);
		}
	}
	
	else{
		if(dot[Y][X].chain == 4){
			sweep(X, Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X, Y-1, colr);
				delay();
			DotProc(X, Y+1, colr);
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X+1, Y, colr);
		}
	}
	
	if(kbhit()){
		k = getch();
		if(k == 27)
			stop = true;
		if(k == '+'){
			speed++;
			if(speed == 101) speed = 100;
			drawhud();
		}
		if(k == '-'){
			speed--; 
			drawhud();
		}
		if(k == 'p')
			pause = true;
	}
	return;
}
//convert virtual to physical coordinates
void GetPhycoord(int x, int y, colors colr, int chain){
	int px, py;
	char c;
	switch(colr){
		case BLUE: c = 2; break;
		case RED: c = 3; break;
		case GREEN: c = 1;
	}
	px = x * 3 + 16; //formula x
	py = y * 3 + 6; //formula y
	if(chain == 0){
		gotoxy(px, py);
		printf("%c", c);
	}
	else if(chain == 1){
		gotoxy(px, py);
		printf("%c", c);
		gotoxy(++px, py);
		printf("%c", c);
	}
	else if(chain == 2){
		gotoxy(px, py);
		printf("%c", c);
		gotoxy(++px, py);
		printf("%c", c);
		gotoxy(px, ++py);
		printf("%c", c);
	}
	
}

void sweep(int x, int y){
	int px, py;
	px = x * 3 + 16;
	py = y * 3 + 6;
	gotoxy(px, py); printf("  ");
	gotoxy(px,py+1); printf("  ");
	gotoxy(px, py); printf("  ");


}

void delay(){
	int a, b;
	for(a = 0; a != 10010-(speed*10); a++)
		for(b = 0; b != 1010-(speed*10); b++);
	if(pause == true){
		gotoxy(15, 3);
		printf("PAUSED - press any key to play");
		getch();
		gotoxy(15, 3);
		printf("                               ");
		pause = false;
	}
}

colors winscan(){
	int i, j;
	r = b = g = 0;
	for(i = 0; i < VsizeXY; i++) // <- V
		for(j = 0; j < VsizeXY; j++){ // <- V
			switch(dot[i][j].Color){
				case BLUE: b++; break;
				case RED: r++; break;
				case GREEN: g++; break;
			}
		}
	
	gotoxy(VsizeXY*3+18, 25);
	printf("DOMINATION COUNT:");
	gotoxy(VsizeXY*3+18, 26);
	printf("Blue: %d", b);
	gotoxy(VsizeXY*3+18, 27);
	printf("Red: %d", r);
	gotoxy(VsizeXY*3+18, 28);
	printf("Green: %d", g);
	
	if(r - (b + g) > 10){
		gotoxy(VsizeXY*3+18, 30);
		printf("Red is DOMINATING!    ");
	}
	else if(b - (r + g) > 10){
		gotoxy(VsizeXY*3+18, 30);
		printf("Blue is DOMINATING!   ");
	}
	else if(g - (r + b) > 10){
		gotoxy(VsizeXY*3+18, 30);
		printf("Green is Dominating!  ");
	}
	if(r - (b + g) > 30){
		gotoxy(VsizeXY*3+18, 30);
		printf("Red is UNSTOPPABLE!   ");
	}
	else if(b - (r + g) > 30){
		gotoxy(VsizeXY*3+18, 30);
		printf("Blue is UNSTOPPABLE!  ");
	}
	else if(g - (r + b) > 30){
		gotoxy(VsizeXY*3+18, 30);
		printf("Green is UNSTOPPABE!  ");
	}
	if(r - (b + g) > 60){
		gotoxy(VsizeXY*3+18, 30);
		printf("Red is GODLIKE!       ");
	}
	else if(b - (r + g) > 60){
		gotoxy(VsizeXY*3+18, 30);
		printf("Blue is GODLIKE!      ");
	}
	else if(g - (r + b) > 60){
		gotoxy(VsizeXY*3+18, 30);
		printf("Green is GODLIKE!     ");
	}		
}
//keyboard controls
void control(){
	gotoxy(vx, vy);
	int key = 0;
	while(key != 32){
		key = getch();
		if(key == 'a' && vcoord.X != 0){
			vcoord.X--;
			vx -= 3;
		}
		if(key == 'd' && vcoord.X != VsizeXY-1){ // <- V-1
			vcoord.X++;
			vx += 3;
		}
		if(key == 'w' && vcoord.Y != 0){
			vcoord.Y--;
			vy -= 3;
		}
		if(key == 's' && vcoord.Y != VsizeXY-1){ // <- V-1
			vcoord.Y++;
			vy += 3;
		}
		if(key == 'n'){
			if(colored == true){
				colored = false;
				system("color 7");
			}
			else
				colored = true;
		}
		gotoxy(vx, vy);
		
		if(key == '+'){
			speed++;
			drawhud();
		}
		
		if(key == '-'){
			speed--;
			drawhud();
		}
		
		if(key == 'c'){
			if(cheats == false)
				cheats = true;
			else
				cheats = false;
			drawhud();
		}
		
		if(key == 49){
			system("cls");
			initialize();
			main();
			return;
		}
		
		if(cheats == true)
			hiddencheat(key);
		
	}
}

void picksize(){
	system("cls");
	gotoxy(28, 12);
	printf("TINY  5x5  7x7  10x10  HD");
	char c = 0;
	int x = 28;
	while(c != 32){
		gotoxy(x, 12);
		c = getch();
		if(c == 77){
			if(x == 28) x = 34;
			else if(x == 34) x = 39;
			else if(x == 39) x = 44;
			else if(x == 44) x = 51;
		}
		if(c == 75){
			if(x == 51) x = 44;
			else if(x == 44) x = 39;
			else if(x == 39) x = 34;
			else if(x == 34) x = 28;
		}
	}
	SMALL_RECT rect;
	switch(x){
		case 28: VsizeXY = 3; rect = {0,0,51,35}; SetConsoleWindowInfo(hwnd, 1, &rect); break;
		case 34: VsizeXY = 5; rect = {0,0,68,35}; SetConsoleWindowInfo(hwnd, 1, &rect); break;
		case 39: VsizeXY = 7; rect = {0,0,70,35}; SetConsoleWindowInfo(hwnd, 1, &rect); break;
		case 44: VsizeXY = 10; rect = {0,0,75,39}; SetConsoleWindowInfo(hwnd, 1, &rect); break;
		case 51: VsizeXY = 15; rect = {0,0,96,53}; SetConsoleWindowInfo(hwnd, 1, &rect); break;
	}
}
//PROGRAM COPYRIGHT ©JoCas
