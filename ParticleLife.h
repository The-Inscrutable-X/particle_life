#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
///opt/homebrew/Cellar/sdl2/2.28.5/include
typedef struct {
    int position_x;
    int position_y;
    int velocity_x;
    int velocity_y;
    int type;
} particle;