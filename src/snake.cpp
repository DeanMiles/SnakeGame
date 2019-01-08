#include "snake.h"
#include "cpoint.h"
#define TRUE 1
#define FALSE 0
#define speed_value 16
#include"screen.h"
#include "screen.h"
#include <list>


CSnake::CSnake(CRect r, char _c /*=' '*/):
  CFramedWindow(r, _c)
{
  beginSet();
  score = Body.size();
  foodPos = randCoord();
}

void CSnake::paint()					//main function
{
  paintBoard();
  CFramedWindow::paint();		     		//printin window from the other file
  KindOfFood();
  displayPoints();
  //authormode();
  options();
}

void CSnake::Moving(const CPoint &value)		//control of Moving
{
  if( Body.size() == FALSE ) 
  return;
  for(unsigned int i=Body.size(); i > 0; --i) 
    if( Body[0].x+value.x == Body[i].x && Body[0].y+value.y == Body[i].y) 
    { 						
	gameOver = true; 
	menu = true; 
        flag = FALSE;
        paint();
	return; 
    }
    else
    Body[i] = Body[i-1];
  
  Body[0].x = geom.topleft.x + (Body[0].x - geom.topleft.x + value.x) % (geom.size.x -2);    //przejscie
  if(Body[0].x <= geom.topleft.x) 
	Body[0].x = geom.topleft.x + geom.size.x-2;

  Body[0].y = geom.topleft.y + (Body[0].y - geom.topleft.y + value.y) % (geom.size.y -2);
  if(Body[0].y <= geom.topleft.y) 
	Body[0].y = geom.topleft.y + geom.size.y-2;
}

void CSnake::MoveSnakeWindow(const CPoint &value)      //movin snake if window change posit.
{
  for( unsigned int i = 0 ; i < Body.size() ; ++i )
    Body[i] += value;
}

void CSnake::paintSnake()				//drawin snake
{
  gotoyx( Body[0].y, Body[0].x);
  printl("*");
  for( unsigned int i = 1 ; i < Body.size() ; ++i )
  {
    gotoyx( Body[i].y, Body[i].x) ;
    printl("+") ;
  }
}

void CSnake::eatin(const CPoint foodXY)             //eatin and randin premium
{
  Body.push_back(foodXY);
  ++score;
  if(score > 5)
  premium = rand()%5;
  if( premium % 5 == 1)
  flag = true ;
  else
  flag = false ;
  foodPos = randCoord();
  gameSpeed = 256/( speed_value + score );
}

void CSnake::reset()
{
  flag = false;
  premium = 2;
  gameOver = false;
  menu = true;
  pause = false;
  signpost = FALSE ;
  Body.clear();
  beginSet();
  score = Body.size();
  directSnake = CPoint(1, 0);
  paintSnake();
}

void CSnake::start()					
{
  flag = false;
  gameSpeed = speed_value;
  premium = 2;
  gameOver = false;
  pause = false;
  signpost = FALSE ;
  Body.clear();
  beginSet();
  score = Body.size();
  directSnake = CPoint(1, 0);
  paintSnake();
}

bool CSnake::handleEvent(int key)			  //control game
{
  if(!menu && !pause && !gameOver)
  {
    if( foodPos.x == Body[0].x && foodPos.y == Body[0].y ) 
    {
	if( flag)
	{
	eatin(foodPos);
        score++;
	}
	else	
	eatin(foodPos);		
    }							    
    if( !(++counter % gameSpeed) )
    {
      Moving(directSnake);
      paint();
    }
    
    switch(key)
    {
      case 'w':  
	if(signpost!=2)  
	{                          
           Moving(directSnake = CPoint (0, -1));
           signpost = 0;
	}
       return true;
      case 's':
	if(signpost!=0)  
	{                            
           Moving(directSnake = CPoint (0, 1));
           signpost = 2;
	}
        return true;
      case 'd':
        if(signpost!=3)  
	{                            
           Moving(directSnake = CPoint (1, 0));
           signpost = 1;
	}
        return true;
      case 'a':
	if(signpost!=1)
	{                            
           Moving(directSnake = CPoint (-1, 0));
           signpost = 3;
	}
        return true;
      case 'p': 
        pause = true;
	flag = false;
	paint();
        return true;
      case 'h':                                  
        menu = !menu;
        return true;
      case 'r': 
        reset();
        return true;
      case 'f':
	start();
	menu = !menu;
	return true;
    }
    return true;
  }
  else
  {
    switch (key)
      {
      case KEY_UP:
        MoveSnakeWindow(CPoint (0, -1));                                  
        move (CPoint (0, -1));
        foodPos += CPoint(0, -1);
        return true;
      case KEY_DOWN:
        MoveSnakeWindow(CPoint (0, 1));
        move (CPoint (0, 1));
        foodPos += CPoint(0, 1);
        return true;
      case KEY_RIGHT:
        MoveSnakeWindow(CPoint (1, 0));
        move (CPoint (1, 0));
        foodPos += CPoint(1, 0);
        return true;
      case KEY_LEFT:
        MoveSnakeWindow(CPoint (-1, 0));
        move (CPoint (-1, 0));
        foodPos += CPoint(-1, 0);
        return true;
      case 'p': 
        pause = false;
        return true;
      case 'h': 
        menu = !menu;
        return true;
      case 'r':
        reset();
        return true;
      case 'f':
	start();
	menu = !menu;
	return true;
      };
    return false;
  }
}

void CSnake::options()					//main function
{
  if( gameOver )
  {
    gotoyx(geom.topleft.y+1, geom.topleft.x+1);
    printl("GameOver, result : %u", score);
  }

  if( flag )
  {
    gotoyx(0, 12);
    printl("PREMIUM TIME !!!");
  }

  if( pause )
  {
    gotoyx(geom.topleft.y+10, geom.topleft.x+20);
    printl("PAUSE");
  }

  if( menu )
    printMenu();
  
  if( !menu && !pause && !gameOver )
    paintSnake();
}

void CSnake::paintBoard()		           //displayin lvl of the game
{
  gotoyx(geom.topleft.y-1, geom.topleft.x-1);
  printl("| LEVEL: %u |", score/3);         
}

void CSnake::KindOfFood()                           //acceleration game by double premium p
{
  if(!flag && !menu && !pause)
  {
  gotoyx(foodPos.y, foodPos.x);
  printl("x");
  }
  else if(!menu && !pause)
  {
  gotoyx(foodPos.y, foodPos.x);
  printl("$");
  }
}

void CSnake::displayPoints()
{
  gotoyx(0,0);
  printl("points: %d",score-2);

  gotoyx(1,0);
  printl("ping: %d",rand() % 30 + 60);
}

void CSnake::beginSet()
{
  Body.push_back(CPoint(geom.topleft.x+14, geom.topleft.y+9));   //settin coord for begin
  Body.push_back(CPoint(geom.topleft.x+13, geom.topleft.y+9));
  Body.push_back(CPoint(geom.topleft.x+12, geom.topleft.y+9));
}

CPoint CSnake::randCoord()
{
  srand(time(NULL));
  for( int i = 0 ; i <=1 ; i++ )
  {
  int first = geom.topleft.x + rand() % (geom.size.x-2)+1;
  int second = geom.topleft.y + rand()%(geom.size.y-2)+1;
  food = CPoint( first, second);
      //if( Body[j].x == first && Body[j].y == second)
      for( unsigned int j = 1 ; j < Body.size() ; ++j )    //avoidin rand food in snake's body
      {							    //works 100%
        if(Body[j].y==second && Body[j].x==first) 
	    i--;
      }
  }
  return food;
}

void CSnake::printMenu()
{
    gotoyx(geom.topleft.y+5, geom.topleft.x+6);
    printl("h - toggle for help information");
    gotoyx(geom.topleft.y+6, geom.topleft.x+6);
    printl("p - toggle pause/play mode");
    gotoyx(geom.topleft.y+7, geom.topleft.x+6);
    printl("r - restart game");
    gotoyx(geom.topleft.y+8, geom.topleft.x+6);
    printl("'wasd'- move snake(in play mode)");
    gotoyx(geom.topleft.y+9, geom.topleft.x+9);
    printl("or move window(in pause mode)");
    gotoyx(geom.topleft.y+10, geom.topleft.x+9);    
    printl("by arrows");
    gotoyx(geom.topleft.y+11, geom.topleft.x+6);
    printl("f - begin game");
}

void CSnake::authormode()
{
    gotoyx(45,0);
    printl("author: Alan Franas");
}
