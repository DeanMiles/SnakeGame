#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include <vector>
#include <ctime>

class CSnake:public CFramedWindow
{
  vector<CPoint> Body;              //wektor
  CPoint speed = CPoint(1, 0);
  CPoint foodPos;
  CPoint food;
  int gameSpeed = 16;
  unsigned int counter = 0;
  bool pause = false;
  bool menu = true;
  bool gameOver = false;

public:
  int signpost = 1;
  unsigned int score;
  void start();
  bool flag = false;
  int premium=0;
  CSnake(CRect r, char _c = ' ');
  void Moving(const CPoint&);
  void MoWindow(const CPoint &delta);
  void eatin(CPoint);
  bool handleEvent(int key);
  void paint();
  void paintBoard();
  void KindOfFood();
  void displayPoints();
  void beginSet();
  CPoint randCoord();
  void printMenu();
  void reset();
  void paintSnake();
  void authormode();
};

#endif
