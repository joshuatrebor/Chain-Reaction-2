#include<stdio.h>
#include<windows.h>

int VsizeXY = 3;
int speed = 1;
int vx = 17, vy = 7;

void gotoxy(int x, int y){
	COORD coord, d;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}
void drawhud();
void drawboard(){
	int hor, vert, y = 5, num;
	int pxy;
	pxy = 3 * VsizeXY;
	
	for(vert = 0; vert < pxy+1; vert++){
		gotoxy(15, y++);
		for(hor = 0; hor < pxy+1; hor++){
			if(vert % 3 == 0){
				if(vert == 0 && hor == 0)
					printf("%c", 0xC9);
				else if(vert == 0 && hor == pxy)
					printf("%c", 0xBB);
				else if(vert == pxy && hor == 0)
					printf("%c", 0xC8);
				else if(vert == pxy && hor == pxy)
					printf("%c", 0xBC);
				else if(vert == 0 && hor % 3 == 0 && hor != 0 && hor != pxy)
					printf("%c", 0xCB);
				else if(vert == pxy && hor % 3 == 0 && hor != 0 && hor != pxy)
					printf("%c", 0xCA);
				else if(hor % 3 == 0)
					printf("%c", 0xCE);
				else
					printf("%c", 0xCD);	
			}
			else{
				if(hor % 3 == 0) 
					printf("%c", 0xBA);
				else
					printf(" ");
			}
		}
		printf("\n");
	}
	y = 3;
	for(num = 1; num <= VsizeXY; num++){
		gotoxy(13, y+=3);
		printf("%2d", num);
	}
	y = 13;
	for(num = 1; num <= VsizeXY; num++){
		gotoxy(y+=3, 4);
		printf("%d", num);
	}
}

bool cheats = false;
char cheat = ' ';
bool pause = false;

void hiddencheat(int key){
	switch(key){
		case 'r': cheat = 'r'; break;
		case 'b': cheat = 'b'; break;
		case 'g': cheat = 'g'; break;
	}
	drawhud();
}

void drawhud(){
	gotoxy(VsizeXY*3+18, 6);
	printf("BLUE  -  %c", 2);
	gotoxy(VsizeXY*3+18, 7);
	printf("RED   -  %c", 3);
	gotoxy(VsizeXY*3+18, 8);
	printf("GREEN -  %c", 1);
	
	gotoxy(VsizeXY*3+18, 10);
	printf("Controls:");
	gotoxy(VsizeXY*3+18, 11);
	printf("%c - w", 30);
	gotoxy(VsizeXY*3+18, 12);
	printf("%c - s", 31);
	gotoxy(VsizeXY*3+18, 13);
	printf("%c - a", 17);
	gotoxy(VsizeXY*3+18, 14);
	printf("%c - d", 16);
	gotoxy(VsizeXY*3+18, 15);
	printf("space");
	gotoxy(VsizeXY*3+18, 17);
	printf("Toggle Color - n");
	
	gotoxy(VsizeXY*3+18, 31);
	printf("fps: %5d", speed);
	
	gotoxy(VsizeXY*3+18, 32);
	if(cheats == true)
		printf("c%c", cheat);
	else if(cheat != true)
		printf("   ");
	
	
		
	gotoxy(vx, vy);
}



