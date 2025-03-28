/*
#ifndef POWER_H_
#define POWER_H_
#include "Baseoject.h"
#include "comfunc.h"

class POW : public Baseoject
{
public:
    POW();
    ~POW();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void SetDirection(int vx, int vy);
    void Update();
    void Render(SDL_Renderer* renderer);

private:
    SDL_Rect pow_rect;
    int velX, velY;
    SDL_Texture* pow_texture; // Texture cho đạn
};

#endif // POWER_H_
*/
#ifndef POWER_H_
#define POWER_H_
#include "Baseoject.h"
#include "comfunc.h"

class POW : public Baseoject
{
public:
    POW();
    ~POW();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void SetDirection(int vx, int vy);
    void Update();
    void Render(SDL_Renderer* renderer);
    void SetSize(int w, int h); // Thêm để đặt kích thước đạn

private:
    SDL_Rect pow_rect;
    int velX, velY;
    SDL_Texture* pow_texture;
};

#endif // POWER_H_
