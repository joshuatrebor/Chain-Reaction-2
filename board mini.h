#include<stdio.h>
#include<windows.h>

void gotoxy(int x, int y){
	COORD coord, d;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void drawboard(){
	int hor, vert, y = 5, num;
	for(vert = 0; vert < 16; vert++){
		gotoxy(15, y++);
		for(hor = 0; hor < 16; hor++){
			if(vert % 3 == 0){
				if(vert == 0 && hor == 0)
					printf("%c", 0xC9);
				else if(vert == 0 && hor == 15)
					printf("%c", 0xBB);
				else if(vert == 15 && hor == 0)
					printf("%c", 0xC8);
				else if(vert == 15 && hor == 15)
					printf("%c", 0xBC);
				else if(vert == 0 && hor % 3 == 0 && hor != 0 && hor != 15)
					printf("%c", 0xCB);
				else if(vert == 15 && hor % 3 == 0 && hor != 0 && hor != 15)
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
	for(num = 0; num < 5; num++){
		gotoxy(13, y+=3);
		printf("%2d", num);
	}
	y = 13;
	for(num = 0; num < 5; num++){
		gotoxy(y+=3, 4);
		printf("%d", num);
	}
}

void drawhud(){
	gotoxy(48, 6);
	printf("BLUE  -  %c", 2);
	gotoxy(48, 7);
	printf("RED   -  %c", 3);
	gotoxy(48, 8);
	printf("GREEN -  %c", 1);
	
	gotoxy(48, 10);
	printf("Controls:");
	gotoxy(50, 11);
	printf("%c - w", 30);
	gotoxy(50, 12);
	printf("%c - s", 31);
	gotoxy(50, 13);
	printf("%c - a", 17);
	gotoxy(50, 14);
	printf("%c - d", 16);
	gotoxy(50, 15);
	printf("space");
	gotoxy(48, 17);
	printf("Toggle Color - n");
	
}

bool cheats = false;
char cheat = ' ';
void hiddencheat(int key){
	switch(key){
		case 'r': cheat = 'r'; break;
		case 'b': cheat = 'b'; break;
		case 'g': cheat = 'g'; break;
	}
}

void printcheat(int vx, int vy){
	gotoxy(48, 34);
	if(cheats == true)
		printf("c%c", cheat);
	else
		printf("  ");
	gotoxy(vx, vy);
}




