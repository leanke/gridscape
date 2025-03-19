#include "gridscape.h"
#include "client.h"  // Added explicit include for client.h
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
LogBuffer* allocate_logbuffer(int size) {
  LogBuffer* logs = (LogBuffer*)calloc(1, sizeof(LogBuffer));
  logs->logs = (Log*)calloc(size, sizeof(Log));
  logs->length = size;
  logs->idx = 0;
  return logs;
}
void free_logbuffer(LogBuffer* buffer) {
  free(buffer->logs);
  free(buffer);
}
void add_log(LogBuffer* logs, Log* log) {
  if (logs->idx == logs->length) {
      return;
  }
  logs->logs[logs->idx] = *log;
  logs->idx += 1;
  //printf("Log: %f, %f, %f\n", log->episode_return, log->episode_length, log->score);
}
Log aggregate_and_clear(LogBuffer* logs) {
  Log log = {0};
  if (logs->idx == 0) {
      return log;
  }
  for (int i = 0; i < logs->idx; i++) {
      log.episode_return += logs->logs[i].episode_return;
      log.episode_length += logs->logs[i].episode_length;
      log.score += logs->logs[i].score;
  }
  log.episode_return /= logs->idx;
  log.episode_length /= logs->idx;
  log.score /= logs->idx;
  logs->idx = 0;
  return log;
}

// TODO: fix below

void init(Gridscape* env) {
  // Init Grid
  int height = 100;
  int width = 100;
  Grid *grid = createGrid(height, width);
  initMap(grid);
  debug(grid);
  Object playerObj = {OBJECT_PLAYER, "Player", 1, 1};
  Player *player = createPlayer(playerObj, 1, 3, 3);
  setTile(grid, player->object, 3, 3);
  
  // Store grid and player in the env struct
  env->grid = grid;
  env->player = player;
  
  // logging
  env->tick = 0;
  env->win = 0;
}

void allocate(Gridscape* env) {
  env->observations = (float*)calloc(8, sizeof(float));
  env->actions = (int*)calloc(2, sizeof(int));
  env->rewards = (float*)calloc(1, sizeof(float));
  env->terminals = (unsigned char*)calloc(1, sizeof(unsigned char));
  env->log_buffer = allocate_logbuffer(LOG_BUFFER_SIZE);
  
  init(env);
}

void compute_observations(Gridscape* env) {
  // Basic observations about the player position and grid boundaries
  env->observations[0] = (float)env->player->x / env->grid->width;  // Normalized X position
  env->observations[1] = (float)env->player->y / env->grid->height; // Normalized Y position
  env->observations[2] = (float)env->player->direction / 9.0f;      // Normalized direction
  env->observations[3] = (float)env->player->object.tier;          
  env->observations[4] = (float)env->tick / 1000.0f;                // Normalized tick count
  env->observations[5] = (float)env->score / env->max_score;        // Progress towards goal
  env->observations[6] = 0.0f;                                      // Reserved
  env->observations[7] = 0.0f;                                      // Reserved
}

void c_reset(Gridscape* env) {
  // Free existing grid and player if they exist
  if (env->grid != NULL) {
    freeGrid(env->grid);
  }
  if (env->player != NULL) {
    free(env->player);
  }
  
  env->log = (Log){0};
  env->score = 0;
  
  // Initialize new grid and player
  init(env);
  compute_observations(env);
}

void reset_round(Gridscape* env) {
  // Reset for next round without completely resetting the environment
  env->tick = 0;
}

void c_step(Gridscape* env) {
  env->tick += 1;
  env->log.episode_length += 1;
  env->rewards[0] = 0;
  env->terminals[0] = 0;
  
  // step logic here
  if (env->actions[0] >= 1 && env->actions[0] <= 9 && env->actions[0] != 5) {
    int index = env->actions[0];
    movePlayer(env->grid, env->player, MOVEMENT_DELTA[index][0], MOVEMENT_DELTA[index][1], index);
  } else if (env->actions[0] == 10) { // Use 10 for 'e'/'E' (interact)
    playerInteract(env->grid, env->player);
  } else if (env->actions[0] == 11) { // Use 11 for 'a'/'A' (attack)
    playerAttack(env->grid, env->player);
  }

  env->score += 1;
  env->rewards[0] = 1; // agent wins
  env->log.episode_return += 1;
  env->log.score += 1.0;
  
  if (env->score >= env->max_score) {
    env->terminals[0] = 1;
    add_log(env->log_buffer, &env->log);
    c_reset(env);
    return;
  } else {
    compute_observations(env);
  }
}

void free_allocated(Gridscape* env) {
  free(env->observations);
  free(env->actions);
  free(env->rewards);
  free(env->terminals);
  free(env->player);
  freeGrid(env->grid);
  free_logbuffer(env->log_buffer);
}

// Replace the client rendering stub with actual implementation
CursesWindow* make_client(Gridscape* env) {
  // Initialize ncurses window
  CursesWindow* client = initCursesWindow();
  return client;
}

void close_client(CursesWindow* client) {
  // Clean up ncurses
  closeCursesWindow(client);
}

void c_render(CursesWindow* client, Gridscape* env) {
  // Use the ncurses rendering implementation
  renderGrid(client, env->grid, env->player);
}
