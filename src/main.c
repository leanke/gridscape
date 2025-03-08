#include "../include/client.h"
#include "../include/gridscape.h"
#include <stdio.h>
#include <stdlib.h>

void debug(Grid *grid) {
  for (int i = 1; i < 8; i++) {
    Object tree = {OBJECT_TREE, "Tree", i, i};
    Object rock = {OBJECT_ROCK, "Rock", i, i};
    setTile(grid, tree, 20, (i + 10));
    setTile(grid, rock, 25, (i + 10));
    // Pond... James Pond
    Object water = {OBJECT_WATER, "Water", 1, 1};
    for (int x = 10; x < 15; x++) {
      for (int y = 10; y < 18; y++) {
        setTile(grid, water, x, y);
      }
    }
  }
}

void placeObject(Grid *grid, Object object, int x, int y) {
  setTile(grid, object, x, y);
}

int main() {
  // Init Grid
  int height = 100;
  int width = 100;
  Grid *grid = createGrid(height, width);
  initMap(grid);

  debug(grid);

  Object playerObj = {OBJECT_PLAYER, "Player", 1, 1};
  Player *player = createPlayer(playerObj, 1, 3, 3);
  Player *new = createPlayer(playerObj, 1, 5, 5);

  setTile(grid, player->object, 3, 3);
  setTile(grid, new->object, 5, 5);
  // Render Client
  CursesWindow *cursesWindow = initCursesWindow();
  renderGrid(cursesWindow, grid, player);

  // Quit on q
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

  closeCursesWindow(cursesWindow);
  free(player);
  freeGrid(grid);

  return 0;
}
