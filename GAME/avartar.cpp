
#include "avartar.hpp"

AVARTAR::AVARTAR()
{
    player_texture = NULL;
    player_rect = {0, 0, 150, 150};
    velX = 0;
    velY = 0;
}

AVARTAR::~AVARTAR()
{
    if (player_texture != NULL) {
        SDL_DestroyTexture(player_texture);
        player_texture = NULL;
    }
}

bool AVARTAR::LoadImg(const char* name, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(name);
    if (surface == NULL) {
        std::cout << "Không thể load ảnh: " << name << " - " << IMG_GetError() << std::endl;
        return false;
    }
    player_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(player_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return (player_texture != NULL);
}

void AVARTAR::SetPosition(int x, int y)
{
    player_rect.x = x;
    player_rect.y = y;
}

void AVARTAR::Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, player_texture, NULL, &player_rect);
}

void AVARTAR::HandleEvent(SDL_Event& e, SDL_Renderer* renderer)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: velX = -1; break;
            case SDLK_RIGHT: velX = 1; break;
            case SDLK_UP: velY = -1; break;
            case SDLK_DOWN: velY = 1; break;
            case SDLK_l:  // Khi ấn phím "1" thì bắn đạn
                POW* new_pow = new POW();
                if (new_pow->LoadImg("dan/1.png", renderer)) {
                    new_pow->SetPosition(player_rect.x + player_rect.w / 9, player_rect.y);
                    new_pow->SetDirection(0, -1); // Bay lên với tốc độ BULLET_SPEED
                    pows.push_back(new_pow);
                } else {
                    std::cout << "Failed to load bullet image: " << IMG_GetError() << std::endl;
                    delete new_pow; // Giải phóng nếu load thất bại
                }
                break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT: velX = 0; break;
            case SDLK_UP:
            case SDLK_DOWN: velY = 0; break;
        }
    }
}

void AVARTAR::UpdatePows()
{
    for (auto& pow : pows) {
        pow->Update();
    }
}

void AVARTAR::UpdatePosition()
{
    player_rect.x += velX;
    player_rect.y += velY;

    // Giới hạn nhân vật trong màn hình
    if (player_rect.x < 0) player_rect.x = 0;
    if (player_rect.y < 0) player_rect.y = 0;
    if (player_rect.x > WIDTH - player_rect.w) player_rect.x = WIDTH - player_rect.w;
    if (player_rect.y > HEIGHT - player_rect.h) player_rect.y = HEIGHT - player_rect.h;
}

