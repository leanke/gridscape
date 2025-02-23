#include "../include/client.h"
#include <ncurses.h>
#include <stdio.h> // For debug printing

CursesWindow *initCursesWindow() {
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    int viewWidth, viewHeight;
    getmaxyx(stdscr, viewHeight, viewWidth);
    // initColorPairs();

    CursesWindow *cursesWindow = (CursesWindow *)malloc(sizeof(CursesWindow));
    cursesWindow->width = viewWidth;
    cursesWindow->height = viewHeight;
    cursesWindow->win = newwin(viewHeight, viewWidth, 0, 0);
    // cursesWindow->left = NULL;
    // cursesWindow->top = NULL;
    // cursesWindow->bottom = NULL;
    return cursesWindow;
}

void renderGrid(CursesWindow *cursesWindow, Grid *grid, Player *player) {
    werase(cursesWindow->win);
    int viewWidth, viewHeight;
    getmaxyx(cursesWindow->win, viewHeight, viewWidth);
    int startX = player->x - viewWidth / 2;
    int startY = player->y - viewHeight / 2;
    for (int y = 0; y < viewHeight; y++) {
        for (int x = 0; x < viewWidth; x++) {
            int gridX = startX + x;
            int gridY = startY + y;
            // char displayChar = '.';
            // int color = COLOR_PAIR(21);

            if (gridX >= 0 && gridX < grid->width && gridY >= 0 && gridY < grid->height) {
                Tile tile = grid->array[gridX][gridY];
                // color = getCharColor(tile);

                // char displayChar = tile->object->name[0];
                if (tile.object.type == 0) {
                    // wattron(cursesWindow->win, color);
                    mvwprintw(cursesWindow->win, y, x, "%c", '.'); //, displayChar);
                    // wattroff(cursesWindow->win, color);
                } else {
                    // wattron(cursesWindow->win, color);
                    mvwprintw(cursesWindow->win, y, x, "%c", tile.object.name[0]); //, displayChar);
                    // wattroff(cursesWindow->win, color);
                }

            }
        }
    }
    wrefresh(cursesWindow->win);
}

void closeCursesWindow(CursesWindow *cursesWindow) {
    delwin(cursesWindow->win);
    endwin();
    free(cursesWindow);
}

void splitCursesWindow(CursesWindow *cursesWindow) {
    int splitHeight = cursesWindow->height / 2;
    int width, height;
    width = cursesWindow->width;
    height = cursesWindow->height;
    CursesWindow *left = (CursesWindow *)malloc(sizeof(CursesWindow));
    CursesWindow *top = (CursesWindow *)malloc(sizeof(CursesWindow));
    CursesWindow *bottom = (CursesWindow *)malloc(sizeof(CursesWindow));
    left->width = width / 2;
    left->height = height;
    left->win = newwin(height, width / 2, 0, 0);
    top->width = width;
    top->height = splitHeight;
    top->win = newwin(splitHeight, width, 0, 0);
    bottom->width = width;
    bottom->height = height - splitHeight;
    bottom->win = newwin(height - splitHeight, width, splitHeight, 0);
    // cursesWindow->left = left;
    // cursesWindow->top = top;
    // cursesWindow->bottom = bottom;
}

void resizeCursesWindow(CursesWindow *cursesWindow, int width, int height) {
    cursesWindow->width = width;
    cursesWindow->height = height;
    wresize(cursesWindow->win, height, width);
    mvwin(cursesWindow->win, 0, 0);
    wrefresh(cursesWindow->win);
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
    int bg = COLOR_BLACK;
    int tier = tile.object.tier;


    switch (tier) {
        case 1:
            fg = COLOR_WHITE;
            break;
        case 2:
            fg = COLOR_GREEN;
            break;
        case 3:
            fg = COLOR_YELLOW;
            break;
        case 4:
            fg = COLOR_RED;
            break;
        case 5:
            fg = COLOR_GREEN;
            bg = COLOR_MAGENTA;
            break;
        case 6:
            fg = COLOR_YELLOW;
            bg = COLOR_MAGENTA;
            break;
        case 7:
            fg = COLOR_RED;
            bg = COLOR_MAGENTA;
            break;
        case 8:
            fg = COLOR_CYAN;
            bg = COLOR_MAGENTA;
            break;
        default:
            fg = COLOR_WHITE;
            break;
    }

    return COLOR_PAIR(fg + (bg * 10));
}
