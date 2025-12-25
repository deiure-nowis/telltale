#ifndef WORLD_H
#define WORLD_H

#define MAP_SIZE 15

typedef enum {
    TILE_WALKABLE,    // E.g., grass
    TILE_NON_WALKABLE // E.g., rock or wall
} TileType;

typedef struct {
    TileType tiles[MAP_SIZE][MAP_SIZE];
} World;

void initWorld(World* world);
int isTileWalkable(const World* world, int isoX, int isoY);

#endif // WORLD_H
