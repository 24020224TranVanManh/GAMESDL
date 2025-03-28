#ifndef COMFUNC_H
#define COMFUNC_H

#include <iostream>
#include <SDL.h>
#include <string>
#include <SDL_image.h>

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRender = NULL;
static  SDL_Event gEvent;

const int WIDTH = 736;
const int HEIGHT = 900;
const int BBF = 32;
const int Color_R = 167;
const int Color_G = 175;
const int Color_B = 180;
#define  TILE_SIZE 32
#define  Map_X 400
#define  Map_Y 10

typedef struct Map
{
    int start_x;
    int start_y;
    int max_x;
    int max_y;
    int tile[Map_X][Map_Y];
    char* file_name;
};
typedef struct INPUT
{
    int left;
    int right;
    int up;
    int down;
    int jump;
};

#endif
