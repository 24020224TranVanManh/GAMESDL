#ifndef COMFUNC_H
#define COMFUNC_H

#include <iostream>
#include <SDL.h>
#include <string>
#include <SDL_image.h>

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRender = NULL;
static  SDL_Event gEvent;

const int WIDTH = 1280;
const int HEIGHT = 640;
const int BBF = 32;
const int Color_R = 167;
const int Color_G = 175;
const int Color_B = 180;
#define  TILE_SIZE 64
#define  Map_X 410
#define  Map_Y 20
typedef struct Map
{
    int start_x;
    int start_y;
    int max_x;
    int max_y;
    int tile[Map_X][Map_Y];
    char* file_name;
};
#endif
