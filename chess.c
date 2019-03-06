#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"		//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char bitmap[1000];
int i, j;

void BMPmake()
{
    // -- FILE HEADER -- //

    // bitmap signature
    bitmap[0] = 'B';
    bitmap[1] = 'M';

    // file size
    bitmap[2] = 66; // 40 + 14 + 12
    bitmap[3] = 0;
    bitmap[4] = 0;
    bitmap[5] = 0;

    // reserved field (in hex. 00 00 00 00)
    for(int i = 6; i < 10; i++) bitmap[i] = 0;

    // offset of pixel data inside the image
	bitmap[10]=54;
	//bitmap[11]=6;
    for(int i = 11; i < 14; i++) bitmap[i] = 0;

    // -- BITMAP HEADER -- //

    // header size
    bitmap[14] = 40;
    for(int i = 15; i < 18; i++) bitmap[i] = 0;

    // width of the image
    bitmap[18] = 4;
    for(int i = 19; i < 22; i++) bitmap[i] = 0;

    // height of the image
    bitmap[22] = 1;
    for(int i = 23; i < 26; i++) bitmap[i] = 0;

    // reserved field
    bitmap[26] = 1;
    bitmap[27] = 0;

    // number of bits per pixel
    bitmap[28] = 24; // 3 byte
    bitmap[29] = 0;

    // compression method (no compression here)
    for(int i = 30; i < 34; i++) bitmap[i] = 0;

    // size of pixel data
    bitmap[34] = 21; // 12 bits => 4 pixels
    bitmap[35] = 0;
    bitmap[36] = 0;
    bitmap[37] = 0;

    // horizontal resolution of the image - pixels per meter (2835)
    bitmap[39] = 0;
    bitmap[38] = 0;
    bitmap[40] = 0x30;//0b00110000;
    bitmap[41] = 0xB1;//0b10110001;

    // vertical resolution of the image - pixels per meter (2835)
    bitmap[42] = 0;
    bitmap[43] = 0;
    bitmap[44] = 0x30; //0b00110000;
    bitmap[45] = 0xB1; //0b10110001;

    // color pallette information
    for(int i = 46; i < 50; i++) bitmap[i] = 0;

    // number of important colors
    for(int i = 50; i < 54; i++) bitmap[i] = 0;

    // -- PIXEL DATA -- //
    for(int i = 54; i < 69; i++) bitmap[i] = 255;
}

void BMPwrite()
{
    FILE *file;
    file = fopen("bitmap.bmp", "w+");
    for(int i = 0; i < 66; i++)
    {
        fputc(bitmap[i], file);
    }
    fclose(file);
}

enum PieceNum{
	EMPTY,
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

enum Side {
	NOTYPE,
	WHITE,
	BLACK
};

typedef struct
{
	char side;
	char type;
} Piece;

typedef struct
{
	char abbrev;
	char * fullName;
} PieceName;

PieceName* names;
Piece** board;

Piece** initBoard(void)
 {
	Piece **board = (Piece **)malloc(8 * sizeof(Piece *)); 
	for(i=0; i<8; i++)
	{
		*(board+i) = (Piece *)malloc(8 * sizeof(Piece));
	}
	for (i = 0; i < 8; i++)
	{
		if (i < 5) 
			(*board + i)->type = i+2;
		else
			(*board + i)->type = 9 - i;
		(*board + i)->side = BLACK;
		(*(board + 1) + i)->side = BLACK;	//Sets the value for the black pawn
		(*(board + 1) + i)->type = PAWN;


		if (i < 5)
			(*(board + 7) + i)->type = i + 2;
		else
			(*(board + 7) + i)->type = 9 - i;
		(*(board + 7) + i)->side = WHITE;
		(*(board + 6) + i)->side = WHITE;
		(*(board + 6) + i)->type = PAWN;
	}
	return board;
}

void initNames(void)
{
	names = (PieceName *)malloc(7 * sizeof(PieceName));
	for (i = 0; i < 7; i++)
	{
		(names + i)->fullName = (char *)malloc(6 * sizeof(char));
	}
	strcpy(names->fullName, "Empty");
	strcpy((names+1)->fullName, "Pawn");
	strcpy((names + 2)->fullName, "Rook");
	strcpy((names + 3)->fullName, "Knight");
	strcpy((names + 4)->fullName, "Bishop");
	strcpy((names + 5)->fullName, "Queen");
	strcpy((names + 6)->fullName, "King");

	(names)	   ->abbrev = '.';
	(names + 1)->abbrev = 'P';
	(names + 2)->abbrev = 'R';
	(names + 3)->abbrev = 'N';
	(names + 4)->abbrev = 'B';
	(names + 5)->abbrev = 'Q';
	(names + 6)->abbrev = 'K';
}

void printBoard(void)
{
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			char side = (*(board + i) + j)->side;
			char type = (*(board + i) + j)->type;
			if (side == BLACK)
			{
				printf(ANSI_COLOR_RED "%c "ANSI_COLOR_RESET, (names + type)->abbrev);
			}
			else if (side == WHITE)
			{
				printf(ANSI_COLOR_GREEN "%c "ANSI_COLOR_RESET, (names + type)->abbrev);
			}
			else printf(". ");

		}
		printf("\n");
	}
}

int main()
{
	initNames();

	board = initBoard();

	//printf("%d\n", (*(board + 6) + 7)->type);

	printBoard();

	
	FILE *f = fopen("board.bmp", "w");
	BMPmake();
	BMPwrite();
}



