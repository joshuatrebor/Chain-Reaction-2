//PROGRAM COPYRIGHT ©JoCas
#include<conio.h>
#include<stdlib.h>
#include"board mini.h"

//colors for players turnover
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
Dot dot[10][10];  //declare virtual board matrix

//initialize vboard
void initialize(){
	int X, Y;
	for(Y = 0; Y < 10; Y++){
		for(X = 0; X < 10; X++){
			dot[Y][X].x = X;
			dot[Y][X].y = Y;
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
			if((X == 0 && Y == 0) || (X == 0 && Y == 9) || (X == 9 && Y == 0) || (X == 9 && Y == 9))
				dot[Y][X].space = 1;
			else if(X == 0 || Y == 0 || X == 9 || Y == 9)
				dot[Y][X].space = 2;
			else
				dot[Y][X].space = 3;
		}
	}
}

int r = 0, b = 0, g = 0;
bool colored = true;
COORD vcoord;
int vx = 17, vy = 7;

void GameLoop(int nplayers);
void GetVcoord(colors colr);
void DotProc(int x, int y, colors colr);
void GetPhycoord(int x, int y, colors colr, int chain);
void sweep(int x, int y);
colors winscan(); 
void delay();
void control();
	
int main(){
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
		if(c == ','){
			gotoxy(29, 12);
			n = 2;
		}
		else if(c == '.'){
			gotoxy(41, 12);
			n = 3;
		}
	}
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
		 		gotoxy(48, 20);
		 		printf("Blue is Eliminated     ");
		 		getch();
		 		n++;
		 		continue;
		 	}
		 	if(turn == RED && r == 0){
		 		gotoxy(48, 20);
		 		printf("Red is Eliminated     ");
		 		getch();
		 		n++;
		 		continue;
		 	}
		 	if(turn == GREEN && g == 0){
		 		gotoxy(48, 20);
		 		printf("Green is Eliminated     ");
		 		getch();
		 		n++;
		 		continue;
		 	}
		 	if(r != 0 && g == 0 && b == 0){
		 		gotoxy(48, 20);
		 		printf("RED WINS!!          ");
		 		break;
		 	}
		 	else if(r == 0 && g != 0 && b == 0){
		 		gotoxy(48, 20);
		 		printf("GREEN WINS!!          ");
		 		break;
		 	}
		 	else if(r == 0 && g == 0 && b != 0){
		 		gotoxy(48, 20);
		 		printf("BLUE WINS!!          ");
		 		break;
		 	}
		 }
		 printcheat(vx, vy);
		 //input function
		 GetVcoord(turn);
		 n++;
	}
}
//Input and control
void GetVcoord(colors colr){
	int X, Y;
	colors turnover = EMPTY; //true is valid turn, false if invalid, repeat
	while(1){
		gotoxy(48, 20);
		printf("                          ");
		gotoxy(48, 20);
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
		gotoxy(48, 20); //else
		printf("Invalid Move!       ");
		getch();
		gotoxy(48, 20);
		printf("              ");
	}
	DotProc(X, Y, colr);
	
}

void DotProc(int X, int Y, colors colr){
	GetPhycoord(X, Y, colr, dot[Y][X].chain);
	dot[Y][X].Color = colr;	
	dot[Y][X].chain++;
	
	//eplode
	
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
	
	else if(dot[Y][X].x == 0 && dot[Y][X].y == 9){
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
	
	else if(dot[Y][X].x == 9 && dot[Y][X].y == 0){
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
	
	else if(dot[Y][X].x == 9 && dot[Y][X].y == 9){
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
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
				delay();
			DotProc(X, Y+1, colr);
		}
	}
	
	else if(dot[Y][X].x == 9){
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
				delay();
			DotProc(X, Y+1, colr);
		}
	}
	
	else if(dot[Y][X].y == 0){
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y+1, colr);
		}
	}
	
	else if(dot[Y][X].y == 9){
		if(dot[Y][X].chain == 3){
			sweep(X,Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
		}
	}
	
	else{
		if(dot[Y][X].chain == 4){
			sweep(X, Y);
			dot[Y][X].chain = 0;
			dot[Y][X].Color = EMPTY;
				delay();
			DotProc(X+1, Y, colr);
				delay();
			DotProc(X, Y+1, colr);
				delay();
			DotProc(X-1, Y, colr);
				delay();
			DotProc(X, Y-1, colr);
		}
	}
	
	
	
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
	for(a = 0; a != 10000; a++)
		for(b = 0; b != 1000; b++);
}

colors winscan(){
	int i, j;
	r = b = g = 0;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++){
			switch(dot[i][j].Color){
				case BLUE: b++; break;
				case RED: r++; break;
				case GREEN: g++; break;
			}
		}
	
	gotoxy(48, 25);
	printf("DOMINATION COUNT:");
	gotoxy(50, 26);
	printf("Blue: %d", b);
	gotoxy(50, 27);
	printf("Red: %d", r);
	gotoxy(50, 28);
	printf("Green: %d", g);
	
	if(r - (b + g) > 10){
		gotoxy(48, 30);
		printf("Red is DOMINATING!    ");
	}
	else if(b - (r + g) > 10){
		gotoxy(48, 30);
		printf("Blue is DOMINATING!   ");
	}
	else if(g - (r + b) > 10){
		gotoxy(48, 30);
		printf("Green is Dominating!  ");
	}
	if(r - (b + g) > 30){
		gotoxy(48, 30);
		printf("Red is UNSTOPPABLE!   ");
	}
	else if(b - (r + g) > 30){
		gotoxy(48, 30);
		printf("Blue is UNSTOPPABLE!  ");
	}
	else if(g - (r + b) > 30){
		gotoxy(48, 30);
		printf("Green is UNSTOPPABE!  ");
	}
	if(r - (b + g) > 60){
		gotoxy(48, 30);
		printf("Red is GODLIKE!       ");
	}
	else if(b - (r + g) > 60){
		gotoxy(48, 30);
		printf("Blue is GODLIKE!      ");
	}
	else if(g - (r + b) > 60){
		gotoxy(48, 30);
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
		if(key == 'd' && vcoord.X != 9){
			vcoord.X++;
			vx += 3;
		}
		if(key == 'w' && vcoord.Y != 0){
			vcoord.Y--;
			vy -= 3;
		}
		if(key == 's' && vcoord.Y != 9){
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
		
		if(key == 'c'){
			if(cheats == false){
				cheats = true;
				printcheat(vx, vy);
			}
			else{
				cheats = false;
				printcheat(vx, vy);
			}
		}
		if(cheats == true)
			hiddencheat(key);
		
	}
}
//PROGRAM COPYRIGHT ©JoCas
