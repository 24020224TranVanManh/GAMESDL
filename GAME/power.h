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
    int damage;
    float velX;  // Vận tốc theo trục X
    float velY;  // Vận tốc theo trục Y
    float posX;  // Vị trí thực tế theo trục X
    float posY;  // Vị trí thực tế theo trục Y

public:
    POW();
    ~POW();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void SetSize(int w, int h);
    void SetDamage(int dmg);
    void SetDirection(float vx, float vy); // Đặt vận tốc
    void Update(); // Cập nhật vị trí
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect() const { return pow_rect; }
    int GetDamage() const { return damage; }
};

#endif // POWER_H
