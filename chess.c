#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"		//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char bitmap[1000];
int i, j;

void moveCrds(char, short int, char, short int);

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
	 short int row;
	 short int col;
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
	short int row = space->row;
	short int col = space->col;

	int spot = 0;

	if(pawn.side == WHITE)
	{
		if(board[row-1][col].type == EMPTY)
		{
			ret[0].row = row-1;
			ret[0].col = col;
			spot+=1;
		}
		if(col > 0)
		{
			if(board[row-1][col-1].side == BLACK)		// Up 1 and to the left
			{
				ret[spot].row = row-1;
				ret[spot].col = col-1;
				spot+=1;
			}
		}
		if(col < 7)
		{
			if(board[row-1][col+1].side == BLACK)		// Up 1 and to the right
			{
				ret[spot].row = row-1;
				ret[spot].col = col+1;
				spot+=1;
			}
		}
		if( space->row == 6
			&& board[row-2][col].type == EMPTY 
			&& board[row-1][col].type == EMPTY)			//two spots above are clear
		{
				ret[spot].row = row-2;
				ret[spot].col = col;
				spot+=1;
		}
		ret[spot].row =  -1;
		ret[spot].col = -1;
	}
	else												//Pawn is black
	{
		if(board[row+1][col].type == EMPTY)
		{
			ret[0].row = row + 1;
			ret[0].col = col;
			spot+=1;
		}
		if(col > 0)
		{
			if(board[row+1][col-1].side == BLACK)		// Up 1 and to the right
			{
				ret[spot].row = row + 1;
				ret[spot].col = col - 1;
				spot+=1;
			}
		}
		if(col < 7)
		{
			if(board[row+1][col+1].side == BLACK)		// Up 1 and to the left
			{
				ret[spot].row = row - 1;
				ret[spot].col = col + 1;
				spot+=1;
			}
		}
		if( space->row == 1
			&& board[row+2][col].type == EMPTY 
			&& board[row+1][col].type == EMPTY)			//two spots above are clear
		{
				ret[spot].row = row+2;
				ret[spot].col = col;
				spot+=1;
		}
		ret[spot].row =  -1;
		ret[spot].col = -1;
	}
	return ret;
}

void printLegalMoves(crds* spot)
{
	crds* legal_moves = getLegalMoves(spot);

	i = 0;
	while(legal_moves[i].row != -1)
	{
		printf("%c%d\n", legal_moves[i].col+'a', 8-legal_moves[i].row);
		i+=1;
	}
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

crds* chessToCrds(char col, short int row)
{
	crds* ret = (crds*) malloc(sizeof(crds));
	(*ret).row = 8 - row;
	(*ret).col = col - 97;
	return ret;
}

void flush()
{
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

crds* getMove()
{
	char srccol, destcol;
	short int srcrow, destrow;

	//printf("Enter a move (ex. e2 e4):\n");
	printf("Enter a move: ");
	scanf("%c%hu %c%hu", &srccol, &srcrow, &destcol, &destrow);
	flush();

	crds* dest = chessToCrds(destcol, destrow);
	moveCrds(srccol, srcrow, destcol, destrow);
	return dest;
}

void move(short int srccol, short int srcrow, short int destcol, short int destrow)
{
	board[destrow][destcol].side = board[srcrow][srccol].side;
	board[destrow][destcol].type = board[srcrow][srccol].type;
	board[srcrow][srccol].type = NOTYPE;
	board[srcrow][srccol].side = EMPTY;
}

void moveCrds(char srccol, short int srcrow, char destcol, short int destrow)
{
	crds* srcCrds = chessToCrds(srccol, srcrow);
	crds* destCrds = chessToCrds(destcol, destrow);
	move(srcCrds->col, srcCrds->row, destCrds->col, destCrds->row);
}

int main()
{
	initNames();
	initBoard();
	printBoard();

	crds* current_spot = (crds *) malloc(sizeof(crds));

	char ans;
	printf("Would you like to enter a move (y/n)? ");
	ans = getchar();
	flush();

	while(ans != 'n')
	{
		current_spot = getMove();
		printBoard();
		printLegalMoves(current_spot);
		printf("Would you like to enter another move (y/n)? ");
		ans = getchar();
		flush();
	}
}



