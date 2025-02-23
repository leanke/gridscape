#include <stdio.h>
#include <stdlib.h>
#include "gridscape.h"
#include "client.h"

int main() {
	// Init grid
	int height = 100;
	int width = 100;
	Grid *grid = createGrid(height, width);
	Object tree = {1, "Tree", 1};
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			Object object = {0, "", 0};
			setTile(grid, object, x, y);
			if (x == 0 || x == height - 1 || y == 0 || y == width - 1) {
				setTile(grid, tree, x, y);
			}
		}
	}

	// Object tree = {1, "Tree", 1};
	// setTile(grid, tree, 2, 2);
	// Object rock = {2, "Rock", 1};
	// setTile(grid, rock, 4, 4);
	// Object water = {3, "Water", 1};
	// setTile(grid, water, 6, 6);

	Object player_obj = {4, "Player", 1};
	Player *player = createPlayer(player_obj, 3, 3);
	setTile(grid, player->object, 3, 3);

	// Render Client
	CursesWindow *cursesWindow = initCursesWindow(20, 20);
    renderGrid(cursesWindow, grid, player);

    // Wait for user input and quit if 'q' is pressed
    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case 'w':
                movePlayer(grid, player, 0, -1); // Move player up
                break;
            case 'a':
                movePlayer(grid, player, -1, 0); // Move player left
                break;
            case 's':
                movePlayer(grid, player, 0, 1); // Move player down
                break;
            case 'd':
                movePlayer(grid, player, 1, 0); // Move player right
                break;
        }
        renderGrid(cursesWindow, grid, player);
    }

    closeCursesWindow(cursesWindow);
	free(player);
    freeGrid(grid);

    return 0;
}
