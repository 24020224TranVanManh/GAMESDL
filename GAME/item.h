
#ifndef ITEM_H // Bảo vệ tránh định nghĩa lại header file
#define ITEM_H

#include "Baseoject.h" // Include header của lớp Baseoject để kế thừa
#include "comfunc.h"   // Header chứa các hàm tiện ích chung (giả định)

enum BulletType { // Kiểu liệt kê các loại đạn mà vật phẩm có thể cung cấp
    BULLET_NORMAL, // Đạn thường
    BULLET_FIRE,   // Đạn lửa
    BULLET_FAST    // Đạn nhanh
};

class Item : public Baseoject { // Lớp Item kế thừa từ Baseoject, đại diện cho vật phẩm rơi trong game
public:
    Item(); // Constructor mặc định để khởi tạo vật phẩm
    ~Item(); // Destructor để giải phóng tài nguyên
    bool LoadImg(const char* path, SDL_Renderer* renderer); // Tải hình ảnh cho vật phẩm từ file
    void SetPosition(int x, int y); // Đặt vị trí cho vật phẩm
    void Render(SDL_Renderer* renderer); // Vẽ vật phẩm lên màn hình
    void Update(); // Cập nhật vị trí của vật phẩm khi rơi
    SDL_Rect GetRect() const { return item_rect; } // Trả về hình chữ nhật của vật phẩm để kiểm tra va chạm
    BulletType GetBulletType() const { return bullet_type; } // Trả về loại đạn mà vật phẩm cung cấp
    bool IsActive() const { return is_active; } // Kiểm tra xem vật phẩm còn hoạt động không
    void Deactivate() { is_active = false; } // Vô hiệu hóa vật phẩm (biến mất)
    void SetBulletType(BulletType type) { bullet_type = type; } // Đặt loại đạn cho vật phẩm

private:
    SDL_Rect item_rect; // Hình chữ nhật xác định vị trí và kích thước của vật phẩm
    SDL_Texture* item_texture; // Texture (hình ảnh) của vật phẩm để vẽ lên màn hình
    BulletType bullet_type; // Loại đạn mà vật phẩm cung cấp cho người chơi
    bool is_active; // Trạng thái hoạt động của vật phẩm (true: đang tồn tại, false: đã biến mất)
    float velY; // Tốc độ rơi theo trục Y (dùng float để chính xác)
    float posY; // Vị trí thực tế theo trục Y (dùng float để chính xác)
};

#endif // ITEM_H // Kết thúc bảo vệ header
