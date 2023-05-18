#include "g2048.h"
#include "Basics.h"

g2048::g2048(PegRect rect, WORD wStyle)
    : PegDecoratedWindow(rect, wStyle)
{
    // The "New game" string is loaded from a variable,
    // due to that it can be overwritten to show information for debugging
    newGameBuffer = new CP_CHAR[40]();
    const char* newGame = "New game";
    for (int ii = 0; ii < 9; ++ii)
        newGameBuffer[ii] = newGame[ii];
    stringBuffer = new CP_CHAR[40]();

    win = false, gameOver = false, moved = false;
    score = 0, last_score = 0;
    decimalDisplay = true, showCredits = false, loadable = false;
    click_origin_x = 0, click_origin_y = 0;

    Cal_longto_OBC(0, &obcd_zero);
    Cal_longto_OBC(3, &obcd_three);
    Cal_longto_OBC(100, &obcd_hundred);

    AddNewTile();
}

g2048::~g2048()
{
    delete newGameBuffer;
    delete stringBuffer;
}

// Start of code from RosettaCode.org (?)

void g2048::UnblockAllTiles(){
    for( int y = 0; y < 4; y++ )
    {
	    for( int x = 0; x < 4; x++ )
        {
		    board[x][y].blocked = false;
        }
    }
}

bool g2048::TestAdd( int x, int y, UINT value )
{
	if( x < 0 || x > 3 || y < 0 || y > 3 )
        return false;
	return board[x][y].value == value;
}

bool g2048::CanMove(){
	for( int y = 0; y < 4; y++ ){
	    for( int x = 0; x < 4; x++ ){
		     if( !board[x][y].value ){
                  return true;
             }
        }
    }
	for( int y = 0; y < 4; y++ ){
	    for( int x = 0; x < 4; x++ ){
		     if( TestAdd( x + 1, y, board[x][y].value ) ) return true;
		     if( TestAdd( x - 1, y, board[x][y].value ) ) return true;
		     if( TestAdd( x, y + 1, board[x][y].value ) ) return true;
		     if( TestAdd( x, y - 1, board[x][y].value ) ) return true;
	    }
    }
	return false;
}

void g2048::AddNewTile()
{
	for( int y = 0; y < 4; y++ )
    {
	    for( int x = 0; x < 4; x++ )
        {
            if( !board[x][y].value )
            {
		        short a, b, s;
                OBCD obcd_tempa;
		        do {
                    Cal_Random(1, &obcd_zero, &obcd_three, &obcd_tempa);
                    Cal_toshort_OBC(&obcd_tempa, &a);
                    Cal_Random(1, &obcd_zero, &obcd_three, &obcd_tempa);
                    Cal_toshort_OBC(&obcd_tempa, &b);
                } while ( board[a][b].value );
 
                Cal_Random(1, &obcd_zero, &obcd_hundred, &obcd_tempa);
	       	    Cal_toshort_OBC(&obcd_tempa, &s);
                board[a][b].value = ( s > 89 ? 2 : 1 );
    		    if( CanMove() )
                {
                     return;
                }
		    }
        }
    }
	gameOver = true;
}

void g2048::MoveVertically( int x, int y, int d )
{
    if( board[x][y + d].value && board[x][y + d].value == board[x][y].value
        && !board[x][y].blocked && !board[x][y + d].blocked  )
    {
	    board[x][y].value = 0;
	    board[x][y + d].value++;
	    score += Basics::Pow(2, board[x][y + d].value);
	    board[x][y + d].blocked = true;
	    moved = true;
	}
    else if( !board[x][y + d].value && board[x][y].value )
    {
	    board[x][y + d].value = board[x][y].value;
	    board[x][y].value = 0;
	    moved = true;
	}
	if( d > 0 )
    {
        if( y + d < 3 )
        {
            MoveVertically( x, y + d,  1 );
        }
    }
    else
    { 
        if( y + d > 0 )
        {
            MoveVertically( x, y + d, -1 );
        }
    }
}

void g2048::MoveHorizontally( int x, int y, int d )
{
	if( board[x + d][y].value && board[x + d][y].value == board[x][y].value
        && !board[x][y].blocked && !board[x + d][y].blocked  )
    {
	    board[x][y].value = 0;
	    board[x + d][y].value++;
	    score += Basics::Pow(2, board[x + d][y].value);
	    board[x + d][y].blocked = true;
	    moved = true;
	}
    else if( !board[x + d][y].value && board[x][y].value )
    {
	    board[x + d][y].value = board[x][y].value;
	    board[x][y].value = 0;
	    moved = true;
	}
	if( d > 0 )
    { 
        if( x + d < 3 )
        {
             MoveHorizontally( x + d, y,  1 );
        }
    }
    else
    {
        if( x + d > 0 )
        {
             MoveHorizontally( x + d, y, -1 );
        }
    }
}

void g2048::MoveTiles( g2048::Direction d )
{
	switch( d )
	{
	    case UP:
	    	for( int x = 0; x < 4; x++ )
            {
                 for( int y = 1; y < 4; ++y )
                 {
                      if( board[x][y].value )
                      {
                           MoveVertically( x, y, -1 );
                      }
                 }
		    }
		break;
	    case DOWN:
		     for( int x = 0; x < 4; x++ )
             {
                  for( int y = 2; y >= 0; --y )
                  {
                       if( board[x][y].value )
                       {
                            MoveVertically( x, y, 1 );
                       }
                  }
		     }
		break;
	    case RIGHT:
		     for( int y = 0; y < 4; y++ )
             {
                  for( int x = 2; x >= 0; --x )
                  {
                       if( board[x][y].value )
                       {
                            MoveHorizontally( x, y, 1 );
                       }
                  }
		     }
        break;
        case LEFT:
		     for( int y = 0; y < 4; y++ )
             {
                  for( int x = 1; x < 4; ++x )
                  {
                       if( board[x][y].value )
                       {
                            MoveHorizontally( x, y, -1 );
                       }
                  }
		     }
        break;
	}
}

void g2048::Save(){
     last_score = score;

     for(int i = 0; i<4; i++)
     {
          for(int j = 0; j<4; j++)
          {
               last_board[i][j].value = board[i][j].value;
          }        
     }

     loadable = true;
}

void g2048::Load()
{
     score = last_score;
     
     for(int i = 0; i<4; i++)
     {
          for(int j = 0; j<4; j++)
          {
               board[i][j].value = last_board[i][j].value;
          }
     }
     
     gameOver = false;
     loadable = false;
}

void g2048::Draw()
{
	BeginDraw();
	ClearScreen();
	DrawFrame();
	DrawChildren();

    //Defines "color" to paint with
    PegColor color(BLACK, WHITE, CF_FILL);	

    //Draws vertical grid lines
    Line(0, 50, 0, 209, color);
    Line(39, 50, 39, 209, color);
    Line(79, 50, 79, 209, color);
    Line(119, 50, 119, 209, color);
    Line(159, 50, 159, 209, color);

    //Draws horizontal grid lines    
    Line(0, 50, 159, 50, color);    
    Line(0, 89, 159, 89, color);
    Line(0, 129, 159, 129, color);
    Line(0, 169, 159, 169, color);
    Line(0, 209, 159, 209, color);
    
    //Draws current numbers into grid
    for( int y = 0; y < 4; y++ )
    {
	    for( int x = 0; x < 4; x++ )
        {
     		if( board[x][y].value )
            {
                PegPoint p2 = {19+40*x,69+40*y};
                //CircleR( p2.x, p2.y, 20, color, 1);

                if(decimalDisplay)
                {
                    CP_IntToString(Basics::Pow(2, board[x][y].value), stringBuffer);
                }
                else
                {
                    CP_IntToString(board[x][y].value, stringBuffer);
                }

                // Center-align the text
                int textWidth = TextWidth((PEGCHAR*)stringBuffer, PegTextThing::GetBasicFont());
                int textHeight = TextHeight((PEGCHAR*)stringBuffer, PegTextThing::GetBasicFont());
                p2.x -= textWidth / 2;
                p2.y -= textHeight / 2;

                DrawText( p2, (PEGCHAR*)stringBuffer, color, PegTextThing::GetBasicFont());
            }
	    }
	}
    
    // Draws horizontal GUI lines
    Line(0, 0, 159, 0, color);
    Line(0, 19, 159, 19, color);
        
    // Draws vertical GUI lines
    Line(0, 0, 0, 19, color);
    Line(70, 0, 70, 19, color);
    Line(119, 0, 119, 19, color);
    Line(139, 0, 139, 19, color);
    Line(159, 0, 159, 19, color);
    
	PegPoint pn = {10,5};
    PegPoint pu = {80,5};
    PegPoint pq = {147,5};
    DrawText( pn, (PEGCHAR*)newGameBuffer, color, PegTextThing::GetBasicFont());
    DrawText( pu, "Undo", color, PegTextThing::GetBasicFont());
    
    if(decimalDisplay){
        PegPoint pd = {127,5};
        DrawText( pd, "d", color, PegTextThing::GetBasicFont());
    }else{
        PegPoint pd = {121,5};
        DrawText( pd, "2^n", color, PegTextThing::GetBasicFont());  
    }
    
    if(showCredits){
         CircleR(150,10,9, color, 1);
         
         PegRect p;
         p.Set(10,30,150,220);
         RectangleR(p, color);
         
         PegPoint p1 = {40,35};
         PegPoint d = {0,15};
         DrawText( p1, "Game by:", color, PegTextThing::GetBasicFont()); p1 = p1+d;
         DrawText( p1, "sdasda7777", color, PegTextThing::GetBasicFont()); p1 = p1+d;
         // Your name here?
         
         p1 = p1+d;
         DrawText( p1, "Sources:", color, PegTextThing::GetBasicFont()); p1 = p1+d;
         DrawText( p1, "rosettacode/2048", color, PegTextThing::GetBasicFont()); p1 = p1+d;
         DrawText( p1, "ClassPad SDK", color, PegTextThing::GetBasicFont()); p1 = p1+d;
    }
    DrawText( pq, "?", color, PegTextThing::GetBasicFont());
	
	// Draws current score on status bar
	PegPoint psmallsc = {10,230};
    CP_IntToString(score, stringBuffer);
	CPString strys("score: ");
    CPString strscore((PEGCHAR*)stringBuffer);    
    DrawText( psmallsc, strys+strscore, color, PegTextThing::GetBasicFont());
	
	// Draws game over screen
	if(gameOver){
        PegRect p;
        p.Set(10,80,150,160);
        RectangleR(p, color);
        
        CP_IntToString(score, stringBuffer);
        
        PegPoint pgo = {50,95};
        PegPoint psc = {40,125};
        DrawText( pgo, "Game over!", color, PegTextThing::GetBasicFont());

        CPString strys("Your score: ");
        CPString strscore((PEGCHAR*)stringBuffer);
        
        DrawText( psc, strys+strscore, color, PegTextThing::GetBasicFont());
    }

	EndDraw();
}

SIGNED g2048::Message(const PegMessage &message)
{
	switch(message.wType)
    {
        case PM_LBUTTONDOWN:
        {
            click_origin_x = message.Point.x;
            click_origin_y = message.Point.y;
            break;
        }
        case PM_LBUTTONUP: // touch commands
        {
            if(!gameOver && !showCredits
               && click_origin_y > 20 && click_origin_y < 230
               && message.Point.y > 20 && message.Point.y < 230
               && (Basics::Abs(click_origin_x - message.Point.x) > 2
                   || Basics::Abs(click_origin_y - message.Point.y) > 2))
            {
                int dx = Basics::Abs(click_origin_x - message.Point.x);
                int dy = Basics::Abs(click_origin_y - message.Point.y);

                if(dx > dy)
                {
                    if(click_origin_x > message.Point.x)
                    {
                        DoFullMove(LEFT);
                    }
                    else
                    {
                        DoFullMove(RIGHT);
                    }
                }
                else
                {
                    if(click_origin_y > message.Point.y)
                    {
                        DoFullMove(UP);
                    }
                    else
                    {
                        DoFullMove(DOWN);
                    }
                }
            }
            else if(!showCredits
                    && click_origin_x > 0 && click_origin_x < 70
                    && click_origin_y > 0 && click_origin_y < 20
                    && message.Point.x > 0 && message.Point.x < 70
                    && message.Point.y > 0 && message.Point.y < 20)
            {
                // "New game" button
                for(int i = 0; i<4; i++)
                {
                    for(int j = 0; j<4; j++)
                    {
                        board[i][j].value = 0;
                    }
                }
                       
                win = false;
                gameOver = false;
                moved = false;
                score = 0;

                loadable = false;
                       
                AddNewTile();
            }
            else if(!showCredits
                    && click_origin_x > 70 && click_origin_x < 120
                    && click_origin_y > 0 && click_origin_y < 20
                    && message.Point.x > 70 && message.Point.x < 120
                    && message.Point.y > 0 && message.Point.y < 20)
            {
                // "Undo" button
                if(loadable){
                    Load();
                }
            }
            else if(!showCredits
                    && click_origin_x > 120 && click_origin_x < 140
                    && click_origin_y > 0 && click_origin_y < 20
                    && message.Point.x > 120 && message.Point.x < 140
                    && message.Point.y > 0 && message.Point.y < 20)
            {
                // switch number modes button ("d"/"2^n")
                decimalDisplay = !decimalDisplay;
            }
            else if(click_origin_x > 140 && click_origin_x < 160
                    && click_origin_y > 0 && click_origin_y < 20
                    && message.Point.x > 140 && message.Point.x < 160
                    && message.Point.y > 0 && message.Point.y < 20)
            {
                // credits button ("?")
                showCredits = !showCredits;
            }
             
            Redraw();
            break;
        }
        case PM_KEY: // key commands
        {
            if (!showCredits)
            {
                if (message.iData == 8) // backspace (=undo) key command
                {
                    if(loadable){
                        Load();
                    }
                }
                else if (!gameOver)
                {
                    if (message.iData == 52 // "4"
                        || message.iData == 608) // "LEFT"
                    {
                        DoFullMove(LEFT);
                    }
                    else if (message.iData == 53 // "5"
                             || message.iData == 607) // "DOWN"
                    {
                        DoFullMove(DOWN);
                    }
                    else if (message.iData == 54 // "6"
                             || message.iData == 609) // "RIGHT"
                    {
                        DoFullMove(RIGHT);
                    }
                    else if (message.iData == 56 // "8"
                             || message.iData == 606) // "UP"
                    {
                        DoFullMove(UP);
                    }
                }
            }

            // For keypress debugging:
            // CP_IntToString(message.iData, newGameBuffer);
            
            Redraw();
            break;
        }
        default:
	        return PegDecoratedWindow::Message(message);
    }
    return 0;
}
