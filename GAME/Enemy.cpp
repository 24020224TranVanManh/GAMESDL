
#include "Enemy.h"
#include <cmath>

// Constructor cho Enemy
Enemy::Enemy()
{
    enemy_rect = {0, 0, 50, 50};
    posY = 0.0f;
    velY = 0.5f;
    enemy_texture = NULL;
    health = 3;
}

Enemy::~Enemy()
{
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
}

bool Enemy::Update()
{
    posY += velY;
    enemy_rect.y = static_cast<int>(posY);
    if (enemy_rect.y > HEIGHT) {
        return false;
    }
    return true;
}

void Enemy::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);
}

bool Enemy::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Không thể load ảnh quái: " << IMG_GetError() << std::endl;
        return false;
    }
    enemy_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(enemy_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return (enemy_texture != NULL);
}

void Enemy::SetPosition(int x, int y)
{
    enemy_rect.x = x;
    posY = static_cast<float>(y);
    enemy_rect.y = y;
}

void Enemy::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
    std::cout << "Enemy health: " << health << std::endl;
}

// AdvancedEnemy
AdvancedEnemy::AdvancedEnemy()
{
    enemy_rect = {0, 0, 50, 50};
    velY = 0;
    posX = 0.0f;
    velX = 0.5f;
    enemy_texture = NULL;
    health = 5;
    last_shot_time = 0;
}

AdvancedEnemy::~AdvancedEnemy()
{
    for (auto pow : enemy_pows) {
        delete pow;
    }
    enemy_pows.clear();
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
}

bool AdvancedEnemy::Update()
{
    posX += velX;
    enemy_rect.x = static_cast<int>(posX);
    if (enemy_rect.x <= 0 || enemy_rect.x >= WIDTH - enemy_rect.w) {
        velX = -velX;
    }

    for (auto& pow : enemy_pows) {
        pow->Update();
    }

    return true;
}

void AdvancedEnemy::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);
    for (auto pow : enemy_pows) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_Rect pow_rect = pow->GetRect();
        SDL_RenderDrawRect(renderer, &pow_rect);
        pow->Render(renderer);
    }
}

void AdvancedEnemy::Shoot(SDL_Renderer* renderer)
{
    Uint32 current_time = SDL_GetTicks();
    const Uint32 shoot_delay = 3000;
    if (current_time - last_shot_time >= shoot_delay) {
        POW* new_pow = new POW();
        if (new_pow->LoadImg("dan/1.png", renderer)) {
            new_pow->SetSize(30, 30);
            new_pow->SetPosition(enemy_rect.x + enemy_rect.w / 2 - 15, enemy_rect.y + enemy_rect.h);
            new_pow->SetDamage(1);
            new_pow->SetDirection(0, 0.5f);
            enemy_pows.push_back(new_pow);
            last_shot_time = current_time;
            std::cout << "AdvancedEnemy fired a bullet!" << std::endl;
        } else {
            std::cout << "Failed to load enemy bullet image!" << std::endl;
            delete new_pow;
        }
    }
}

void AdvancedEnemy::SetPosition(int x, int y)
{
    posX = static_cast<float>(x);
    enemy_rect.x = x;
    posY = static_cast<float>(y);
    enemy_rect.y = y;
}

// Boss
Boss::Boss()
{
    enemy_rect = {0, 0, 100, 100}; // Kích thước lớn hơn
    velY = 0;
    posX = 0.0f;
    velX = 0.5f; // Di chuyển ngang
    enemy_texture = NULL;
    health = 20; // Máu cao hơn
    last_shot_time = 0;
}

Boss::~Boss()
{
    for (auto pow : boss_pows) {
        delete pow;
    }
    boss_pows.clear();
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
}

bool Boss::Update()
{
    posX += velX;
    enemy_rect.x = static_cast<int>(posX);
    if (enemy_rect.x <= 0 || enemy_rect.x >= WIDTH - enemy_rect.w) {
        velX = -velX; // Đổi hướng khi chạm biên
    }

    for (auto& pow : boss_pows) {
        pow->Update();
    }

    return true;
}

void Boss::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);
    for (auto pow : boss_pows) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Đạn boss màu đỏ
        SDL_Rect pow_rect = pow->GetRect();
        SDL_RenderDrawRect(renderer, &pow_rect);
        pow->Render(renderer);
    }
}

void Boss::Shoot(SDL_Renderer* renderer)
{
    Uint32 current_time = SDL_GetTicks();
    const Uint32 shoot_delay = 2000; // Bắn mỗi 2 giây
    if (current_time - last_shot_time >= shoot_delay) {
        // Bắn đạn về 5 hướng (góc 0, 45, 90, 135, 180 độ)
        float angles[] = {0, M_PI/4, M_PI/2, 3*M_PI/4, M_PI}; // Đơn vị radian
        for (float angle : angles) {
            POW* new_pow = new POW();
            if (new_pow->LoadImg("dan/1.png", renderer)) {
                new_pow->SetSize(20, 20);
                new_pow->SetPosition(enemy_rect.x + enemy_rect.w / 2 - 10, enemy_rect.y + enemy_rect.h / 2);
                new_pow->SetDamage(2);
                float speed = 0.5f; // Tốc độ đạn
                new_pow->SetDirection(speed * cos(angle), speed * sin(angle)); // Tính hướng dựa trên góc
                boss_pows.push_back(new_pow);
            } else {
                delete new_pow;
            }
        }
        last_shot_time = current_time;
        std::cout << "Boss fired bullets in multiple directions!" << std::endl;
    }
}

void Boss::SetPosition(int x, int y)
{
    posX = static_cast<float>(x);
    enemy_rect.x = x;
    posY = static_cast<float>(y);
    enemy_rect.y = y;
}
