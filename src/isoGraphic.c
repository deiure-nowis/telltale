#include "isoGraphic.h"
#include "isoWindow.h"
#include "player.h"
#include "world.h"
#include <math.h>

static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path){
	SDL_Surface* surface = IMG_Load(path);
	if(!surface){
		SDL_Log("Failed to load image %s: %s", path, IMG_GetError());
		return NULL;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void initAssets(SDL_Renderer* renderer, GameAssets* assets){
	assets->tileGrass = loadTexture(renderer, "assets/images/tileAssets.png");
}

void destroyAssets(GameAssets* assets){
	if(assets->tileGrass) SDL_DestroyTexture(assets->tileGrass);
}

static void drawTile(SDL_Renderer* renderer, int isoX, int isoY, int isHovered, Tile tile, const GameAssets* assets) {
	int tx, ty;
	projectWorldToScreen((float)isoX, (float)isoY, &tx, &ty);

	// Apply height drop and alpha
	float dropOffset = (1.0f - tile.alpha) * 100.0f;
	ty += (int)dropOffset;
	Uint8 a = (Uint8)(tile.alpha * 255);

	if(!assets->tileGrass) return;
	SDL_Texture* texture = assets->tileGrass;

	// dest rect set
	SDL_Rect destRect;
	int texW = 64;
	int texH = 48;
	destRect.w = (int)(texW * zoom);
	destRect.h = (int)(texH * zoom);

	destRect.x = tx - (destRect.w / 2);
	destRect.y = ty;

	// source rect set
	SDL_Rect srcRect;
	int nativeW = 32;
	int nativeH = 24;
	srcRect.x = tile.variantX * nativeW;
	srcRect.y = tile.variantY * nativeH;
	srcRect.w = nativeW;
	srcRect.h = nativeH;

	SDL_SetTextureAlphaMod(texture, a);

	// Hover
	if(isHovered){
		SDL_SetTextureColorMod(texture, 155, 200, 155);
	}else{
		SDL_SetTextureColorMod(texture, 255, 255, 255);
	}

	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
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
			Tile* tile = &world->grid[y][x];
			int isVisible = (x>=minX && x<=maxX && y>=minY && y<=maxY);

			tile->alpha += (isVisible - tile->alpha) * speed;
			if(tile->alpha < 0.01f) tile->alpha = 0.0f;
			if(tile->alpha > 0.99f) tile->alpha = 1.0f;
		}
	}
}

void drawGrid(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const World* world, const GameAssets* assets){
	int half = VISIBLE_GRID_SIZE / 2;

	int min_isoX = (int)cameraIsoX - half;
	int min_isoY = (int)cameraIsoY - half;

	int max_isoX = min_isoX + VISIBLE_GRID_SIZE - 1;
	int max_isoY = min_isoY + VISIBLE_GRID_SIZE - 1;

	int anim_minX = min_isoX - 5;
	int anim_minY = min_isoY - 5;
	int anim_maxX = max_isoX + 5;
	int anim_maxY = max_isoY + 5;

	// Clamp
	if(anim_minX < 0) anim_minX = 0;
	if(anim_minY < 0) anim_minY = 0;
	if(anim_maxX > MAP_SIZE - 1) anim_maxX = MAP_SIZE - 1;
	if(anim_maxY > MAP_SIZE - 1) anim_maxY = MAP_SIZE - 1;

	// Depth sorting
	int xs, xe, xstep;
	int ys, ye, ystep;
	switch(rotation){
		case 0:
			xs = anim_minX; xe = anim_maxX; xstep = 1;
			ys = anim_minY; ye = anim_maxY; ystep = 1;
			break;
		case 90:
			xs = anim_maxX; xe = anim_minX; xstep = -1;
			ys = anim_minY; ye = anim_maxY; ystep = 1;
			break;
		case 180:
			xs = anim_maxX; xe = anim_minX; xstep = -1;
			ys = anim_maxY; ye = anim_minY; ystep = -1;
			break;
		case 270:
			xs = anim_minX; xe = anim_maxX; xstep = 1;
			ys = anim_maxY; ye = anim_minY; ystep = -1;
			break;
		default:
			xs = anim_minX; xe = anim_maxX; xstep = 1;
			ys = anim_minX; ye = anim_maxX; ystep = 1;
			break;
	}

	for(int isoY=ys; isoY!=ye + ystep; isoY+=ystep){
		for(int isoX=xs; isoX!=xe + xstep; isoX+=xstep){
			if(world->grid[isoY][isoX].type == TILE_WALKABLE){
				int isHovered = (isoX == hoverIsoX && isoY == hoverIsoY);
				Tile tile = world->grid[isoY][isoX];
				drawTile(renderer, isoX, isoY, isHovered, tile, assets);
			}
		}
	}
}

void drawScene(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const Player* player, const World* world, const GameAssets* assets){
	drawGrid(renderer, hoverIsoX, hoverIsoY, world, assets);

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
