#ifndef CLIENT_H
#define CLIENT_H

#include "gridscape.h"
#include <ncurses.h>

// Define the CursesWindow struct
typedef struct CursesWindow {
  int width;
  int height;
  WINDOW *win;
  WINDOW *topWin;
  WINDOW *bottomWin;
} CursesWindow;

// Forward declarations for grid-related types
struct Grid;
struct Player;
typedef struct Grid Grid;
typedef struct Player Player;

// Function declarations
CursesWindow *initCursesWindow();
void initColorPairs();
int getCharColor(struct Tile tile);
void splitCursesWindow(CursesWindow *cursesWindow);
void renderGrid(CursesWindow *cursesWindow, Grid *grid, Player *player);
void closeCursesWindow(CursesWindow *cursesWindow);
void resizeCursesWindow(CursesWindow *cursesWindow, int width, int height);

#endif // CLIENT_H