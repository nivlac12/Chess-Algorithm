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
void initBoard(void);
void initNames(void);
void printBoard(void);
crds* getLegalMoves(crds*);
crds* getLegalMovesPawn(crds*);
crds* getLegalMovesRook(crds*);
crds* getLegalMovesKnight(crds*);
crds* getLegalMovesBishop(crds*);
crds* getLegalMovesQueen(crds*);
crds* getLegalMovesKing(crds*);
crds* chessToCrds(char, short int);
void move(short int, short int, short int, short int);
void moveCrds(char, short int, char, short int);
void moveCrds(char, short int, char, short int);