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
*/
#ifndef ENEMY_H // Bảo vệ tránh định nghĩa lại header file
#define ENEMY_H

#include <SDL.h>       // Thư viện SDL để xử lý đồ họa và sự kiện
#include <SDL_image.h> // Thư viện SDL_image để tải hình ảnh
#include <iostream>    // Thư viện nhập/xuất chuẩn để in thông báo
#include <vector>      // Thư viện vector để lưu danh sách đạn
#include "comfunc.h"   // Header chứa các hàm tiện ích chung (giả định)
#include "power.h"     // Header của lớp POW để sử dụng đạn

class Enemy { // Lớp cơ sở Enemy đại diện cho kẻ thù cơ bản
protected:
    SDL_Rect enemy_rect; // Hình chữ nhật xác định vị trí và kích thước của Enemy
    SDL_Texture* enemy_texture; // Texture (hình ảnh) của Enemy để vẽ lên màn hình
    float velY; // Vận tốc theo trục Y (dùng float để chính xác)
    float posY; // Vị trí thực tế theo trục Y (dùng float để chính xác)
    int health; // Máu của Enemy

public:
    Enemy(); // Constructor mặc định
    virtual ~Enemy(); // Destructor ảo để hỗ trợ đa hình khi kế thừa
    virtual bool Update(); // Cập nhật vị trí của Enemy, trả về false nếu ra khỏi màn hình
    virtual void Render(SDL_Renderer* renderer); // Vẽ Enemy lên màn hình
    bool LoadImg(const char* path, SDL_Renderer* renderer); // Tải hình ảnh cho Enemy
    virtual void SetPosition(int x, int y); // Đặt vị trí cho Enemy
    SDL_Rect GetRect() { return enemy_rect; } // Trả về hình chữ nhật của Enemy để kiểm tra va chạm
    void TakeDamage(int damage); // Giảm máu khi Enemy bị tấn công
    bool IsAlive() { return health > 0; } // Kiểm tra xem Enemy còn sống không (máu > 0)
};

class AdvancedEnemy : public Enemy { // Lớp AdvancedEnemy kế thừa từ Enemy, có khả năng bắn đạn
private:
    float velX; // Vận tốc theo trục X (di chuyển ngang)
    float posX; // Vị trí thực tế theo trục X (dùng float để chính xác)
    Uint32 last_shot_time; // Thời gian bắn đạn cuối cùng (ms)
    std::vector<POW*> enemy_pows; // Danh sách các con trỏ tới viên đạn của AdvancedEnemy

public:
    AdvancedEnemy(); // Constructor mặc định
    ~AdvancedEnemy(); // Destructor để giải phóng tài nguyên
    bool Update() override; // Cập nhật vị trí và trạng thái (di chuyển ngang, bắn đạn)
    void Render(SDL_Renderer* renderer) override; // Vẽ AdvancedEnemy và các viên đạn của nó
    void Shoot(SDL_Renderer* renderer); // Bắn đạn xuống dưới
    void SetPosition(int x, int y) override; // Đặt vị trí cho AdvancedEnemy
    std::vector<POW*>& GetPows() { return enemy_pows; } // Trả về tham chiếu tới danh sách đạn
    float GetVelX() const { return velX; } // Trả về vận tốc X hiện tại
    void SetVelX(float newVelX) { velX = newVelX; } // Đặt vận tốc X mới (dùng để đổi hướng)
};

class Boss : public Enemy { // Lớp Boss kế thừa từ Enemy, có khả năng bắn đạn nhiều hướng
private:
    float velX; // Vận tốc theo trục X (di chuyển ngang)
    float posX; // Vị trí thực tế theo trục X (dùng float để chính xác)
    Uint32 last_shot_time; // Thời gian bắn đạn cuối cùng (ms)
    std::vector<POW*> boss_pows; // Danh sách các con trỏ tới viên đạn của Boss

public:
    Boss(); // Constructor mặc định
    ~Boss(); // Destructor để giải phóng tài nguyên
    bool Update() override; // Cập nhật vị trí và trạng thái (di chuyển ngang, bắn đạn)
    void Render(SDL_Renderer* renderer) override; // Vẽ Boss và các viên đạn của nó
    void Shoot(SDL_Renderer* renderer); // Bắn đạn về nhiều hướng
    void SetPosition(int x, int y) override; // Đặt vị trí cho Boss
    std::vector<POW*>& GetPows() { return boss_pows; } // Trả về tham chiếu tới danh sách đạn
};

#endif // ENEMY_H // Kết thúc bảo vệ header
