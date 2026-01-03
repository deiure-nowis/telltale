#include "world.h"
#include <string.h>

void initWorld(World* world){
	// Initialize all tiles as walkable (grass) by default
	for(int y=0; y<MAP_SIZE; y++){
		for(int x=0; x<MAP_SIZE; x++){
			world->grid[y][x].type = TILE_WALKABLE;
			world->grid[y][x].variantX = 0;
			world->grid[y][x].variantY = 0;
			world->grid[y][x].alpha = 0;
		}
	}

	// Add some non-walkable tiles (obstacles) for testing
	world->grid[51][52].type = TILE_NON_WALKABLE;
	world->grid[57][59].type = TILE_NON_WALKABLE;
	world->grid[58][56].type = TILE_NON_WALKABLE;
	world->grid[55][55].type = TILE_NON_WALKABLE;
	world->grid[60][60].type = TILE_NON_WALKABLE;
}

int isTileWalkable(const World* world, int isoX, int isoY){
	// Check bounds
	if(isoX < 0 || isoX >= MAP_SIZE || isoY < 0 || isoY >= MAP_SIZE)
		return 0; // Out of bounds is non-walkable
	return world->grid[isoY][isoX].type == TILE_WALKABLE;
}
