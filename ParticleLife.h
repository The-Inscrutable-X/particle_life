#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>

#define FALSE 0
#define TRUE 1
#define HEIGHT 1000
#define WIDTH 800
///opt/homebrew/Cellar/sdl2/2.28.5/include
typedef struct {
    float position_x;
    float position_y;
    float velocity_x;
    float velocity_y;
    float radius;
    int type;
} particle;

