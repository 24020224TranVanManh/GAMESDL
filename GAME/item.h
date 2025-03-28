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
    void SetPosition(int x, int y);     // Đặt vị trí vật phẩm tại tọa độ (x, y)
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect() const { return item_rect; } // Lấy hình chữ nhật của vật phẩm để kiểm tra va chạm
    BulletType GetBulletType() const { return bullet_type; }
    bool IsActive() const { return is_active; }
    void Deactivate() { is_active = false; } // Đánh dấu vật phẩm đã bị nhặt (ẩn vật phẩm)
    void SetBulletType(BulletType type) { bullet_type = type; }

private:
    SDL_Rect item_rect;
    SDL_Texture* item_texture;
    BulletType bullet_type;
    bool is_active;
};

#endif // ITEM_H
