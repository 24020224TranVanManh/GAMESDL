/*#ifndef ITEM_H
#define ITEM_H

#include "Baseoject.h"
#include "comfunc.h"


enum BulletType {
    BULLET_NORMAL,
    BULLET_FIRE,
    BULLET_FAST
};

class Item : public Baseoject
{
public:
    Item();
    ~Item();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect() const { return item_rect; }
    BulletType GetBulletType() const { return bullet_type; }
    bool IsActive() const { return is_active; }
    void Deactivate() { is_active = false; }
    void SetBulletType(BulletType type) { bullet_type = type; }

private:
    SDL_Rect item_rect;
    SDL_Texture* item_texture;
    BulletType bullet_type;
    bool is_active;
};

#endif // ITEM_H
*/
#ifndef ITEM_H
#define ITEM_H

#include "Baseoject.h"
#include "comfunc.h"

enum BulletType {
    BULLET_NORMAL,
    BULLET_FIRE,
    BULLET_FAST
};

class Item : public Baseoject
{
public:
    Item();
    ~Item();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void Render(SDL_Renderer* renderer);
    void Update(); // Thêm hàm để cập nhật vị trí khi rơi
    SDL_Rect GetRect() const { return item_rect; }
    BulletType GetBulletType() const { return bullet_type; }
    bool IsActive() const { return is_active; }
    void Deactivate() { is_active = false; }
    void SetBulletType(BulletType type) { bullet_type = type; }

private:
    SDL_Rect item_rect;
    SDL_Texture* item_texture;
    BulletType bullet_type;
    bool is_active;
    float velY;  // Tốc độ rơi
    float posY;  // Vị trí thực tế theo trục Y
};

#endif // ITEM_H
