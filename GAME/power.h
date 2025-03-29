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
    void SetSize(int w, int h); // Thêm để đặt kích thước đạn

private:
    SDL_Rect pow_rect;
    int velX, velY;
    SDL_Texture* pow_texture;
};

#endif // POWER_H_
*/
#ifndef POWER_H
#define POWER_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "comfunc.h"

class POW {
private:
    SDL_Rect pow_rect;
    SDL_Texture* pow_texture;
    int velX, velY;
    int damage;

public:
    POW();
    ~POW();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void SetSize(int w, int h);
    void SetPosition(int x, int y);
    void SetDirection(int x, int y);
    void SetDamage(int d); // Chỉ khai báo, không định nghĩa inline
    void Update();
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect() { return pow_rect; }
    int GetDamage() { return damage; }
};

#endif // POWER_H
