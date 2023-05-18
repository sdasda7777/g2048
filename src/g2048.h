#include "ClassPad.h"

class Tile{
public:
    Tile() : value( 0 ), blocked( false ) {}
    byte value;
    bool blocked;
};

class g2048 : public PegDecoratedWindow
{
public:
    enum Direction {
        UP      = 0,
        RIGHT   = 1,
        DOWN    = 2,
        LEFT    = 3,
    };

	g2048(PegRect rect, WORD wStyle);
    ~g2048();

	void Draw();
    SIGNED Message(const PegMessage &Mesg);

protected:
    void Save();
    void Load();

    void MoveTiles(Direction d);
    void UnblockAllTiles();
    void AddNewTile();

    void DoFullMove(Direction d)
    {
        Save();
        MoveTiles(d);
        UnblockAllTiles();
        AddNewTile();
    }

    bool TestAdd( int x, int y, UINT value );
    void MoveVertically( int x, int y, int d );
    void MoveHorizontally( int x, int y, int d );
    bool CanMove();

private:
    Tile board[4][4]; // Game board
    Tile last_board[4][4]; // Previous game board for undo purposes

    CP_CHAR* newGameBuffer; // "New game" string buffer
    CP_CHAR* stringBuffer; // general string buffer

    bool win, gameOver, moved;
    UINT score, last_score;
    bool decimalDisplay, showCredits, loadable;

    byte click_origin_x, click_origin_y;
    OBCD obcd_zero, obcd_three, obcd_hundred;
};
