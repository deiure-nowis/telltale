#ifndef ISOWINDOW_H
#define ISOWINDOW_H

// Window
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640

// UI Grid
#define VISIBLE_GRID_SIZE 15
#define BASE_HALF_TILE_WIDTH 32
#define BASE_HALF_TILE_HEIGHT 16

// Camera
#define CAMERA_PAN_SPEED 400.0f
#define MIN_ZOOM 0.5f
#define MAX_ZOOM 5.0f

// Animations
#define TILE_ANIM_TIME 0.2f // Seconds to fully appear/disappear

extern float zoom;
extern int rotation; // 0, 90, 180, 270 degrees
extern float cameraIsoX;
extern float cameraIsoY;

void projectWorldToScreen(float worldX, float worldY, int* screenX, int* screenY);
void unprojectScreenToWorldRel(float screenRelX, float screenRelY, float* worldRelX, float* worldRelY);
void centerCameraOn(float worldX, float worldY);
void clampCamera(void);

#endif // ISOWINDOW_H
