#ifndef ISOWINDOW_H
#define ISOWINDOW_H

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640
#define GRID_SIZE 15
#define BASE_HALF_TILE_WIDTH 32
#define BASE_HALF_TILE_HEIGHT 16

extern int offsetX;
extern int offsetY;
extern float zoom;
extern int rotation; // 0, 90, 180, 270 degrees

void initIsoOffsets(void);
void updateIsoOffsets(void);
void getIsoScreenPos(int isoX, int isoY, int* screenX, int* screenY);
void getRotatedIsoPos(int isoX, int isoY, int* rotX, int* rotY);

#endif // ISOWINDOW_H
