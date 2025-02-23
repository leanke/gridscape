#ifndef CLIENT_H
#define CLIENT_H

#include <ncurses.h>
#include "gridscape.h"

typedef struct CursesWindow {
    int width;
    int height;
    WINDOW *win;
    // struct CursesWindow *left;
    // struct CursesWindow *top;
    // struct CursesWindow *bottom;
} CursesWindow;

CursesWindow *initCursesWindow();
void initColorPairs();
int getCharColor(Tile tile);
void splitCursesWindow(CursesWindow *cursesWindow);
// void renderTextPanel(CursesWindow *cursesWindow, char *text);
// void renderMenuPanel1(CursesWindow *cursesWindow, char *text);
// void renderMenuPanel2(CursesWindow *cursesWindow, char *text);
// void renderScreenPanel(CursesWindow *cursesWindow, char *text);
void renderGrid(CursesWindow *cursesWindow, Grid *grid, Player *player);
void closeCursesWindow(CursesWindow *cursesWindow);
// int get_color(Tile tile);
void resizeCursesWindow(CursesWindow *cursesWindow, int width, int height);

#endif // CLIENT_H
