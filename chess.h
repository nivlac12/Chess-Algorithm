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
	BLACK,
	LEGALWHITE,
	LEGALBLACK,
	LEGALEMPTY
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
	crds* src;
	crds* dest;
} MoveTuple;

typedef struct
{
	MoveTuple* t;
	int val;
}MoveValTuple;

typedef struct
{
	crds src;
	crds dest;
	char PieceCaptured;
} UndoMoveTuple;

typedef struct
{
	char * fullName;
	short int val;
	char abbrev;
} PieceName;
void initBoard(void);
void initHash(void);
void initNames(void);
void printBoard(void);
crds* getLegalMoves(crds*);
crds* getLegalMovesPawn(crds*);
crds* getLegalMovesRook(crds*);
crds* getLegalMovesKnight(crds*);
crds* getLegalMovesBishop(crds*);
crds* getLegalMovesQueen(crds*);
crds* getLegalMovesKing(crds*);
void spotValid(short int, short int, short int, crds*, short int*);
MoveValTuple* testMoves(char, char, char, char, int, int);
int appendList(crds*, MoveTuple**, int, short int, short int);
int evalFunc(char);
crds* chessToCrds(char, short int);
void move(short int, short int, short int, short int);
void moveCrds(char, short int, char, short int);
void moveCrds(char, short int, char, short int);
void runPVP(void);
void runAgainstComputer(char);