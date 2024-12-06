#ifndef MAIN_H
#define MAIN_H

#include "player.h"
#include "boss.h"
#include "camera.h"

// Global variables
extern int windowWidth;
extern int windowHeight;
extern Player player;
extern Cam camera;

// Function declarations
void drawPlane(float size, int divisions); // Draws a ground plane

#endif // MAIN_H
