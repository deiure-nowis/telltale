#ifndef CONTROL_H
#define CONTROL_H

#include <SDL2/SDL.h>
#include "player.h"

// Returns 1 if mouse is over a valid tile, and fills isoX/isoY with tile coordinates
// Returns 0 if mouse is outside the grid
int getTileUnderMouse(int mouseX, int mouseY, int* isoX, int* isoY);

void handleInput(int* running, Player* player, const World* world);

#endif // CONTROL_H
