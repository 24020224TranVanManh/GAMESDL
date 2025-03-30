
#ifndef POWER_H // Bảo vệ tránh định nghĩa lại header file
#define POWER_H

#include <SDL.h>       // Thư viện SDL để xử lý đồ họa và sự kiện
#include <SDL_image.h> // Thư viện SDL_image để tải hình ảnh (PNG, JPG, v.v.)
#include <iostream>    // Thư viện nhập/xuất chuẩn để in thông báo lỗi
#include "comfunc.h"   // File header chứa các hàm tiện ích chung (giả định)

class POW { // Lớp POW đại diện cho các viên đạn trong game
private:
    SDL_Rect pow_rect;      // Hình chữ nhật xác định vị trí (x, y) và kích thước (w, h) của đạn
    SDL_Texture* pow_texture; // Texture (hình ảnh) của đạn để vẽ lên màn hình
    int damage;             // Sát thương mà đạn gây ra khi va chạm với mục tiêu
    float velX;             // Vận tốc theo trục X (ngang), dùng float để chuyển động mượt mà hơn
    float velY;             // Vận tốc theo trục Y (dọc), dùng float để chính xác hơn int
    float posX;             // Vị trí thực tế theo trục X (float để tính toán chính xác hơn)
    float posY;             // Vị trí thực tế theo trục Y (float để tính toán chính xác hơn)

public:
    POW();                  // Constructor mặc định để khởi tạo một viên đạn mới
    ~POW();                 // Destructor để giải phóng tài nguyên (texture) khi đạn bị hủy
    bool LoadImg(const char* path, SDL_Renderer* renderer); // Tải hình ảnh cho đạn từ file
    void SetPosition(int x, int y); // Đặt vị trí ban đầu của đạn trên màn hình
    void SetSize(int w, int h);     // Đặt kích thước (chiều rộng và chiều cao) của đạn
    void SetDamage(int dmg);        // Đặt giá trị sát thương của đạn
    void SetDirection(float vx, float vy); // Đặt hướng di chuyển của đạn thông qua vận tốc
    void Update();                  // Cập nhật vị trí của đạn mỗi frame dựa trên vận tốc
    void Render(SDL_Renderer* renderer); // Vẽ đạn lên màn hình bằng renderer của SDL
    SDL_Rect GetRect() const { return pow_rect; } // Trả về hình chữ nhật của đạn để kiểm tra va chạm
    int GetDamage() const { return damage; } // Trả về giá trị sát thương của đạn
};

#endif // POWER_H // Kết thúc bảo vệ header
