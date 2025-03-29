#ifndef ENEMY_H
#define ENEMY_H

#include "Baseoject.h"
#include "comfunc.h"
#include <vector>

// Lớp Enemy kế thừa từ Baseoject, đại diện cho quái vật trong game
class Enemy : public Baseoject
{
public:
    Enemy();  // Constructor mặc định
    ~Enemy(); // Destructor để giải phóng tài nguyên

    // Tải hình ảnh cho quái từ đường dẫn file
    bool LoadImg(const char* path, SDL_Renderer* renderer);

    // Đặt vị trí ban đầu cho quái
    void SetPosition(int x, int y);

    // Cập nhật vị trí quái: di chuyển xuống dưới, trả về false nếu quái ra khỏi map
    bool Update();

    // Vẽ quái lên màn hình
    void Render(SDL_Renderer* renderer);

    // Đặt tốc độ di chuyển của quái (sử dụng float để hỗ trợ giá trị thập phân)
    void SetSpeed(float speed) { move_speed = speed; }

    // Lấy hình chữ nhật bao quanh quái để kiểm tra va chạm
    SDL_Rect GetRect() const { return enemy_rect; }

    // Kiểm tra quái còn sống hay không
    bool IsAlive() const { return is_alive; }

    // Xử lý khi quái bị trúng đạn và mất máu
    void TakeDamage(int damage);

    // Lấy giá trị máu hiện tại của quái
    int GetHealth() const { return health; }

private:
    SDL_Rect enemy_rect;     // Hình chữ nhật bao quanh quái (vị trí và kích thước)
    SDL_Texture* enemy_texture; // Texture chứa hình ảnh của quái
    float velY;              // Vận tốc di chuyển theo trục Y (chỉ di chuyển xuống, đổi sang float)
    float move_speed;        // Tốc độ di chuyển của quái (đổi sang float)
    int health;              // Lượng máu của quái
    bool is_alive;           // Trạng thái sống/chết của quái
};

#endif // ENEMY_H
