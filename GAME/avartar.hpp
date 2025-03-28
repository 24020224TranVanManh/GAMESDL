
#include "Baseoject.h"
#include "comfunc.h"
#include "power.h"
#include "item.h"
#include <vector>

#ifndef AVARTAR_H
#define AVARTAR_H
#define BULLET_SPEED 1
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
    SDL_Rect GetRect() const { return player_rect; } // Lấy hình chữ nhật của nhân vật để kiểm tra va chạm
    void CheckCollisionWithItems(std::vector<Item*>& items); // Kiểm tra va chạm giữa nhân vật và vật phẩm
    BulletType GetCurrentBulletType() const { return current_bullet_type; } // Lấy loại đạn hiện tại


private:

    Uint32 last_shot_time;
    static const Uint32 BULLET_NORMAL_DELAY = 500;
    static const Uint32 BULLET_FIRE_DELAY = 700;
    static const Uint32 BULLET_FAST_DELAY = 300;
    std::vector<POW*> pows;
    std::vector<Item*> items;
    SDL_Texture* player_texture;
    SDL_Rect player_rect;
    double velX, velY;
    BulletType current_bullet_type;     // Loại đạn hiện tại của nhân vật
};
#endif // AVARTAR_H
