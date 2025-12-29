#include "isoGraphic.h"
#include "isoWindow.h"
#include "player.h"
#include "world.h"
#include <math.h>

static void drawTile(SDL_Renderer* renderer, int isoX, int isoY, int isHovered, TileType tileType, TileParams tileParams) {
	int tx, ty;
	projectWorldToScreen((float)isoX, (float)isoY, &tx, &ty);

	// Apply height drop and alpha
	float dropOffset = (1.0f - tileParams.alpha) * 100.0f;
	ty += (int)dropOffset;
	Uint8 a = (Uint8)(tileParams.alpha * 255);

	int hw = (int)(BASE_HALF_TILE_WIDTH * zoom);
	int hh = (int)(BASE_HALF_TILE_HEIGHT * zoom);

	// Fill
	if(isHovered){
		SDL_SetRenderDrawColor(renderer, 100, 255, 100, a); // Light green
	}else if(tileType == TILE_WALKABLE){
		SDL_SetRenderDrawColor(renderer, 0, 200, 0, a); // Regular grass green
	}else{
		SDL_SetRenderDrawColor(renderer, 53, 53, 0, a); // Brown
	}

	// Upper half
	for (int y = ty; y < ty + hh; ++y) {
		int y_rel = y - ty;
		int dx = (y_rel * hw) / hh;
		int left_x = tx - dx;
		int right_x = tx + dx;
		SDL_RenderDrawLine(renderer, left_x, y, right_x, y);
	}

	// Lower half
	for (int y = ty + hh; y < ty + 2 * hh; ++y) {
		int y_rel = y - (ty + hh);
		int dx = (y_rel * hw) / hh;
		int left_x = (tx - hw) + dx;
		int right_x = (tx + hw) - dx;
		SDL_RenderDrawLine(renderer, left_x, y, right_x, y);
	}

	// Border
	if(isHovered){
		SDL_SetRenderDrawColor(renderer, 255, 255, 100, a); // Yellow highlight
	}else if(tileType == TILE_WALKABLE){
		SDL_SetRenderDrawColor(renderer, 0, 100, 0, a); // Dark green border
	}else{
		SDL_SetRenderDrawColor(renderer, 162, 124, 91, a); // Light brown
	}

	SDL_RenderDrawLine(renderer, tx, ty, tx + hw, ty + hh); // top to right
	SDL_RenderDrawLine(renderer, tx + hw, ty + hh, tx, ty + 2 * hh); // right to bottom
	SDL_RenderDrawLine(renderer, tx, ty + 2 * hh, tx - hw, ty + hh); // bottom to left
	SDL_RenderDrawLine(renderer, tx - hw, ty + hh, tx, ty); // left to top
}

void updateTileAnimations(float deltaTime, World* world){
    float speed = deltaTime / TILE_ANIM_TIME;
    int half = VISIBLE_GRID_SIZE / 2;

	int minX = (int)cameraIsoX - half;
	int minY = (int)cameraIsoY - half;
	int maxX = minX + VISIBLE_GRID_SIZE - 1;
	int maxY = minY + VISIBLE_GRID_SIZE - 1;

	int searMinX = minX - 10, searMinY = minY - 10;
	int searMaxX = maxX + 10, searMaxY = maxY + 10;

	if(searMinX < 0) searMinX = 0;
	if(searMinY < 0) searMinY = 0;
	if(searMaxX > MAP_SIZE - 1) searMaxX = MAP_SIZE - 1;
	if(searMaxY > MAP_SIZE - 1) searMaxY = MAP_SIZE - 1;

	for(int y=searMinY; y<=searMaxY; y++){
		for(int x=searMinX; x<=searMaxX; x++){
			TileParams* v = &world->params[y][x];
			int isVisible = (x>=minX && x<=maxX && y>=minY && y<=maxY);

			v->alpha += (isVisible - v->alpha) * speed;
			if(v->alpha < 0.01f) v->alpha = 0.0f;
		}
	}
}

void drawGrid(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const World* world){
	int half = VISIBLE_GRID_SIZE / 2;

	int min_isoX = (int)cameraIsoX - half;
	int min_isoY = (int)cameraIsoY - half;

	int max_isoX = min_isoX + VISIBLE_GRID_SIZE - 1;
	int max_isoY = min_isoY + VISIBLE_GRID_SIZE - 1;

	int anim_minX = min_isoX - 5;
	int anim_minY = min_isoY - 5;
	int anim_maxX = max_isoX + 5;
	int anim_maxY = max_isoY + 5;

	if(anim_minX < 0) anim_minX = 0;
	if(anim_minY < 0) anim_minY = 0;
	if(anim_maxX > MAP_SIZE - 1) anim_maxX = MAP_SIZE - 1;
	if(anim_maxY > MAP_SIZE - 1) anim_maxY = MAP_SIZE - 1;

	for(int isoY=anim_minY; isoY<=anim_maxY; ++isoY){
		for(int isoX=anim_minX; isoX<=anim_maxX; ++isoX){
			int isHovered = (isoX == hoverIsoX && isoY == hoverIsoY);
			TileType tileType = world->tiles[isoY][isoX];
			TileParams tileParams = world->params[isoY][isoX];
			drawTile(renderer, isoX, isoY, isHovered, tileType, tileParams);
		}
	}
}

void drawScene(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const Player* player, const World* world){
	drawGrid(renderer, hoverIsoX, hoverIsoY, world);

	int half = VISIBLE_GRID_SIZE / 2;
	int min_isoX = (int)cameraIsoX - half;
	int min_isoY = (int)cameraIsoY - half;
	int max_isoX = min_isoX + VISIBLE_GRID_SIZE - 1;
	int max_isoY = min_isoY + VISIBLE_GRID_SIZE - 1;

	if(min_isoX < 0) min_isoX = 0;
	if(min_isoY < 0) min_isoY = 0;
	if(max_isoX > MAP_SIZE - 1) max_isoX = MAP_SIZE - 1;
	if(max_isoY > MAP_SIZE - 1) max_isoY = MAP_SIZE - 1;

	int px = player->isoX;
	int py = player->isoY;
	if(px>=min_isoX && px<=max_isoX && py>=min_isoY && py<=max_isoY)
		drawPlayer(renderer, player);
}
