/*
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "comfunc.h"
#include "power.h"

class Enemy {
protected:
    SDL_Rect enemy_rect;
    SDL_Texture* enemy_texture;
    float velY;
    float posY;
    int health;

public:
    Enemy();
    virtual ~Enemy();
    virtual bool Update();
    virtual void Render(SDL_Renderer* renderer);
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    virtual void SetPosition(int x, int y);
    SDL_Rect GetRect() { return enemy_rect; }
    void TakeDamage(int damage);
    bool IsAlive() { return health > 0; }
};

class AdvancedEnemy : public Enemy {
private:
    float velX;
    float posX;
    Uint32 last_shot_time;
    std::vector<POW*> enemy_pows;

public:
    AdvancedEnemy();
    ~AdvancedEnemy();
    bool Update() override;
    void Render(SDL_Renderer* renderer) override;
    void Shoot(SDL_Renderer* renderer);
    void SetPosition(int x, int y) override;
    std::vector<POW*>& GetPows() { return enemy_pows; }
    float GetVelX() const { return velX; }       // Thêm getter
    void SetVelX(float newVelX) { velX = newVelX; } // Thêm setter
};

#endif // ENEMY_H
*/
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "comfunc.h"
#include "power.h"

class Enemy {
protected:
    SDL_Rect enemy_rect;
    SDL_Texture* enemy_texture;
    float velY;
    float posY;
    int health;

public:
    Enemy();
    virtual ~Enemy();
    virtual bool Update();
    virtual void Render(SDL_Renderer* renderer);
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    virtual void SetPosition(int x, int y);
    SDL_Rect GetRect() { return enemy_rect; }
    void TakeDamage(int damage);
    bool IsAlive() { return health > 0; }
};

class AdvancedEnemy : public Enemy {
private:
    float velX;
    float posX;
    Uint32 last_shot_time;
    std::vector<POW*> enemy_pows;

public:
    AdvancedEnemy();
    ~AdvancedEnemy();
    bool Update() override;
    void Render(SDL_Renderer* renderer) override;
    void Shoot(SDL_Renderer* renderer);
    void SetPosition(int x, int y) override;
    std::vector<POW*>& GetPows() { return enemy_pows; }
    float GetVelX() const { return velX; }
    void SetVelX(float newVelX) { velX = newVelX; }
};

class Boss : public Enemy {
private:
    float velX;  // Vận tốc ngang
    float posX;  // Vị trí thực tế X
    Uint32 last_shot_time;
    std::vector<POW*> boss_pows;

public:
    Boss();
    ~Boss();
    bool Update() override;
    void Render(SDL_Renderer* renderer) override;
    void Shoot(SDL_Renderer* renderer); // Bắn về nhiều phía
    void SetPosition(int x, int y) override;
    std::vector<POW*>& GetPows() { return boss_pows; }
};

#endif // ENEMY_H
