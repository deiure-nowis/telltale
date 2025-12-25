#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "world.h"

#define MAX_PATH_LENGTH (MAP_SIZE * MAP_SIZE) // Max possible path length
#define MOVE_PLAYER_DELAY_MS 500 // Delay between moves in milliseconds

typedef enum{
	DIR_SE, // Southeast
	DIR_NE, // Northeast
	DIR_NW, // Northwest
	DIR_SW  // Southwest
} Direction;

typedef struct{
	int x, y;
} Point;

typedef struct{
	int isoX;		// Isometric grid X position
	int isoY;		// Isometric grid Y position
	Direction dir;	// Facing direction
	Point path[MAX_PATH_LENGTH]; // Array to store path points
	int pathLength;	// Number of points in the path
	int pathIndex;	// Current step in the path
	int moving;		// 1 if following a path, 0 if idle
	Uint64 lastMoveTime; // Time of last move (milliseconds)
} Player;

void initPlayer(Player* player);
void drawPlayer(SDL_Renderer* renderer, const Player* player);
void findPath(Player* player, int destX, int destY, const World* world);
void updatePlayer(Player* player, const World* world);

#endif // PLAYER_H
