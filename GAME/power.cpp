
#include "power.h"

POW::POW()
{
    pow_rect = {0, 0, 10, 10};
    pow_texture = NULL;
    damage = 1;
    velX = 0.0f;
    velY = 0.0f;
    posX = 0.0f;
    posY = 0.0f;
}

POW::~POW()
{
    if (pow_texture != NULL) {
        SDL_DestroyTexture(pow_texture);
        pow_texture = NULL;
    }
}

bool POW::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Không thể load ảnh đạn: " << IMG_GetError() << std::endl;
        return false;
    }
    pow_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(pow_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return (pow_texture != NULL);
}

void POW::SetPosition(int x, int y)
{
    posX = static_cast<float>(x);
    posY = static_cast<float>(y);
    pow_rect.x = x;
    pow_rect.y = y;
}

void POW::SetSize(int w, int h)
{
    pow_rect.w = w;
    pow_rect.h = h;
}

void POW::SetDamage(int dmg)
{
    damage = dmg;
}

void POW::SetDirection(float vx, float vy)
{
    velX = vx;
    velY = vy;
}

void POW::Update()
{
    posX += velX; // Cập nhật vị trí thực tế dựa trên vận tốc
    posY += velY;
    pow_rect.x = static_cast<int>(posX); // Chuyển về int để gán vào SDL_Rect
    pow_rect.y = static_cast<int>(posY);
}

void POW::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, pow_texture, NULL, &pow_rect);
}
