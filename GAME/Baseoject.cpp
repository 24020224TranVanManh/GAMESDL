#include"Baseoject.h"
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
bool Baseoject::LoadImg(char* path,SDL_Renderer* Color)
{
    free();
    SDL_Texture* new_text=NULL;
    SDL_Surface* load_surface=IMG_Load(path);
    if(load_surface!=NULL)
    {
        //SDL_SetColorKey(load_surface,SDL_TRUE,Color_R,SDL_TRUE,Color_G,SDL_TRUE,Color_B);
        new_text=SDL_CreateTextureFromSurface(Color,load_surface);
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
}
