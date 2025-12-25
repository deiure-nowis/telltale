#include "world.h"

void initWorld(World* world) {
    // Initialize all tiles as walkable (grass) by default
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            world->tiles[y][x] = TILE_WALKABLE;
        }
    }

    // Add some non-walkable tiles (obstacles) for testing
    // Example: a small cluster near the center and a few scattered
    world->tiles[1][2] = TILE_NON_WALKABLE; // Center
    world->tiles[7][9] = TILE_NON_WALKABLE; // Right of center
    world->tiles[8][6] = TILE_NON_WALKABLE; // Below center
    world->tiles[5][5] = TILE_NON_WALKABLE; // Another obstacle
    world->tiles[10][10] = TILE_NON_WALKABLE; // Another obstacle
}

int isTileWalkable(const World* world, int isoX, int isoY) {
    // Check bounds
    if (isoX < 0 || isoX >= MAP_SIZE || isoY < 0 || isoY >= MAP_SIZE) {
        return 0; // Out of bounds is non-walkable
    }
    return world->tiles[isoY][isoX] == TILE_WALKABLE;
}
