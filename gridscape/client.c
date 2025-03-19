#include "client.h"

#include <ncurses.h>
#include <stdio.h>

CursesWindow *initCursesWindow() {
  initscr();
  noecho();
  cbreak();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  int viewWidth, viewHeight;
  getmaxyx(stdscr, viewHeight, viewWidth);
  initColorPairs();

  CursesWindow *cursesWindow = (CursesWindow *)malloc(sizeof(CursesWindow));
  cursesWindow->width = viewWidth;
  cursesWindow->height = viewHeight;
  cursesWindow->win = newwin(viewHeight, viewWidth, 0, 0);
  cursesWindow->topWin = NULL;
  cursesWindow->bottomWin = NULL;

  splitCursesWindow(cursesWindow);

  return cursesWindow;
}

void splitCursesWindow(CursesWindow *cursesWindow) {
  int topHeight = cursesWindow->height * 3 / 4;
  int bottomHeight = cursesWindow->height - topHeight;

  cursesWindow->topWin = newwin(topHeight, cursesWindow->width, 0, 0);
  cursesWindow->bottomWin =
      newwin(bottomHeight, cursesWindow->width, topHeight, 0);

  box(cursesWindow->topWin, 0, 0);
  box(cursesWindow->bottomWin, 0, 0);

  wrefresh(cursesWindow->topWin);
  wrefresh(cursesWindow->bottomWin);
}

void renderGrid(CursesWindow *cursesWindow, Grid *grid, Player *player) {
  WINDOW *targetWin = cursesWindow->topWin;

  werase(targetWin);
  int viewWidth, viewHeight;
  getmaxyx(targetWin, viewHeight, viewWidth);
  int startX = player->x - viewWidth / 2;
  int startY = player->y - viewHeight / 2;
  for (int y = 0; y < viewHeight - 2; y++) { // -2 for border adn same below
    for (int x = 0; x < viewWidth - 2; x++) {
      int gridX = startX + x;
      int gridY = startY + y;

      if (gridX >= 0 && gridX < grid->width && gridY >= 0 &&
          gridY < grid->height) {
        Tile tile = grid->array[gridX][gridY];
        int color = COLOR_PAIR(21);

        if (tile.object.id == 0) {
          wattron(targetWin, color);
          mvwprintw(targetWin, y + 1, x + 1, "%c", '.');
          wattroff(targetWin, color);
        } else if (gridX == player->x && gridY == player->y) {
          wattron(targetWin, color);
          mvwprintw(targetWin, y + 1, x + 1, "%c", '@');
          wattroff(targetWin, color);
        } else if (tile.object.id == OBJECT_WATER) {
          color = COLOR_PAIR(77);
          wattron(targetWin, color);
          mvwprintw(targetWin, y + 1, x + 1, "%c", tile.object.id);
          wattroff(targetWin, color);
        } else {
          color = getCharColor(tile);
          wattron(targetWin, color);
          mvwprintw(targetWin, y + 1, x + 1, "%c", tile.object.name[0]);
          wattroff(targetWin, color);
        }
      }
    }
  }

  box(targetWin, 0, 0);
  box(cursesWindow->bottomWin, 0, 0);
  mvwprintw(cursesWindow->bottomWin, 1, 2, "Use keys 1-9 to move, 'q' to quit");
  wrefresh(targetWin);
  wrefresh(cursesWindow->bottomWin);
}

void closeCursesWindow(CursesWindow *cursesWindow) {
  if (cursesWindow->topWin != NULL) {
    delwin(cursesWindow->topWin);
  }
  if (cursesWindow->bottomWin != NULL) {
    delwin(cursesWindow->bottomWin);
  }
  delwin(cursesWindow->win);
  endwin();
  free(cursesWindow);
}

void resizeCursesWindow(CursesWindow *cursesWindow, int width, int height) {
  cursesWindow->width = width;
  cursesWindow->height = height;
  wresize(cursesWindow->win, height, width);
  mvwin(cursesWindow->win, 0, 0);
  if (cursesWindow->topWin != NULL && cursesWindow->bottomWin != NULL) {
    int topHeight = height * 2 / 3;
    int bottomHeight = height - topHeight;

    wresize(cursesWindow->topWin, topHeight, width);
    mvwin(cursesWindow->topWin, 0, 0);

    wresize(cursesWindow->bottomWin, bottomHeight, width);
    mvwin(cursesWindow->bottomWin, topHeight, 0);
  }

  wrefresh(cursesWindow->win);
  if (cursesWindow->topWin != NULL) {
    wrefresh(cursesWindow->topWin);
  }

  if (cursesWindow->bottomWin != NULL) {
    wrefresh(cursesWindow->bottomWin);
  }
}

void initColorPairs() {
  start_color();
  init_pair(11, COLOR_WHITE, COLOR_WHITE);
  init_pair(12, COLOR_BLACK, COLOR_WHITE);
  init_pair(13, COLOR_GREEN, COLOR_WHITE);
  init_pair(14, COLOR_BLUE, COLOR_WHITE);
  init_pair(15, COLOR_RED, COLOR_WHITE);
  init_pair(16, COLOR_YELLOW, COLOR_WHITE);
  init_pair(17, COLOR_CYAN, COLOR_WHITE);
  init_pair(18, COLOR_MAGENTA, COLOR_WHITE);

  init_pair(21, COLOR_WHITE, COLOR_BLACK);
  init_pair(22, COLOR_BLACK, COLOR_BLACK);
  init_pair(23, COLOR_GREEN, COLOR_BLACK);
  init_pair(24, COLOR_BLUE, COLOR_BLACK);
  init_pair(25, COLOR_RED, COLOR_BLACK);
  init_pair(26, COLOR_YELLOW, COLOR_BLACK);
  init_pair(27, COLOR_CYAN, COLOR_BLACK);
  init_pair(28, COLOR_MAGENTA, COLOR_BLACK);

  init_pair(31, COLOR_WHITE, COLOR_GREEN);
  init_pair(32, COLOR_BLACK, COLOR_GREEN);
  init_pair(33, COLOR_GREEN, COLOR_GREEN);
  init_pair(34, COLOR_BLUE, COLOR_GREEN);
  init_pair(35, COLOR_RED, COLOR_GREEN);
  init_pair(36, COLOR_YELLOW, COLOR_GREEN);
  init_pair(37, COLOR_CYAN, COLOR_GREEN);
  init_pair(38, COLOR_MAGENTA, COLOR_GREEN);

  init_pair(41, COLOR_WHITE, COLOR_BLUE);
  init_pair(42, COLOR_BLACK, COLOR_BLUE);
  init_pair(43, COLOR_GREEN, COLOR_BLUE);
  init_pair(44, COLOR_BLUE, COLOR_BLUE);
  init_pair(45, COLOR_RED, COLOR_BLUE);
  init_pair(46, COLOR_YELLOW, COLOR_BLUE);
  init_pair(47, COLOR_CYAN, COLOR_BLUE);
  init_pair(48, COLOR_MAGENTA, COLOR_BLUE);

  init_pair(51, COLOR_WHITE, COLOR_RED);
  init_pair(52, COLOR_BLACK, COLOR_RED);
  init_pair(53, COLOR_GREEN, COLOR_RED);
  init_pair(54, COLOR_BLUE, COLOR_RED);
  init_pair(55, COLOR_RED, COLOR_RED);
  init_pair(56, COLOR_YELLOW, COLOR_RED);
  init_pair(57, COLOR_CYAN, COLOR_RED);
  init_pair(58, COLOR_MAGENTA, COLOR_RED);

  init_pair(61, COLOR_WHITE, COLOR_YELLOW);
  init_pair(62, COLOR_BLACK, COLOR_YELLOW);
  init_pair(63, COLOR_GREEN, COLOR_YELLOW);
  init_pair(64, COLOR_BLUE, COLOR_YELLOW);
  init_pair(65, COLOR_RED, COLOR_YELLOW);
  init_pair(66, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(67, COLOR_CYAN, COLOR_YELLOW);
  init_pair(68, COLOR_MAGENTA, COLOR_YELLOW);

  init_pair(71, COLOR_WHITE, COLOR_CYAN);
  init_pair(72, COLOR_BLACK, COLOR_CYAN);
  init_pair(73, COLOR_GREEN, COLOR_CYAN);
  init_pair(74, COLOR_BLUE, COLOR_CYAN);
  init_pair(75, COLOR_RED, COLOR_CYAN);
  init_pair(76, COLOR_YELLOW, COLOR_CYAN);
  init_pair(77, COLOR_CYAN, COLOR_CYAN);
  init_pair(78, COLOR_MAGENTA, COLOR_CYAN);

  init_pair(81, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(82, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(83, COLOR_GREEN, COLOR_MAGENTA);
  init_pair(84, COLOR_BLUE, COLOR_MAGENTA);
  init_pair(85, COLOR_RED, COLOR_MAGENTA);
  init_pair(86, COLOR_YELLOW, COLOR_MAGENTA);
  init_pair(87, COLOR_CYAN, COLOR_MAGENTA);
  init_pair(88, COLOR_MAGENTA, COLOR_MAGENTA);
}

int getCharColor(Tile tile) {
  int fg = COLOR_WHITE;
  int bg = 0;
  int color = tile.object.color;

  switch (color) {
  case 1:
    fg = 1;
    break;
  case 2:
    fg = 3;
    break;
  case 3:
    fg = 6;
    break;
  case 4:
    fg = 5;
    break;
  case 5:
    fg = 3;
    bg = 80;
    break;
  case 6:
    fg = 6;
    bg = 80;
    break;
  case 7:
    fg = 5;
    bg = 80;
    break;
  case 8:
    fg = 7;
    bg = 80;
    break;
  default:
    fg = 1;
    break;
  }

  if (bg == 0) {
    bg = 20;
  }
  if (fg == 0) {
    fg = 1;
  }
  return COLOR_PAIR(fg + bg);
}
