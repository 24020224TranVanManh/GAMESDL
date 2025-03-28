/*#include"Baseoject.h"
#include"comfunc.h"
#include"map_game.hpp"
#include"power.h"
#include<vector>
#ifndef AVARTAR_h
#define AVARTAR_H

class AVARTAR
{
public:
    AVARTAR();
    ~AVARTAR();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void Draw (SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void UpdatePosition();
     void HandleEvent(SDL_Event& e,SDL_Renderer* renderer);
     void set_bullet_list(std::vector<POW*> bullet_list)
     {
         pows=bullet_list;
     }
     void UpdatePows();
private:
    std::vector<POW*> pows;
    SDL_Texture* player_texture;
    SDL_Rect player_rect;
    double velX,velY;

};

#endif // AVARTAR_h
*/
#ifndef AVARTAR_H
#define AVARTAR_H

#include "Baseoject.h"
#include "comfunc.h"
#include "power.h"
#include <vector>

class AVARTAR
{
public:
    AVARTAR();
    ~AVARTAR();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void Draw(SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void UpdatePosition();
    void HandleEvent(SDL_Event& e, SDL_Renderer* renderer);
    void set_bullet_list(std::vector<POW*> bullet_list)
    {
        pows = bullet_list;
    }
    void UpdatePows();
    std::vector<POW*> get_pows() const { return pows; } // Thêm hàm để truy cập pows

private:
    std::vector<POW*> pows;
    SDL_Texture* player_texture;
    SDL_Rect player_rect;
    double velX, velY;
};

#endif // AVARTAR_H

