#include "world.h"
#include <string.h>

void initWorld(World* world){
	// Initialize all tiles as walkable (grass) by default
	for(int y=0; y<MAP_SIZE; y++){
		for(int x=0; x<MAP_SIZE; x++){
			world->tiles[y][x] = TILE_WALKABLE;
			world->params[y][x].alpha = 0.0f;
		}
	}

	// Add some non-walkable tiles (obstacles) for testing
	world->tiles[51][52] = TILE_NON_WALKABLE;
	world->tiles[57][59] = TILE_NON_WALKABLE;
	world->tiles[58][56] = TILE_NON_WALKABLE;
	world->tiles[55][55] = TILE_NON_WALKABLE;
	world->tiles[60][60] = TILE_NON_WALKABLE;
	
	world->tiles[28][26] = TILE_NON_WALKABLE;
	world->tiles[25][25] = TILE_NON_WALKABLE;
	world->tiles[30][30] = TILE_NON_WALKABLE;
}

int isTileWalkable(const World* world, int isoX, int isoY){
	// Check bounds
	if(isoX < 0 || isoX >= MAP_SIZE || isoY < 0 || isoY >= MAP_SIZE)
		return 0; // Out of bounds is non-walkable
	return world->tiles[isoY][isoX] == TILE_WALKABLE;
}
