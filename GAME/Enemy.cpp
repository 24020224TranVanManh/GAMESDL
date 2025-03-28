#include "enemy.h"

Enemy::Enemy()
{
    enemy_rect = {0, 0, 50, 50};       // Khởi tạo vị trí (0, 0) và kích thước 50x50 pixel cho quái
    enemy_texture = NULL;               // Khởi tạo texture là NULL
    velY = 2;                           // Vận tốc di chuyển xuống dưới (2 pixel mỗi khung hình)
    is_alive = true;                    // Ban đầu quái còn sống
}

Enemy::~Enemy()
{
    if (enemy_texture != NULL) {        // Kiểm tra nếu texture tồn tại
        SDL_DestroyTexture(enemy_texture); // Giải phóng texture
        enemy_texture = NULL;           // Đặt lại về NULL
    }
}

bool Enemy::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path); // Tải ảnh từ đường dẫn
    if (!surface) {                     // Nếu tải thất bại
        std::cout << "Không thể load ảnh quái: " << IMG_GetError() << std::endl;
        return false;
    }
    enemy_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture
    SDL_SetTextureBlendMode(enemy_texture, SDL_BLENDMODE_BLEND); // Hỗ trợ độ trong suốt (alpha)
    SDL_FreeSurface(surface);           // Giải phóng surface
    return (enemy_texture != NULL);     // Trả về true nếu texture được tạo thành công
}

void Enemy::SetPosition(int x, int y)
{
    enemy_rect.x = x;                   // Đặt tọa độ x của quái
    enemy_rect.y = y;                   // Đặt tọa độ y của quái
}

void Enemy::Update()
{
    if (!is_alive) return;              // Nếu quái đã chết, không cập nhật vị trí
    enemy_rect.y += velY;               // Di chuyển quái xuống dưới (tăng tọa độ y)
    if (enemy_rect.y > HEIGHT) {        // Nếu quái ra khỏi màn hình (vượt quá cạnh dưới)
        is_alive = false;               // Đánh dấu quái đã chết
    }
}

void Enemy::Render(SDL_Renderer* renderer)
{
    if (is_alive && enemy_texture) {    // Chỉ vẽ nếu quái còn sống và texture hợp lệ
        SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect); // Vẽ quái lên renderer
    }
}
