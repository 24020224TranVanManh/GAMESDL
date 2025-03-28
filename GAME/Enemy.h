#ifndef ENEMY_H
#define ENEMY_H

#include "Baseoject.h"   // Lớp cơ sở cho các đối tượng đồ họa
#include "comfunc.h"     // Các định nghĩa chung (SDL, hằng số, v.v.)

// Định nghĩa lớp Enemy: Đại diện cho quái trong game
class Enemy : public Baseoject
{
public:
    Enemy();                            // Constructor: Khởi tạo quái
    ~Enemy();                           // Destructor: Giải phóng tài nguyên
    bool LoadImg(const char* path, SDL_Renderer* renderer); // Tải hình ảnh cho quái
    void SetPosition(int x, int y);     // Đặt vị trí của quái
    void Update();                      // Cập nhật vị trí của quái (di chuyển)
    void Render(SDL_Renderer* renderer); // Vẽ quái lên màn hình
    SDL_Rect GetRect() const { return enemy_rect; } // Lấy hình chữ nhật của quái để kiểm tra va chạm
    bool IsAlive() const { return is_alive; } // Kiểm tra xem quái còn sống không
    void SetAlive(bool alive) { is_alive = alive; } // Đặt trạng thái sống/chết của quái

private:
    SDL_Rect enemy_rect;                // Hình chữ nhật xác định vị trí và kích thước của quái
    SDL_Texture* enemy_texture;         // Texture (hình ảnh) của quái
    int velY;                           // Vận tốc di chuyển theo trục y (quái di chuyển xuống dưới)
    bool is_alive;                      // Trạng thái của quái: true = còn sống, false = đã chết
};

#endif // ENEMY_H
