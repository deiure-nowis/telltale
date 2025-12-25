#include "isoGraphic.h"
#include "isoWindow.h"
#include "player.h"
#include "world.h"

static void drawTile(SDL_Renderer* renderer, int isoX, int isoY, int isHovered, TileType tileType) {
	int tx, ty;
	getIsoScreenPos(isoX, isoY, &tx, &ty);
	tx += offsetX;
	ty += offsetY;

	int hw = BASE_HALF_TILE_WIDTH * zoom;
	int hh = BASE_HALF_TILE_HEIGHT * zoom;

	// Fill
	if(isHovered){
		SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255); // Light green
	}else if(tileType == TILE_WALKABLE){
		SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);     // Regular grass green
	}else{
		SDL_SetRenderDrawColor(renderer, 53, 53, 0, 255); // Brown
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
		SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255); // Yellow highlight
	}else if(tileType == TILE_WALKABLE){
		SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);     // Dark green border
	}else{
		SDL_SetRenderDrawColor(renderer, 162, 124, 91, 255); // Light brown
	}

	SDL_RenderDrawLine(renderer, tx, ty, tx + hw, ty + hh); // top to right
	SDL_RenderDrawLine(renderer, tx + hw, ty + hh, tx, ty + 2 * hh); // right to bottom
	SDL_RenderDrawLine(renderer, tx, ty + 2 * hh, tx - hw, ty + hh); // bottom to left
	SDL_RenderDrawLine(renderer, tx - hw, ty + hh, tx, ty); // left to top
}

void drawGrid(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const World* world) {
	for (int isoY = 0; isoY < GRID_SIZE; ++isoY) {
		for (int isoX = 0; isoX < GRID_SIZE; ++isoX) {
			int isHovered = (isoX == hoverIsoX && isoY == hoverIsoY);
			TileType tileType = world->tiles[isoY][isoX];
			drawTile(renderer, isoX, isoY, isHovered, tileType);
		}
	}
}

void drawScene(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const Player* player, const World* world) {
	drawGrid(renderer, hoverIsoX, hoverIsoY, world);
	drawPlayer(renderer, player);
}
