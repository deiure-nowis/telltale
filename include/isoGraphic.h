#ifndef ISOGRAPHIC_H
#define ISOGRAPHIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "world.h"

typedef struct{
	SDL_Texture* tileGrass;
} GameAssets;

void initAssets(SDL_Renderer* renderer, GameAssets* assets);
void destroyAssets(GameAssets* assets);

void updateTileAnimations(float deltaTime, World* world);
void drawGrid(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const World* world, const GameAssets* assets);
void drawScene(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const Player* player, const World* world, const GameAssets* assets);

#endif // ISOGRAPHIC_H
