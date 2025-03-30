/*#include"Baseoject.h"
#include"comfunc.h"

Baseoject::Baseoject()
{
    p_objcet=NULL;
    rect_.x=0;
    rect_.y=0;
    rect_.w=0;
    rect_.h=0;
}

Baseoject::~Baseoject()
{
    free();
}
 bool Baseoject::LoadImg(const char* path,SDL_Renderer* renderer)
{
    free();
    SDL_Texture* new_text=NULL;
    SDL_Surface* load_surface=IMG_Load(path);
    if(load_surface!=NULL)
    {

        Uint32 color_key = SDL_MapRGB(load_surface->format, Color_R, Color_G, Color_B);
        SDL_SetColorKey(load_surface, SDL_TRUE, color_key);
        // SDL_SetColorKey()

        // SDL_CreateTextureFromSurface(Color, load_surface);
        new_text = SDL_CreateTextureFromSurface(renderer, load_surface);
        if(new_text!=NULL)
        {
            rect_.w=load_surface->w;
            rect_.h=load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    p_objcet=new_text;
    return p_objcet!=NULL;
}
void Baseoject::render (SDL_Renderer* des,const SDL_Rect* Clip)
{
    SDL_Rect renderquad = {rect_.x,rect_.y,rect_.w,rect_.h};
    SDL_RenderCopy(des,p_objcet,Clip,&renderquad);

}

void Baseoject::free()
{
    if(p_objcet!=NULL)
    {
        SDL_DestroyTexture(p_objcet);
        p_objcet=NULL;
        rect_.w=0;
        rect_.h=0;
    }
    rect_ = {0, 0, 0, 0};
}*/
#include "Baseoject.h"
#include <iostream>

Baseoject::Baseoject() {
    texture = NULL;
    width = 0;
    height = 0;
}

Baseoject::~Baseoject() {
    free();
}

bool Baseoject::LoadImg(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Unable to load image: " << IMG_GetError() << std::endl;
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        width = surface->w;
        height = surface->h;
    }
    SDL_FreeSurface(surface);
    return texture != NULL;
}

void Baseoject::render(SDL_Renderer* renderer, const SDL_Rect* clip) {
    SDL_Rect renderQuad = {0, 0, width, height};
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(renderer, texture, clip, &renderQuad);
}

void Baseoject::free() {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}
