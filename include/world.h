#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#define MAP_SIZE 100

#define X_VARIANT 4
#define Y_VARIANT 3

typedef enum{
	TILE_WALKABLE,    // E.g., grass
	TILE_NON_WALKABLE // E.g., rock or wall
} TileType;

typedef struct{
	TileType type;
	uint8_t variantX;
	uint8_t variantY;
	float alpha;
} Tile;

typedef struct{
	Tile grid[MAP_SIZE][MAP_SIZE];
} World;

void initWorld(World* world);
int isTileWalkable(const World* world, int isoX, int isoY);

#endif // WORLD_H
