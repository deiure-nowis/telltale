#include "player.h"
#include "isoWindow.h"
#include "world.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
	int x, y;		// Grid position
	int g;			// Cost from start
	int h;			// Heuristic estimate to goal
	int f;			// g + h
	int parentX, parentY; // Parent node for path reconstruction
} Node;

typedef struct{
	Node nodes[MAX_PATH_LENGTH];
	int size;
} PriorityQueue;

static void pqInit(PriorityQueue* pq){
	pq->size = 0;
}

static void pqPush(PriorityQueue* pq, Node node){
	pq->nodes[pq->size++] = node;
	// Sort by f value (bubble up new node)
	for(int i=pq->size - 1; i>0; i--){
		if(pq->nodes[i].f < pq->nodes[i - 1].f){
			Node temp = pq->nodes[i];
			pq->nodes[i] = pq->nodes[i - 1];
			pq->nodes[i - 1] = temp;
		}else{
			break;
		}
	}
}

static Node pqPop(PriorityQueue* pq){
	Node best = pq->nodes[0];
	for(int i=1; i<pq->size; i++)
		pq->nodes[i - 1] = pq->nodes[i];
	pq->size--;
	return best;
}

static int manhattanDistance(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

void initPlayer(Player* player) {
	player->isoX = MAP_SIZE / 2; // Center of map grid
	player->isoY = MAP_SIZE / 2;
	player->dir = DIR_SE; // Start facing southeast
	player->pathLength = 0;
	player->pathIndex = 0;
	player->moving = 0;
	player->lastMoveTime = 0;
}

void findPath(Player* player, int destX, int destY, const World* world){
	if(!isTileWalkable(world, destX, destY)){
		player->pathLength = 0; // Clear path if destination is invalid
		player->moving = 0;
		return;
	}

	PriorityQueue openList;
	pqInit(&openList);

	int closed[MAP_SIZE][MAP_SIZE] = {0};
	int parentX[MAP_SIZE][MAP_SIZE];
	int parentY[MAP_SIZE][MAP_SIZE];
	int bestG[MAP_SIZE][MAP_SIZE];
	
	for(int y=0; y<MAP_SIZE; y++){
		for(int x=0; x<MAP_SIZE; x++){
			parentX[y][x] = -1;
			parentY[y][x] = -1;
			bestG[y][x] = 2147483647;
		}
	}

	Node start = {player->isoX, player->isoY, 0, manhattanDistance(player->isoX, player->isoY, destX, destY), 0, -1, -1};
	start.f = start.g + start.h;

	pqPush(&openList, start);
	bestG[start.y][start.x] = 0;

	int directions[4][2] = {
		{1, 0},		// SE
		{0, -1},	// NE
		{-1, 0},	// NW
		{0, 1}		// SW
	};

	while(openList.size > 0){
		Node current = pqPop(&openList);
		int cx = current.x;
		int cy = current.y;

		if(closed[cy][cx]) continue;
		closed[cy][cx] = 1;

		if(cx == destX && cy == destY){
			// Reconstruct path
			player->pathLength = 0;
			int x = destX, y = destY;
			while(!(x==player->isoX && y==player->isoY)){
				player->path[player->pathLength++] = (Point){x, y};
				int px = parentX[y][x];
				int py = parentY[y][x];
				x = px;
				y = py;
			}
			// Reverse
			for(int i=0; i<player->pathLength / 2; i++){
				Point temp = player->path[i];
				player->path[i] = player->path[player->pathLength - 1 - i];
				player->path[player->pathLength - 1 - i] = temp;
			}

			player->pathIndex = 0;
			player->moving = 1;
			return;
		}

		for(int i=0; i<4; i++){
			int nx = cx + directions[i][0];
			int ny = cy + directions[i][1];

			if(nx<0 || nx>=MAP_SIZE || ny<0 || ny>=MAP_SIZE) continue;
			if(!isTileWalkable(world, nx, ny)) continue;
			if(closed[ny][nx]) continue;

			int newG = current.g + 1;
			if(newG >= bestG[ny][nx]) continue;

			bestG[ny][nx] = newG;
			parentX[ny][nx] = cx;
			parentY[ny][nx] = cy;

			int h = manhattanDistance(nx, ny, destX, destY);
			Node newNode = {nx, ny, newG, h, newG + h, cx, cy};
			pqPush(&openList, newNode);
		}
	}

	// No path found
	player->pathLength = 0;
	player->moving = 0;
}

void updatePlayer(Player* player, const World* world){
	if(!player->moving || player->pathIndex>=player->pathLength){
		player->moving = 0;
		return;
	}

	// Check if enough time passed since last move
	Uint64 currentTime = SDL_GetTicks64();
	if(currentTime<player->lastMoveTime + MOVE_PLAYER_DELAY_MS)
		return;

	// Move to next tile
	int nextX = player->path[player->pathIndex].x;
	int nextY = player->path[player->pathIndex].y;

	// Update direction based on movement
	if(nextX>player->isoX){
		player->dir = DIR_SE;
	}else if(nextX<player->isoX){
		player->dir = DIR_NW;
	}else if(nextY<player->isoY){
		player->dir = DIR_NE;
	}else if(nextY>player->isoY){
		player->dir = DIR_SW;
	}

	// Move player
	player->isoX = nextX;
	player->isoY = nextY;
	player->pathIndex++;
	player->lastMoveTime = currentTime;

	if(player->pathIndex>=player->pathLength)
		player->moving = 0;
}

void drawPlayer(SDL_Renderer* renderer, const Player* player){
	int screenX, screenY;
	projectWorldToScreen((float)player->isoX, (float)player->isoY, &screenX, &screenY);

	int hw = (int)(BASE_HALF_TILE_WIDTH * zoom);
	int hh = (int)(BASE_HALF_TILE_HEIGHT * zoom);

	// Draw gray rectangle
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray

	// Upper half
	for (int y = screenY; y < screenY + hh; ++y) {
		int y_rel = y - screenY;
		int dx = (y_rel * hw) / hh;
		int left_x = screenX - dx;
		int right_x = screenX + dx;
		SDL_RenderDrawLine(renderer, left_x, y, right_x, y);
	}

	// Lower half
	for (int y = screenY + hh; y < screenY + 2 * hh; ++y) {
		int y_rel = y - (screenY + hh);
		int dx = (y_rel * hw) / hh;
		int left_x = (screenX - hw) + dx;
		int right_x = (screenX + hw) - dx;
		SDL_RenderDrawLine(renderer, left_x, y, right_x, y);
	}

	// Draw red direction indicator
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red

	// Adjust direction based on rotation
	Direction rotDir = player->dir;
	switch(rotation){
		case 90:  // 90° clockwise
			if (player->dir == DIR_SE) rotDir = DIR_NE;
			else if (player->dir == DIR_NE) rotDir = DIR_NW;
			else if (player->dir == DIR_NW) rotDir = DIR_SW;
			else if (player->dir == DIR_SW) rotDir = DIR_SE;
			break;
		case 180: // 180°
			if (player->dir == DIR_SE) rotDir = DIR_NW;
			else if (player->dir == DIR_NE) rotDir = DIR_SW;
			else if (player->dir == DIR_NW) rotDir = DIR_SE;
			else if (player->dir == DIR_SW) rotDir = DIR_NE;
			break;
		case 270: // 270° clockwise
			if (player->dir == DIR_SE) rotDir = DIR_SW;
			else if (player->dir == DIR_NE) rotDir = DIR_SE;
			else if (player->dir == DIR_NW) rotDir = DIR_NE;
			else if (player->dir == DIR_SW) rotDir = DIR_NW;
			break;
	}

	int vecEndX = screenX;
	int vecEndY = screenY + hh;

	switch(rotDir){
		case DIR_SE:
			vecEndX = screenX + hw;
			vecEndY = screenY + hh*2;
			break;
		case DIR_NE:
			vecEndX = screenX + hw;
			vecEndY = screenY;
			break;
		case DIR_NW:
			vecEndX = screenX - hw;
			vecEndY = screenY;
			break;
		case DIR_SW:
			vecEndX = screenX - hw;
			vecEndY = screenY + hh*2;
			break;
	}

	SDL_RenderDrawLine(renderer, screenX, screenY + hh, vecEndX, vecEndY);
}
