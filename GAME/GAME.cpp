
#include <iostream>
#include <SDL.h>
#include "Baseoject.h"
#include "comfunc.h"
#include "avartar.hpp"

Baseoject gBackground;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        success = false;
    } else {
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gRender == NULL) success = false;
        else {
            SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        }
        int imgl = IMG_INIT_PNG;
        if (!(IMG_Init(imgl) & imgl)) success = false;
    }
    return success;
}

bool loadBackground()
{
    return gBackground.LoadImg("anh/anh.jpg", gRender);
}

void close()
{
    gBackground.free();
    SDL_DestroyRenderer(gRender);
    SDL_DestroyWindow(gWindow);
    gRender = NULL;
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (!InitData()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadBackground()) {
        std::cout << "Failed to load background!" << std::endl;
        return -1;
    }

    AVARTAR P_player;
    if (!P_player.LoadImg("nhanvat/left.png", gRender)) {
        std::cout << "Failed to load player image!" << std::endl;
        return -1;
    }
    P_player.SetPosition(500, 500);

    bool is_quit = false;
    while (!is_quit) {
        while (SDL_PollEvent(&gEvent) != 0) {
            if (gEvent.type == SDL_QUIT) {
                is_quit = true;
            }
            P_player.HandleEvent(gEvent, gRender);
        }

        P_player.UpdatePosition(); // Cập nhật vị trí nhân vật
        P_player.UpdatePows();     // Cập nhật vị trí đạn

        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        SDL_RenderClear(gRender);
        gBackground.render(gRender, NULL); // Vẽ nền

        // Vẽ tất cả đạn
        for (auto pow : P_player.get_pows()) {
            pow->Render(gRender);
        }

        P_player.Draw(gRender); // Vẽ nhân vật
        SDL_RenderPresent(gRender);
    }

    close();
    return 0;
}
