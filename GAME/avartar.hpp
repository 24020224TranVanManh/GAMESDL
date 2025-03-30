
#ifndef AVARTAR_H // Bảo vệ tránh định nghĩa lại header file
#define AVARTAR_H
#define BULLET_SPEED 3 // Định nghĩa tốc độ mặc định của đạn (hằng số)

#include <SDL.h>       // Thư viện SDL để xử lý đồ họa và sự kiện
#include <SDL_image.h> // Thư viện SDL_image để tải hình ảnh
#include <vector>      // Thư viện vector để lưu danh sách đạn
#include "comfunc.h"   // Header chứa các hàm tiện ích chung (giả định)
#include "power.h"     // Header của lớp POW để sử dụng đạn
#include "item.h"      // Header của lớp Item để xử lý vật phẩm

class AVARTAR { // Lớp AVARTAR đại diện cho nhân vật người chơi
public:
    AVARTAR(int player_id = 1); // Constructor với tham số player_id, mặc định là 1 (Player 1)
    ~AVARTAR(); // Destructor để giải phóng tài nguyên
    bool LoadImg(const char* path, SDL_Renderer* renderer); // Tải hình ảnh cho nhân vật từ file
    void Draw(SDL_Renderer* renderer); // Vẽ nhân vật lên màn hình
    void SetPosition(int x, int y); // Đặt vị trí cho nhân vật
    void UpdatePosition(); // Cập nhật vị trí của nhân vật dựa trên vận tốc
    void HandleEvent(SDL_Event& e, SDL_Renderer* renderer); // Xử lý sự kiện bàn phím (di chuyển, bắn)
    void set_bullet_list(std::vector<POW*> bullet_list) { pows = bullet_list; } // Đặt danh sách đạn cho nhân vật
    void UpdatePows(); // Cập nhật vị trí của các viên đạn
    std::vector<POW*> get_pows() const { return pows; } // Trả về danh sách đạn của nhân vật
    SDL_Rect GetRect() const { return player_rect; } // Trả về hình chữ nhật của nhân vật để kiểm tra va chạm
    void CheckCollisionWithItems(std::vector<Item*>& items); // Kiểm tra va chạm giữa nhân vật và vật phẩm
    BulletType GetCurrentBulletType() const { return current_bullet_type; } // Trả về loại đạn hiện tại của nhân vật
    int GetHealth() const { return health; } // Trả về giá trị máu hiện tại của nhân vật
    void TakeDamage(int damage); // Giảm máu khi nhân vật bị tấn công
    bool IsAlive() const { return health > 0; } // Kiểm tra xem nhân vật còn sống không (máu > 0)

private:
    int player_id; // Biến lưu ID của người chơi (1 hoặc 2) để phân biệt Player 1 và Player 2
    Uint32 last_shot_time; // Thời gian bắn đạn cuối cùng (ms), dùng để kiểm soát tốc độ bắn
    static const Uint32 BULLET_NORMAL_DELAY = 300; // Thời gian chờ giữa các lần bắn đạn thường (400ms)
    static const Uint32 BULLET_FIRE_DELAY = 500;   // Thời gian chờ giữa các lần bắn đạn lửa (600ms)
    static const Uint32 BULLET_FAST_DELAY = 200;   // Thời gian chờ giữa các lần bắn đạn nhanh (300ms)
    std::vector<POW*> pows; // Danh sách các con trỏ tới viên đạn của nhân vật
    SDL_Texture* player_texture; // Texture (hình ảnh) của nhân vật để vẽ lên màn hình
    SDL_Rect player_rect; // Hình chữ nhật xác định vị trí và kích thước của nhân vật
    double velX, velY; // Vận tốc của nhân vật theo trục X và Y (dùng double để chính xác)
    BulletType current_bullet_type; // Loại đạn hiện tại của nhân vật (NORMAL, FIRE, FAST)
    int health; // Máu của nhân vật
};

#endif // AVARTAR_H // Kết thúc bảo vệ header
