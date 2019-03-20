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

typedef struct {
	 int row;
	 int col;
} crds;

typedef struct
{
	char abbrev;
	char * fullName;
} PieceName;

PieceName* names;
Piece board[8][8];

void initBoard(void)
 {

	for (i = 0; i < 8; i++)
	{
		if (i < 5) 
			board[0][i].type = i+2;
		else
			(*board + i)->type = 9 - i;
		board[0][i].side = BLACK;
		board[1][i].side = BLACK;	//Sets the value for the black pawn
		board[1][i].type = PAWN;


		if (i < 5)
			board[7][i].type = i + 2;
		else
			board[7][i].type = 9 - i;
		board[7][i].side = WHITE;
		board[6][i].side = WHITE;
		board[6][i].type = PAWN;
	}
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

crds* getLegalMovesPawn(crds*);

crds* getLegalMoves(crds* space)
{
	if(board[space->row][space->col].type == PAWN)
	{
		return getLegalMovesPawn(space);
	}
}

crds* getLegalMovesPawn(crds* space)
{
	Piece pawn = board[space->row][space->col];
	crds* ret = (crds *) malloc(sizeof(crds)*5);
	 int row = space->row;
	 int col = space->col;

	int spot = 0;

	if(pawn.side == WHITE)
	{
		if(board[row-1][col].type == EMPTY)
		{
			(*ret).row = row-1;
			(*ret).col = col;
			spot+=1;
		}
		if(space->col > 0)
		{
			if(board[row-1][col-1].side == BLACK)		// Up 1 and to the left
			{
				(*(ret+spot)).row = row-1;
				(*(ret+spot)).col = col-1;
				spot+=1;
			}
		}
		if(space->col < 7)
		{
			if(board[row-1][col+1].side == BLACK)		// Up 1 and to the right
			{
				(*(ret+spot)).row = row-1;
				(*(ret+spot)).col = col+1;
				spot+=1;
			}
		}
		if(space->row == 6 && board[row-2][col].type == EMPTY && board[row-1][col].type == EMPTY)	//two spots above are clear
		{
				(*(ret+spot)).row = row-2;
				(*(ret+spot)).col = col;
				spot+=1;
		}

		(*(ret+spot)).row = -1;
		(*(ret+spot)).col = -1;

	}
	return ret;
}

crds* getLegalMovesRook(crds* space)
{

}

crds* getLegalMovesKnight(crds* space)
{

}

crds* getLegalMovesBishop(crds* space)
{

}

crds* getLegalMovesQueen(crds* space)
{

}

crds* getLegalMovesKing(crds* space)
{

}

crds* chessToCrds(char col, char row)
{
	crds* ret = (crds*) malloc(sizeof(crds));
	(*ret).row = 8 - row;
	(*ret).col = col - 97;
	return ret;
}

void move(char srccol, char srcrow, char destcol, char destrow)
{
	board[destrow][destcol].side = board[srcrow][srccol].side;
	board[destrow][destcol].type = board[srcrow][srccol].type;
	board[srcrow][srccol].type = NOTYPE;
	board[srcrow][srccol].side = EMPTY;
}

void moveCrds(char srccol, char srcrow, char destcol, char destrow)
{
	crds* srcCrds = chessToCrds(srccol, srcrow);
	crds* destCrds = chessToCrds(destcol, destrow);

	move(srcCrds->col, srcCrds->row, destCrds->col, destCrds->row);
}

int main()
{
	initNames();

	initBoard();

	//printf("%d\n", (*(board + 6) + 7)->type);

	printBoard();

	crds* var = (crds*) malloc(sizeof(crds));
	(*var).row = 6;
	(*var).col = 4;

	crds* var2 = getLegalMoves(var);

	i = 0;
	while((*(var2+i)).row != -1)
	{
		printf("%c%d\n", (*(var2+i)).col+'a', 8-(*(var2+i)).row);
		i+=1;
	}

	moveCrds('e',7,'e',5);
	moveCrds('d',7,'d',5);
	moveCrds('f',7,'f',5);
	moveCrds('e',2,'e',4);
	printBoard();

	var = chessToCrds('e',4);
	var2 = getLegalMoves(var);

	i=0;
	//printf("%d\n",8-(*(var2+i)).row);
	while((*(var2+i)).row != -1)
	{
		printf("%c%d\n", (*(var2+i)).col+'a', 8-(*(var2+i)).row);
		i+=1;
	}
}



