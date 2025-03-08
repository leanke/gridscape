#ifndef CLIENT_H
#define CLIENT_H

#include "gridscape.h"
#include <ncurses.h>

typedef struct CursesWindow {
  int width;
  int height;
  WINDOW *win;
  WINDOW *topWin;
  WINDOW *bottomWin;
} CursesWindow;

CursesWindow *initCursesWindow();
void initColorPairs();
int getCharColor(Tile tile);
void splitCursesWindow(CursesWindow *cursesWindow);

void renderGrid(CursesWindow *cursesWindow, Grid *grid, Player *player);
void closeCursesWindow(CursesWindow *cursesWindow);
// int get_color(Tile tile);
void resizeCursesWindow(CursesWindow *cursesWindow, int width, int height);

#endif // CLIENT_H
