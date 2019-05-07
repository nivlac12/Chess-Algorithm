#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "chess.h"

#define ANSI_COLOR_RED     "\033[1;31m"		//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_MAGENTA "\033[1;35m"
#define ANSI_COLOR_BLUE    "\033[1;32m"
#define ANSI_COLOR_RESET   "\033[0m"

char bitmap[1000];
int i, j;
char global_side;

PieceName* names;
Piece board[8][8];
int val_hash[8][8];

void initBoard(void)
 {
 	for(i = 0; i < 8; i++)
 	{
 		for(j = 0; j < 8; j++)
 		{
 			board[i][j].type = 0;
 			board[i][j].side = 0;
 		}
 	}

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

void initHash(void)
{
	int temp[8][8] = 
	{
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 2, 2, 2, 2, 1, 0},
		{0, 1, 2, 3, 3, 2, 1, 0},
		{0, 1, 2, 3, 3, 2, 1, 0},
		{0, 1, 2, 2, 2, 2, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			val_hash[i][j] = temp[i][j];
		}
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
	strcpy((names + 1)->fullName, "Pawn");
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

	(names)->val = 0;
	(names + 1)->val = 5;
	(names + 2)->val = 25;
	(names + 3)->val = 15;
	(names + 4)->val = 15;
	(names + 5)->val = 45;
	(names + 6)->val = 300;
}

void printBoard(void)
{
	for (i = 0; i < 8; i++)
	{
		printf("%d ", 8-i);
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
				printf(ANSI_COLOR_BLUE "%c "ANSI_COLOR_RESET, (names + type)->abbrev);
			}
			else if (side > BLACK)
			{
				if(type == NOTYPE) printf(ANSI_COLOR_MAGENTA ". " ANSI_COLOR_RESET);
				else printf(ANSI_COLOR_MAGENTA "%c "ANSI_COLOR_RESET, (names + type)->abbrev);
			}
			else printf(". ");

		}
		printf("\n");
	}
	printf("  ");
	for(i=0;i<8;i++)
	{
		printf("%c ", 'a' + i);
	}
	printf("\n");
}

crds* getLegalMoves(crds* space)
{
	if(board[space->row][space->col].type == PAWN)
	{
		return getLegalMovesPawn(space);
	}
	else if(board[space->row][space->col].type == ROOK)
	{
		return getLegalMovesRook(space);
	}
	else if(board[space->row][space->col].type == BISHOP)
	{
		return getLegalMovesBishop(space);
	}
	else if(board[space->row][space->col].type == QUEEN)
	{
		return getLegalMovesQueen(space);
	}
	else if(board[space->row][space->col].type == KNIGHT)
	{
		return getLegalMovesKnight(space);
	}
	return getLegalMovesKing(space);
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
			if(board[row+1][col-1].side == WHITE)		// Up 1 and to the right
			{
				ret[spot].row = row + 1;
				ret[spot].col = col - 1;
				spot+=1;
			}
		}
		if(col < 7)
		{
			if(board[row+1][col+1].side == WHITE)		// Up 1 and to the left
			{
				ret[spot].row = row + 1;
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
	printf("Move: %c%d\n", spot->col + 'a', 8-spot->row);
	Piece* piece;
	i = 0;
	while(legal_moves[i].row != -1)
	{
		piece = &board[legal_moves[i].row][legal_moves[i].col];
		if(piece->side == WHITE)
		{
			piece->side = LEGALWHITE;
		}
		else if(piece->side == BLACK)
		{
			piece->side = LEGALBLACK;
		}
		else
		{
			piece->side = LEGALEMPTY;
		}
		printf("%c%d\n", legal_moves[i].col+'a', 8-legal_moves[i].row);
		i+=1;
	}
	printBoard();
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(board[i][j].side == LEGALWHITE)
			{
				board[i][j].side = WHITE;
			}
			if(board[i][j].side == LEGALBLACK)
			{
				board[i][j].side = BLACK;
			}
			else if (board[i][j].side == LEGALEMPTY)
			{
				board[i][j].side = NOTYPE;
			}
		}
	}
}

crds* getLegalMovesRook(crds* space)
{
	Piece pawn = board[space->row][space->col];
	crds* ret = (crds *) malloc(sizeof(crds)*15);
	short int row = space->row;
	short int col = space->col;
	short int side = board[row][col].side;
	short int other_side = (side == WHITE) ? BLACK : WHITE;
	short int spot = 0;

	for(i=row+1; i<8; ++i)	//moving down the board
	{
		if(board[i][col].type == EMPTY)
		{
			ret[spot].row = i;
			ret[spot].col = col;
			spot++;
		}
		else if(board[i][col].side == other_side)
		{
			ret[spot].row = i;
			ret[spot].col = col;
			spot++;
			break;
		}
		else if(board[i][col].side == side)
		{
			break;
		}
	}

	for(i=row-1; i>=0; --i)	//moving up the board
	{
		if(board[i][col].type == EMPTY)
		{
			ret[spot].row = i;
			ret[spot].col = col;
			spot++;
		}
		else if(board[i][col].side == other_side)
		{
			ret[spot].row = i;
			ret[spot].col = col;
			spot++;
			break;
		}
		else if(board[i][col].side == side)
		{
			break;
		}
	}

	for(i=col+1; i<8; ++i)	//moving to the right
	{
		if(board[row][i].type == EMPTY)
		{
			ret[spot].row = row;
			ret[spot].col = i;
			spot++;
		}
		else if(board[row][i].side == other_side)
		{
			ret[spot].row = row;
			ret[spot].col = i;
			spot++;
			break;
		}
		else if(board[row][i].side == side)
		{
			break;
		}
	}

	for(i=col-1; i>=0; --i)	//moving to the left
	{
		if(board[row][i].type == EMPTY)
		{
			ret[spot].row = row;
			ret[spot].col = i;
			spot++;
		}
		else if(board[row][i].side == other_side)
		{
			ret[spot].row = row;
			ret[spot].col = i;
			spot++;
			break;
		}
		else if(board[row][i].side == side)
		{
			break;
		}
	}

	ret[spot].row = -1;
	ret[spot].col = -1;

	return ret;
}

crds* getLegalMovesKnight(crds* space)
{
	Piece pawn = board[space->row][space->col];
	crds* ret = (crds *) malloc(sizeof(crds)*15);
	short int row = space->row;
	short int col = space->col;
	short int other_side = (board[row][col].side == WHITE) ? BLACK : WHITE;
	short int spot = 0;

	spotValid(row+1, col+2, other_side, ret, &spot);
	spotValid(row+1, col-2, other_side, ret, &spot);
	spotValid(row-1, col+2, other_side, ret, &spot);
	spotValid(row-1, col-2, other_side, ret, &spot);
	spotValid(row+2, col+1, other_side, ret, &spot);
	spotValid(row+2, col-1, other_side, ret, &spot);
	spotValid(row-2, col+1, other_side, ret, &spot);
	spotValid(row-2, col-1, other_side, ret, &spot);
	ret[spot].row = -1;
	ret[spot].col = -1;
	return ret;
}

// Only for Knight and King
void spotValid(short int row, short int col, short int other_side, crds* ret, short int* spot)
{
	if( row < 8 && col < 8 && row >= 0 && col >= 0
			&& (board[row][col].type == EMPTY
			|| board[row][col].side == other_side) )
	{
		ret[*spot].row = row;
		ret[*spot].col = col;
		(*spot)++;
	}
}

crds* getLegalMovesBishop(crds* space)
{
	crds* ret = (crds *) malloc(sizeof(crds)*15);
	short int row = space->row;
	short int col = space->col;
	short int side = board[row][col].side;
	short int other_side = (side == WHITE) ? BLACK : WHITE;
	short int spot = 0;

	for(i = row + 1, j = col + 1; i<8 && j<8; i++, j++)	// down and right
	{
		if(board[i][j].type == EMPTY)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
		}
		else if(board[i][j].side == other_side)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
			break;
		}
		else if(board[i][j].side == side)
		{
			break;
		}
	}

	for(i = row - 1, j = col + 1; i>=0 && j<8; i--, j++)	// up and right
	{
		if(board[i][j].type == EMPTY)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
		}
		else if(board[i][j].side == other_side)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
			break;
		}
		else if(board[i][j].side == side)
		{
			break;
		}
	}

	for(i = row + 1, j = col - 1; i<8 && j>=0; i++, j--)	// down and left
	{
		if(board[i][j].type == EMPTY)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
		}
		else if(board[i][j].side == other_side)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
			break;
		}
		else if(board[i][j].side == side)
		{
			break;
		}
	}

	for(i = row - 1, j = col - 1; i>=0 && j>=0; i--, j--)	// up and left
	{
		if(board[i][j].type == EMPTY)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
		}
		else if(board[i][j].side == other_side)
		{
			ret[spot].row = i;
			ret[spot].col = j;
			spot++;
			break;
		}
		else if(board[i][j].side == side)
		{
			break;
		}
	}

	ret[spot].row = -1;
	ret[spot].col = -1;

	return ret;
}

crds* getLegalMovesQueen(crds* space)
{
	crds* rookMoves = getLegalMovesRook(space);
	crds* bishopMoves = getLegalMovesBishop(space);
	crds* ret = (crds *) malloc(sizeof(crds)*30);

	for(i=0; rookMoves[i].row != -1; ++i)
	{
		ret[i].row = rookMoves[i].row;
		ret[i].col = rookMoves[i].col;
	}
	int hold = i;
	for(i=0; bishopMoves[i].row != -1; ++i)
	{
		ret[i+hold].row = bishopMoves[i].row;
		ret[i+hold].col = bishopMoves[i].col;
	}

	ret[i+hold].row = -1;
	ret[i+hold].col = -1;

	return ret;
}

crds* getLegalMovesKing(crds* space)
{
	Piece pawn = board[space->row][space->col];
	crds* ret = (crds *) malloc(sizeof(crds)*15);
	short int row = space->row;
	short int col = space->col;
	short int other_side = (board[row][col].side == WHITE) ? BLACK : WHITE;
	short int spot = 0;

	spotValid(row+1, col+1, other_side, ret, &spot);
	spotValid(row+1, col-1, other_side, ret, &spot);
	spotValid(row-1, col+1, other_side, ret, &spot);
	spotValid(row-1, col-1, other_side, ret, &spot);
	spotValid(row  , col+1, other_side, ret, &spot);
	spotValid(row  , col-1, other_side, ret, &spot);
	spotValid(row+1, col  , other_side, ret, &spot);
	spotValid(row-1, col  , other_side, ret, &spot);
	ret[spot].row = -1;
	ret[spot].col = -1;
	return ret;

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
	crds *src, *dest, *legal_moves;
	int askAgain = 1;

	while(askAgain)
	{
		//printf("Enter a move (ex. e2 e4):\n");
		printf("Enter a move: ");
		scanf("%c%hu %c%hu", &srccol, &srcrow, &destcol, &destrow);
		flush();

		src = chessToCrds(srccol, srcrow);
		dest = chessToCrds(destcol, destrow);
		legal_moves = getLegalMoves(src);

		if(board[src->row][src->col].side == global_side)
		{
			for(i = 0; legal_moves[i].row != -1; i++)
			{
				if(legal_moves[i].row == dest->row && legal_moves[i].col == dest->col)
				{
					askAgain = 0;
				}
			}
		}
		if(askAgain)
		{
			printf("Not a legal move, please enter another move\n");
		}
	}

	moveCrds(srccol, srcrow, destcol, destrow);
	global_side = (global_side == WHITE) ? BLACK : WHITE;
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


MoveValTuple* testMoves(char depth, char side, char max, int best_or_worst_value)
{
	if(depth < 4)
	{
		/*************************************
		Part 1, search all the legal moves
		*************************************/
		crds current_position;
		//MoveTuple** legal_moves_list = (MoveTuple **) malloc(sizeof(MoveTuple *)*120);
		MoveTuple* legal_moves_list[120];
		for(i=0; i<120; i++)
		{
			legal_moves_list[i] = (MoveTuple *) malloc(sizeof(MoveTuple));
			legal_moves_list[i]->src = (crds *) malloc(sizeof(crds));
			legal_moves_list[i]->dest = (crds *) malloc(sizeof(crds));
		}

		char other_side = (side == WHITE) ? BLACK : WHITE;
		int value_of_move[120];
		crds* temp;
		int spot_in_list = 0;
		for(short int a = 0; a<8; a++)
		{
			for(short int b = 0; b<8; b++)
			{
				if(board[a][b].side == side)
				{
					current_position.row = a;
					current_position.col = b;

					temp = getLegalMoves(&current_position);
					spot_in_list = appendList(temp, legal_moves_list, spot_in_list, a, b);
				}
			}
		}
		legal_moves_list[spot_in_list]->dest->row = -1;
		value_of_move[spot_in_list] = -500;

		//printf("Done with part 1\n");
		/*************************************
		Part 2, make a temporary move
		*************************************/
		UndoMoveTuple local_move_to_undo;
		short int srccol, srcrow, destcol, destrow;
		char piece_type;

		int best_or_worst = max ?  -300 : 300;

		int count = 0;
		for(int a = 0; legal_moves_list[a]->dest->row != -1; a++)
		{
			srcrow = legal_moves_list[a]->src->row;
			srccol = legal_moves_list[a]->src->col;
			destrow = legal_moves_list[a]->dest->row;
			destcol = legal_moves_list[a]->dest->col;
			piece_type = board[destrow][destcol].type;
			//printf("Moving %c%d to %c%d, we are currently at depth %d, legal move %d\n", srccol+'a', 8-srcrow, destcol+'a', 8-destrow, depth, a);
			move(srccol, srcrow, destcol, destrow);
			//printBoard();

			MoveValTuple* temp_tuple = testMoves(depth+1, other_side, !max, 0);
			value_of_move[a] = temp_tuple->val;
			if(max && best_or_worst > )
			{
				
			}
			else if(!max && best_or_worst < )
			if(max)
			{
				if(temp_tuple->val > best_or_worst_value)
				{
					break;
				}
			}
			else
			{
				if(temp_tuple->val < best_or_worst_value)
				{
					break;
				}
			}
		
			move(destcol, destrow, srccol, srcrow);
			if(piece_type != EMPTY)
			{
				board[destrow][destcol].type = piece_type;
				board[destrow][destcol].side = other_side;
			}
			//sleep(1);

		}
		//printf("Done with part 2\n");
		/*************************************
		Part 3, undo move but find the best value
		*************************************/
		int best_val = max ? -300 : 300;
		int best_index = 0;
		for(int i = 0; value_of_move[i] != -500;i++)
		{
			if(max)
			{
				if(value_of_move[i]>best_val)
				{
					best_val = value_of_move[i];
					best_index = i;
				}
			}
			else
			{
				if(value_of_move[i]<best_val)
				{
					best_val = value_of_move[i];
					best_index = i;
				}
			}
			//printf("Value of moving %c%d to %c%d is %d\n", 'a'+legal_moves_list[i]->src->col, 8-legal_moves_list[i]->src->row, 'a'+legal_moves_list[i]->dest->col, 8-legal_moves_list[i]->dest->row, value_of_move[i]);
		}
		MoveValTuple* vt = (MoveValTuple*) malloc(sizeof(MoveValTuple));
		vt->t = legal_moves_list[best_index];
		vt->val = best_val;
		//free(legal_moves_list);
		//free(temp);

		return vt;
	}
	else
	{
		//printf("Made it to switzerland\n");
		MoveValTuple*  vt = (MoveValTuple *) malloc(sizeof(MoveValTuple));
		vt->val = evalFunc(side == WHITE ? BLACK : WHITE);
		//printf("Returning from eval function with weight %d\n", vt->val);
		return vt;
	}
}

int evalFunc(char side)
{
	int total = 0;
	char other_side = (side == WHITE) ? BLACK : WHITE;
	crds pos;
	crds* legal_moves;
	for(int k = 0; k<8; k++)
	{
		for(int l = 0; l<8; l++)
		{
			if(board[k][l].side == side)
			{
				pos.row = k;
				pos.col = l;
				legal_moves = getLegalMoves(&pos);
				for (int a = 0; legal_moves[a].row != -1; ++a)
				{
					total += val_hash[legal_moves[a].row][legal_moves[a].col];
				}
				total+=names[board[k][l].type].val + val_hash[k][l];
			}
			else if(board[k][l].side == other_side)
			{
				pos.row = k;
				pos.col = l;
				legal_moves = getLegalMoves(&pos);
				for (int a = 0; legal_moves[a].row != -1; ++a)
				{
					total -= val_hash[legal_moves[a].row][legal_moves[a].col];
				}
				total -= val_hash[k][l];
				total -= names[board[k][l].type].val;
			}
		}
	}
	return total;
}

int appendList(crds* src, MoveTuple** main_list, int starting_spot, short int row, short int col)
{
	int temp_spot = 0;
	//printf("The first legal move returned is %c%d\n", src->col + 'a', 8-src->row);

	while(src[temp_spot].row != -1)
	{
		//printf("The legal move we are looking at is %c%d\n", src->col + 'a', 8-src->row);
		main_list[starting_spot]->src->row = row;
		main_list[starting_spot]->src->col = col;
		main_list[starting_spot]->dest->row = src[temp_spot].row;
		main_list[starting_spot]->dest->col = src[temp_spot].col;
		//printf("The legal moves list seems to think we now have %c%d\n", main_list[starting_spot]->dest->col + 'a', 8-main_list[starting_spot]->dest->row);
		starting_spot++;
		temp_spot++;
 	}
 	return starting_spot;
}


int main()
{
	initNames();
	initHash();
	initBoard();
	printBoard();
	global_side = WHITE;

	crds* current_spot = (crds *) malloc(sizeof(crds));

	char ans;
	//printf("Would you like to enter a move (y/n)? ");
	ans = 'y';
	MoveValTuple* computer_move_tuple;
	MoveTuple* computer_move;

	while(ans != 'q')
	{
		if(global_side == WHITE)
		{
			printf("Enter\n'm' to move a piece,\n'l' to see legal moves from a spot,\n'q' to quit\nSelection: ");
			//printf("Would you like to enter a move (y/n)? ");
			ans = getchar();
			flush();
			if(ans == 'm')
			{
				current_spot = getMove();
				printLegalMoves(current_spot);
			}
			else if(ans == 'l')
			{
				char srccol;
				short int srcrow;
				crds* src;
				printf("Enter a spot: ");
				scanf("%c%hu", &srccol, &srcrow);
				flush();
				src = chessToCrds(srccol, srcrow);
				printLegalMoves(src);
			}
			else if(ans != 'q')
			{
				printf("Not a valid selection\n");
			}
		}	
		else
		{
			char current_side = global_side;
			computer_move_tuple = testMoves(0, global_side, 1, 0);
			computer_move = computer_move_tuple->t;
			move(computer_move->src->col, computer_move->src->row, computer_move->dest->col, computer_move->dest->row);
			global_side = global_side == WHITE ? BLACK : WHITE;
			printBoard();
		}
	}
}



