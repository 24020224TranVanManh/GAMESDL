/*
#include "comfunc.h"
#ifndef BASEOJECT_H
#define BASEOJECT_H

class Baseoject
{
public:
    Baseoject();
    virtual ~Baseoject();
    void SetRect(const int& x, const int& y)
    {
        rect_.x = x;
        rect_.y = y;
    }
    SDL_Rect GetRect()
    {
        return rect_;
    }
    SDL_Texture* GetObject()
    {
        return p_objcet;
    }
    virtual bool LoadImg(const char* path, SDL_Renderer* renderer);
    void render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void free();

protected:
    SDL_Texture* p_objcet;
    SDL_Rect rect_;
};

#endif
*/
#ifndef BASEOJECT_H
#define BASEOJECT_H

#include <SDL.h>
#include <SDL_image.h>

class Baseoject {
public:
    Baseoject();
    ~Baseoject();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, const SDL_Rect* clip);
    void free();

    // Thêm getter để lấy texture
    SDL_Texture* getTexture() const { return texture; }

private:
    SDL_Texture* texture;
    int width;
    int height;
};

#endif // BASEOJECT_H
