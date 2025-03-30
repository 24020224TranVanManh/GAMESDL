/*#include "item.h"

Item::Item()
{
    item_rect = {0, 0, 30, 30};
    item_texture = NULL;
    bullet_type = BULLET_NORMAL;
    is_active = true;
}

Item::~Item()
{
    if (item_texture != NULL) {
        SDL_DestroyTexture(item_texture);
        item_texture = NULL;
    }
}

bool Item::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Không thể load ảnh vật phẩm: " << IMG_GetError() << std::endl;
        return false;
    }
    item_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(item_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return (item_texture != NULL);
}

void Item::SetPosition(int x, int y)
{
    item_rect.x = x;
    item_rect.y = y;
}

void Item::Render(SDL_Renderer* renderer)
{
    if (is_active && item_texture) {
        SDL_RenderCopy(renderer, item_texture, NULL, &item_rect);
    }
}*/
#include "item.h"

Item::Item()
{
    item_rect = {0, 0, 30, 30};
    item_texture = NULL;
    bullet_type = BULLET_NORMAL;
    is_active = true;
    velY = 0.5f; // Tốc độ rơi nhỏ hơn 1
    posY = 0.0f; // Vị trí thực tế ban đầu
}

Item::~Item()
{
    if (item_texture != NULL) {
        SDL_DestroyTexture(item_texture);
        item_texture = NULL;
    }
}

bool Item::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Không thể load ảnh vật phẩm: " << IMG_GetError() << std::endl;
        return false;
    }
    item_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(item_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return (item_texture != NULL);
}

void Item::SetPosition(int x, int y)
{
    item_rect.x = x;
    posY = static_cast<float>(y); // Gán vị trí thực tế
    item_rect.y = y;
}

void Item::Render(SDL_Renderer* renderer)
{
    if (is_active && item_texture) {
        SDL_RenderCopy(renderer, item_texture, NULL, &item_rect);
    }
}

void Item::Update()
{
    if (is_active) {
        posY += velY; // Cập nhật vị trí thực tế
        item_rect.y = static_cast<int>(posY); // Chuyển đổi về int
        if (item_rect.y > HEIGHT) { // Nếu ra khỏi màn hình
            is_active = false; // Vô hiệu hóa vật phẩm
        }
    }
}
