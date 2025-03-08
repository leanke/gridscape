#ifndef GRIDSCAPE_H
#define GRIDSCAPE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_OBJECTS 100

#define OBJECT_EMPTY 0
#define OBJECT_TREE 1
#define OBJECT_ROCK 2
#define OBJECT_WATER 3
#define OBJECT_PLAYER 4
#define OBJECT_ITEM 5

typedef struct Object {
  int id;
  char name[12];
  int tier;
  int color;
} Object;

typedef struct Player {
  Object object;
  int direction;
  int type;
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

typedef struct GameState {
  Grid *grid;
  Player *player[];
  int *action[];
  // Log *log // TODO
  // LogBuffer // TODO
}

extern int MOVEMENT_DELTA[10][2];

Grid *createGrid(int height, int width);
void freeGrid(Grid *grid);
void initMap(Grid *grid);

Tile getTile(Grid *grid, int x, int y);
void setTile(Grid *grid, Object object, int x, int y);
void movePlayer(Grid *grid, Player *player, int deltaX, int deltaY, int dir);

Player *createPlayer(Object object, int type, int x, int y);
Tile checkTile(Grid *grid, Player *player);

void playerInteract(Grid *grid, Player *player);
void playerAttack(Grid *grid, Player *player);
int canInteractWith(Object object);
int canAttack(Object object);
void clearObject(Grid *grid, int x, int y);

#endif // GRIDSCAPE_H
