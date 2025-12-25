#ifndef ISOGRAPHIC_H
#define ISOGRAPHIC_H

#include <SDL2/SDL.h>
#include "player.h"
#include "world.h"

void drawGrid(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const World* world);
void drawScene(SDL_Renderer* renderer, int hoverIsoX, int hoverIsoY, const Player* player, const World* world);

#endif // ISOGRAPHIC_H
