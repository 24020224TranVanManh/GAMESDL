#include "Enemy.h"
#include <iostream>
#include <cstdlib> // Thư viện cho hàm rand()

// Constructor mặc định: Khởi tạo các giá trị ban đầu cho quái
Enemy::Enemy()
{
    enemy_rect = {0, 0, 150, 150}; // Tăng kích thước ô vuông của quái (từ 100x100 lên 150x150)
    velY = 0;                      // Vận tốc ban đầu theo Y
    move_speed = 0.1f;             // Tốc độ di chuyển mặc định (đặt thành 0.1)
    enemy_texture = NULL;          // Texture ban đầu là NULL
    health = 3;                    // Máu mặc định là 3
    is_alive = true;               // Quái bắt đầu ở trạng thái sống
}

// Destructor: Giải phóng texture của quái khi đối tượng bị hủy
Enemy::~Enemy()
{
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture); // Giải phóng texture
        enemy_texture = NULL;              // Đặt lại về NULL để tránh lỗi
    }
}

// Tải hình ảnh cho quái từ file
bool Enemy::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path); // Tải ảnh từ đường dẫn
    if (!surface) {
        std::cout << "Không thể load ảnh quái: " << IMG_GetError() << std::endl;
        return false; // Trả về false nếu tải thất bại
    }
    enemy_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture
    SDL_SetTextureBlendMode(enemy_texture, SDL_BLENDMODE_BLEND);     // Hỗ trợ độ trong suốt
    SDL_FreeSurface(surface);                                        // Giải phóng surface
    return (enemy_texture != NULL);                                  // Trả về true nếu thành công
}

// Đặt vị trí của quái trên màn hình
void Enemy::SetPosition(int x, int y)
{
    enemy_rect.x = x; // Tọa độ X
    enemy_rect.y = y; // Tọa độ Y
}

// Cập nhật vị trí quái: Di chuyển xuống dưới, trả về false nếu quái ra khỏi map
bool Enemy::Update()
{
    if (!is_alive) return false; // Nếu quái đã chết, trả về false để xóa quái

    velY = move_speed;     // Quái di chuyển xuống với tốc độ move_speed (0.1)
    enemy_rect.y += velY;  // Cập nhật vị trí Y (dùng float, nhưng SDL_Rect chỉ nhận int, sẽ tự động làm tròn)

    // Nếu quái ra khỏi cạnh dưới màn hình, trả về false để xóa quái
    if (enemy_rect.y > HEIGHT) {
        return false; // Quái ra khỏi map, sẽ bị xóa
    }

    return true; // Quái vẫn còn trong map
}

// Vẽ quái lên màn hình
void Enemy::Render(SDL_Renderer* renderer)
{
    if (is_alive && enemy_texture) { // Chỉ vẽ nếu quái còn sống và texture tồn tại
        SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect); // Vẽ texture lên renderer
    }
}

// Xử lý khi quái bị trúng đạn
void Enemy::TakeDamage(int damage)
{
    if (!is_alive) return;   // Nếu đã chết, không xử lý
    health -= damage;        // Giảm máu theo sát thương nhận được
    std::cout << "Enemy hit! Health: " << health << std::endl; // In thông báo để kiểm tra
    if (health <= 0) {       // Nếu máu về 0 hoặc âm
        is_alive = false;    // Đánh dấu quái đã chết
        std::cout << "Enemy killed!" << std::endl; // In thông báo khi quái chết
    }
}
