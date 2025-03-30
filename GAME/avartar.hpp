/*
#include "Baseoject.h"
#include "comfunc.h"
#include "power.h"
#include "item.h"
#include "Enemy.h"
#include <vector>

#ifndef AVARTAR_H
#define AVARTAR_H
#define BULLET_SPEED 3

// Lớp AVARTAR đại diện cho nhân vật người chơi
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
    std::vector<POW*> get_pows() const { return pows; } // Lấy danh sách đạn
    SDL_Rect GetRect() const { return player_rect; } // Lấy hình chữ nhật của nhân vật để kiểm tra va chạm
    void CheckCollisionWithItems(std::vector<Item*>& items); // Kiểm tra va chạm giữa nhân vật và vật phẩm
    BulletType GetCurrentBulletType() const { return current_bullet_type; } // Lấy loại đạn hiện tại
    void FireBullet(SDL_Renderer* renderer); // Phương thức bắn đạn

    // Thêm các phương thức để quản lý máu
    int GetHealth() const { return health; } // Lấy giá trị máu hiện tại
    void TakeDamage(int damage); // Giảm máu khi bị va chạm
    bool IsAlive() const { return health > 0; } // Kiểm tra nhân vật còn sống hay không

private:
    Uint32 last_shot_time;
    static const Uint32 BULLET_NORMAL_DELAY = 10;
    static const Uint32 BULLET_FIRE_DELAY = 10;
    static const Uint32 BULLET_FAST_DELAY = 10;
    std::vector<POW*> pows;
    std::vector<Item*> items;
    SDL_Texture* player_texture;
    SDL_Rect player_rect;
    double velX, velY;
    BulletType current_bullet_type;     // Loại đạn hiện tại của nhân vật
    int health;                         // Máu của nhân vật
};
#endif // AVARTAR_H
*/
#ifndef AVARTAR_H
#define AVARTAR_H
#define BULLET_SPEED 3

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "comfunc.h"
#include "power.h"
#include "item.h"

class AVARTAR
{
public:
    AVARTAR(int player_id = 1); // Thêm tham số player_id, mặc định là 1
    ~AVARTAR();
    bool LoadImg(const char* path, SDL_Renderer* renderer);
    void Draw(SDL_Renderer* renderer);
    void SetPosition(int x, int y);
    void UpdatePosition();
    void HandleEvent(SDL_Event& e, SDL_Renderer* renderer);
    void set_bullet_list(std::vector<POW*> bullet_list) { pows = bullet_list; }
    void UpdatePows();
    std::vector<POW*> get_pows() const { return pows; }
    SDL_Rect GetRect() const { return player_rect; }
    void CheckCollisionWithItems(std::vector<Item*>& items);
    BulletType GetCurrentBulletType() const { return current_bullet_type; }
    int GetHealth() const { return health; }
    void TakeDamage(int damage);
    bool IsAlive() const { return health > 0; }

private:
    int player_id; // Biến để xác định người chơi (1 hoặc 2)
    Uint32 last_shot_time;
    static const Uint32 BULLET_NORMAL_DELAY = 500; // Điều chỉnh delay nếu cần
    static const Uint32 BULLET_FIRE_DELAY = 500;
    static const Uint32 BULLET_FAST_DELAY = 500;
    std::vector<POW*> pows;
    SDL_Texture* player_texture;
    SDL_Rect player_rect;
    double velX, velY;
    BulletType current_bullet_type;
    int health;
};

#endif // AVARTAR_H
