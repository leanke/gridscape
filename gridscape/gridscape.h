#ifndef GRIDSCAPE_H
#define GRIDSCAPE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Forward declaration
typedef struct CursesWindow CursesWindow;

#define LOG_BUFFER_SIZE 1024
#define MAX_OBJECTS 100
#define OBJECT_EMPTY 0
#define OBJECT_TREE 1
#define OBJECT_ROCK 2
#define OBJECT_WATER 3
#define OBJECT_PLAYER 4
#define OBJECT_ITEM 5

typedef struct Log {
    float episode_return;
    float episode_length;
    float score;
} Log;
typedef struct LogBuffer {
    Log* logs;
    int length;
    int idx;
} LogBuffer;
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
typedef struct Gridscape {
  Grid *grid;
  Player *player;

  float* observations;
  int* actions;
  float* rewards;
  unsigned char* terminals;
  LogBuffer* log_buffer;
  Log log;
  float width;
  float height;
  int score;
  int max_score;
  int tick;
  int win;
  int frameskip;

} Gridscape;

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
LogBuffer* allocate_logbuffer(int size);
void free_logbuffer(LogBuffer* buffer);
void add_log(LogBuffer* logs, Log* log);
Log aggregate_and_clear(LogBuffer* logs);

// Client-related functions (now using the forward-declared CursesWindow)
CursesWindow* make_client(Gridscape* env);
void close_client(CursesWindow* client);
void c_render(CursesWindow* client, Gridscape* env);

// Core game functions
void init(Gridscape* env);
void compute_observations(Gridscape* env);
void c_reset(Gridscape* env);
void c_step(Gridscape* env);
void reset_round(Gridscape* env);
void allocate(Gridscape* env);
void free_allocated(Gridscape* env);

#endif // GRIDSCAPE_H