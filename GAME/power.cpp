/*
#include "power.h"

POW::POW()
{
    pow_rect = {0, 0, 100, 100}; // Kích thước viên đạn
    velX = 0;
    velY = -3; // Tốc độ mặc định
    pow_texture = NULL;
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
    SDL_FreeSurface(surface);
    return (pow_texture != NULL);
}

void POW::SetPosition(int x, int y)
{
    pow_rect.x = x;
    pow_rect.y = y;
}

void POW::SetDirection(int vx, int vy)
{
    velX = vx;
    velY = vy;
}

void POW::Update()
{
    pow_rect.x += velX;
    pow_rect.y += velY;
}

void POW::Render(SDL_Renderer* renderer)
{
    if (pow_texture) {
        SDL_RenderCopy(renderer, pow_texture, NULL, &pow_rect);
    }
}
*/
#include "power.h"
#define BULLET_SPEED 1
POW::POW()
{
    pow_rect = {0, 0, 0, 0}; // Kích thước sẽ được đặt sau
    velX = 0;
    velY = -BULLET_SPEED;
    pow_texture = NULL;
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
    pow_rect.x = x;
    pow_rect.y = y;
}

void POW::SetDirection(int vx, int vy)
{
    velX = vx;
    velY = vy;
}

void POW::SetSize(int w, int h)
{
    pow_rect.w = w;
    pow_rect.h = h;
}

void POW::Update()
{
    pow_rect.x += velX;
    pow_rect.y += velY;
}

void POW::Render(SDL_Renderer* renderer)
{
    if (pow_texture) {
        SDL_SetTextureColorMod(pow_texture, 255, 255, 200);
        SDL_RenderCopy(renderer, pow_texture, NULL, &pow_rect);
        SDL_SetTextureColorMod(pow_texture, 255, 255, 255);
    }
}
