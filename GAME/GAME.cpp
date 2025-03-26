#include<iostream>
#include<SDL.h>
#include"Baseoject.h"
#include"comfunc.h"
#include"map_game.hpp"
#include"avartar.hpp"
Baseoject gBackroud;
bool InitData ()
{
    bool success=true;
    int ret=SDL_Init(SDL_INIT_VIDEO);
    if(ret<0)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    gWindow=SDL_CreateWindow("GAME",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
    if(gWindow==NULL)
    {
        success=false;
    }
    else
    {
        gRender=SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
        if(gRender==NULL) success=false;
        else
            {
                SDL_SetRenderDrawColor(gRender,255,255,255,255);
            }
        int imgl=IMG_INIT_PNG;
        if(!(IMG_Init(imgl)&& imgl)) success=false;
    }
        return success;
}
bool loadbackgroud ()
{
    return gBackroud.LoadImg("anh/anh.jpg", gRender);
}
void close()
{
    gBackroud.free();
    SDL_DestroyRenderer(gRender);
    gRender=NULL;
    SDL_DestroyWindow(gWindow);
    gWindow=NULL;
    IMG_Quit();
    SDL_Quit();
}
int main(int argc, char* argv[])
{
    if (!InitData())
    {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadbackgroud())
    {
        std::cout << "Failed to load background!" << std::endl;
        return -1;
    }
    GameMap game_MAP;
    game_MAP.LoadMAP("anh/0123.txt");
    game_MAP.LoadTiles(gRender);
    AVARTAR P_player;
    P_player.LoadImg("nhanvat/right.png",gRender);
    P_player.set_clips();

    bool is_quit=false;
    while(!is_quit)
    {
        while(SDL_PollEvent(&gEvent)!=0)
        {
            if(gEvent.type==SDL_QUIT)
            {
                is_quit=true;
            }
            P_player.EVENT(gEvent,gRender);
        }
        SDL_SetRenderDrawColor(gRender,255,255,255,255);
        SDL_RenderClear(gRender);
        gBackroud.render(gRender,NULL);
        game_MAP.DrawMap(gRender);
        P_player.show(gRender);
        SDL_RenderPresent(gRender);
    }
    close();
}
