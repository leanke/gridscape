#include <stdio.h>
#include <stdlib.h>
#include "gridscape.h"

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

Tile getTile(Grid *grid, int x, int y) {
    Tile tile = grid->array[x][y];
    return tile;
}
void setTile(Grid *grid, Object object, int x, int y) {
    grid->array[x][y].object = object;
}
void clearTile(Grid *grid, int x, int y) {
    Object object = {0, "", 0};
    setTile(grid, object, x, y);
}

Player *createPlayer(Object object, int x, int y) {
    Player *player = (Player *)malloc(sizeof(Player));
    player->object = object;
    player->x = x;
    player->y = y;
    return player;
}
void movePlayer(Grid *grid, Player *player, int deltaX, int deltaY) {
    if (player->x + deltaX < 0 || 
        player->x + deltaX >= grid->width || 
        player->y + deltaY < 0 || 
        player->y + deltaY >= grid->height ||
        grid->array[player->x + deltaX][player->y + deltaY].object.type != 0
    ) {
        return;
    }
    clearTile(grid, player->x, player->y);
    player->x += deltaX;
    player->y += deltaY;
    setTile(grid, player->object, player->x, player->y);
}


