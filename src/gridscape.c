#include "../include/gridscape.h"
#include <stdio.h>
#include <stdlib.h>

Grid *createGrid(int height, int width) {
  Grid *grid = (Grid *)malloc(sizeof(Grid));
  grid->height = height;
  grid->width = width;
  grid->array = (Tile **)malloc(height * sizeof(Tile *));
  for (int i = 0; i < height; i++) {
    grid->array[i] = (Tile *)malloc(width * sizeof(Tile));
  }
  return grid;
}
void freeGrid(Grid *grid) {
  for (int i = 0; i < grid->height; i++) {
    free(grid->array[i]);
  }
  free(grid->array);
  free(grid);
}
void initMap(Grid *grid) {
  int height = grid->height;
  int width = grid->width;

  Object tree = {OBJECT_TREE, "Tree", 1, 1};

  for (int x = 0; x < height; x++) {
    for (int y = 0; y < width; y++) {
      Object object = {0, "", 0, 1};
      setTile(grid, object, x, y);
      if (x == 0 || x == height - 1 || y == 0 || y == width - 1) {
        setTile(grid, tree, x, y);
      }
    }
  }
}

int MOVEMENT_DELTA[10][2] = {
    {0, 0},   // 0 (not used)
    {-1, 1},  // 1 - down, left
    {0, 1},   // 2 - down
    {1, 1},   // 3 - down, right
    {-1, 0},  // 4 - left
    {0, 0},   // 5 - none (TODO: make interaction)
    {1, 0},   // 6 - right
    {-1, -1}, // 7 - up, left
    {0, -1},  // 8 - up
    {1, -1}   // 9 - up, right
};

Tile getTile(Grid *grid, int x, int y) {
  Tile tile = grid->array[x][y];
  return tile;
}
void setTile(Grid *grid, Object object, int x, int y) {
  grid->array[x][y].object = object;
}
void clearTile(Grid *grid, int x, int y) {
  Object object = {0, "", 0, 1};
  setTile(grid, object, x, y);
}
void movePlayer(Grid *grid, Player *player, int deltaX, int deltaY, int dir) {
  clearTile(grid, player->x, player->y);
  if (player->x + deltaX < 0 || player->x + deltaX >= grid->width ||
      player->y + deltaY < 0 || player->y + deltaY >= grid->height ||
      grid->array[player->x + deltaX][player->y + deltaY].object.id != 0) {
    deltaY = 0;
    deltaX = 0;
  }
  player->direction = dir;
  player->x += deltaX;
  player->y += deltaY;
  setTile(grid, player->object, player->x, player->y);
}

Player *createPlayer(Object object, int type, int x, int y) {
  Player *player = (Player *)malloc(sizeof(Player));
  player->object = object;
  player->type = type;
  player->x = x;
  player->y = y;
  return player;
}
Tile checkTile(Grid *grid, Player *player) {
  int direction = player->direction;
  int dX = MOVEMENT_DELTA[direction][0];
  int dY = MOVEMENT_DELTA[direction][1];
  Tile tile = getTile(grid, (player->x + dX), (player->y + dY));
  return tile;
}

int canInteractWith(Object object) {
  switch (object.id) {
  case OBJECT_TREE:
  case OBJECT_ROCK:
  case OBJECT_ITEM:
    return 1;
  default:
    return 0;
  }
}

int canAttack(Object object) {
  switch (object.id) {
  case OBJECT_PLAYER:
    return 1;
  case OBJECT_WATER:
  case OBJECT_TREE:
  case OBJECT_ROCK:
  case OBJECT_EMPTY:
    return 0;
  default:
    return 1;
  }
}

void clearObject(Grid *grid, int x, int y) {
  Object emptyObject = {OBJECT_EMPTY, "", 0, 1};
  setTile(grid, emptyObject, x, y);
}

void playerInteract(Grid *grid, Player *player) {
  int direction = player->direction;
  int dX = MOVEMENT_DELTA[direction][0];
  int dY = MOVEMENT_DELTA[direction][1];
  int targetX = player->x + dX;
  int targetY = player->y + dY;

  if (targetX < 0 || targetX >= grid->width || targetY < 0 ||
      targetY >= grid->height) {
    return;
  }

  Tile targetTile = getTile(grid, targetX, targetY);

  if (!canInteractWith(targetTile.object)) {
    return;
  }

  switch (targetTile.object.id) {
  case OBJECT_TREE:
    if (targetTile.object.tier <= player->object.tier) {
      clearObject(grid, targetX, targetY);
    }
    break;
  case OBJECT_ROCK:
    if (targetTile.object.tier <= player->object.tier) {
      clearObject(grid, targetX, targetY);
    }
    break;
  case OBJECT_ITEM:
    clearObject(grid, targetX, targetY);
    break;
  default:
    break;
  }
}

void playerAttack(Grid *grid, Player *player) {
  int direction = player->direction;
  int dX = MOVEMENT_DELTA[direction][0];
  int dY = MOVEMENT_DELTA[direction][1];
  int targetX = player->x + dX;
  int targetY = player->y + dY;

  if (targetX < 0 || targetX >= grid->width || targetY < 0 ||
      targetY >= grid->height) {
    return;
  }

  Tile targetTile = getTile(grid, targetX, targetY);

  if (!canAttack(targetTile.object)) {
    return;
  }

  if (player->object.tier >= targetTile.object.tier) {
    clearObject(grid, targetX, targetY);
  }
}

void c_init() {
  // Init Grid
  int height = 100;
  int width = 100;
  Grid *grid = createGrid(height, width);
  initMap(grid);
  // CursesWindow *cursesWindow = initCursesWindow();

  debug(grid);

  Object playerObj = {OBJECT_PLAYER, "Player", 1, 1};
  Player *player = createPlayer(playerObj, 1, 3, 3);
  // Player *new = createPlayer(playerObj, 1, 5, 5);

  setTile(grid, player->object, 3, 3);
  // setTile(grid, new->object, 5, 5);
}

void c_reset(Grid *grid, Player *player) {
  // closeCursesWindow(cursesWindow);
  free(player);
  freeGrid(grid);
  c_init()
}

// void c_render() {
// // Render Client
// renderGrid(cursesWindow, grid, player);
//
// }
void c_step(Grid *grid, Player *player, int action[]) {

  int ch;
  while ((ch = getch()) != 'q') {
    if (ch >= '1' && ch <= '9' && ch != '5') {
      int index = ch - '0';
      movePlayer(grid, player, MOVEMENT_DELTA[index][0],
                 MOVEMENT_DELTA[index][1], index);
      renderGrid(cursesWindow, grid, player);
    } else if (ch == 'e' || ch == 'E') {
      playerInteract(grid, player);
      renderGrid(cursesWindow, grid, player);
    } else if (ch == 'a' || ch == 'A') {
      playerAttack(grid, player);
      renderGrid(cursesWindow, grid, player);
    } else if (ch == '5') {
      Tile frontTile = checkTile(grid, player);
      mvwprintw(cursesWindow->bottomWin, 2, 2,
                "Looking at: %s (ID: %d, Tier: %d)      ",
                frontTile.object.id ? frontTile.object.name : "Empty",
                frontTile.object.id, frontTile.object.tier);
      wrefresh(cursesWindow->bottomWin);
    }
  }

  return 0;
}
