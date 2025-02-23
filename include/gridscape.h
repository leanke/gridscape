#ifndef GRIDSCAPE_H
#define GRIDSCAPE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Object {
	int type;
	char name[12];
	int tier;
} Object;

typedef struct Player {
	Object object;
	int x;
	int y;
} Player;

typedef struct Tile {
	int id;
	Object object;
} Tile;

typedef struct Grid {
	int height;
	int width;
	Tile **array;
} Grid;

Grid *createGrid(int height, int width);
void freeGrid(Grid *grid);
Tile getTile(Grid *grid, int x, int y);
void setTile(Grid *grid, Object object, int x, int y);
void movePlayer(Grid *grid, Player *player, int deltaX, int deltaY);
Player *createPlayer(Object object, int x, int y) ;

#endif // GRIDSCAPE_H