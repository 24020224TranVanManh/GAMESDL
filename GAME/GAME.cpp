/*
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Baseoject.h"
#include "comfunc.h"
#include "avartar.hpp"
#include "item.h"
#include "Enemy.h"
#include <cstdlib>
#include <string>

// Khai báo toàn cục
Baseoject gBackground;
TTF_Font* gFont = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        success = false;
    } else {
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gRender == NULL) success = false;
        else {
            SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        }
        int imgl = IMG_INIT_PNG;
        if (!(IMG_Init(imgl) & imgl)) success = false;
    }

    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        success = false;
    } else {
        gFont = TTF_OpenFont("VN3D.TTF", 24);
        if (gFont == NULL) {
            std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
            success = false;
        }
    }

    return success;
}

bool loadBackground()
{
    return gBackground.LoadImg("anh/anh.jpg", gRender);
}

void close()
{
    gBackground.free();
    if (gFont != NULL) {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }
    TTF_Quit();
    SDL_DestroyRenderer(gRender);
    SDL_DestroyWindow(gWindow);
    gRender = NULL;
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (textSurface == NULL) {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return texture;
}

int main(int argc, char* argv[])
{
    if (!InitData()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadBackground()) {
        std::cout << "Failed to load background!" << std::endl;
        return -1;
    }

    AVARTAR P_player;
    if (!P_player.LoadImg("nhanvat/left.png", gRender)) {
        std::cout << "Failed to load player image!" << std::endl;
        return -1;
    }
    P_player.SetPosition(500, 500);

    std::vector<Item*> items;

    Item* fire_item = new Item();
    std::vector<Enemy*> enemies;
    const int MAX_ENEMIES = 7;
    Uint32 game_start_time = SDL_GetTicks();
    bool boss_spawned = false;
    int score = 0;

    Uint32 last_spawn_time = 0;
    const Uint32 spawn_delay = 2000;

    bool is_quit = false;

    while (!is_quit) {
        while (SDL_PollEvent(&gEvent) != 0) {
            if (gEvent.type == SDL_QUIT) {
                is_quit = true;
            }
            P_player.HandleEvent(gEvent, gRender);
        }

        if (!P_player.IsAlive()) {
            SDL_Color loseColor = {255, 0, 0, 255};
            SDL_Texture* loseTexture = loadTextTexture("YOU LOSE", loseColor, gRender);
            if (loseTexture) {
                int w, h;
                SDL_QueryTexture(loseTexture, NULL, NULL, &w, &h);
                SDL_Rect loseRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                SDL_RenderClear(gRender);
                gBackground.render(gRender, NULL);
                SDL_RenderCopy(gRender, loseTexture, NULL, &loseRect);
                SDL_RenderPresent(gRender);
                SDL_Delay(2000);
                SDL_DestroyTexture(loseTexture);
            }
            std::cout << "Game Over: Player died!" << std::endl;
            is_quit = true;
            break;
        }

        Uint32 current_time = SDL_GetTicks();
        bool only_enemy1 = (current_time - game_start_time < 10000);

        if (!boss_spawned && score >= 3000) {
            Boss* new_boss = new Boss();
            if (new_boss->LoadImg("quaivat/boss.png", gRender)) {
                new_boss->SetPosition(WIDTH / 2 - 50, 50);
                enemies.push_back(new_boss);
                boss_spawned = true;
                std::cout << "Boss spawned!" << std::endl;
            } else {
                std::cout << "Failed to load boss image!" << std::endl;
                delete new_boss;
            }
        }

        // Quái thường vẫn xuất hiện ngay cả khi boss có mặt
        if (current_time - last_spawn_time >= spawn_delay && enemies.size() < MAX_ENEMIES) {
            if (only_enemy1 || (!only_enemy1 && rand() % 2 == 0)) {
                Enemy* new_enemy = new Enemy();
                if (new_enemy->LoadImg("quaivat/1.png", gRender)) {
                    int x = rand() % (WIDTH - 150);
                    new_enemy->SetPosition(x, -150);
                    enemies.push_back(new_enemy);
                } else {
                    std::cout << "Failed to load enemy image!" << std::endl;
                    delete new_enemy;
                }
            } else {
                AdvancedEnemy* new_enemy = new AdvancedEnemy();
                if (new_enemy->LoadImg("quaivat/2.png", gRender)) {
                    int x = rand() % (WIDTH - 150);
                    new_enemy->SetPosition(x, 0);
                    enemies.push_back(new_enemy);
                } else {
                    std::cout << "Failed to load advanced enemy image!" << std::endl;
                    delete new_enemy;
                }
            }
            last_spawn_time = current_time;
        }

        P_player.UpdatePosition();
        P_player.UpdatePows();
        P_player.CheckCollisionWithItems(items);

        for (auto it = items.begin(); it != items.end();) {
            (*it)->Update();
            if (!(*it)->IsActive()) {
                delete *it;
                it = items.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->Update()) {
                delete *it;
                it = enemies.erase(it);
            } else {
                AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*it);
                Boss* boss = dynamic_cast<Boss*>(*it);
                if (adv_enemy) {
                    adv_enemy->Shoot(gRender);
                } else if (boss) {
                    boss->Shoot(gRender);
                }
                for (auto other = enemies.begin(); other != enemies.end(); ++other) {
                    if (*it != *other) {
                        SDL_Rect rect1 = (*it)->GetRect();
                        SDL_Rect rect2 = (*other)->GetRect();
                        if (SDL_HasIntersection(&rect1, &rect2)) {
                            AdvancedEnemy* adv1 = dynamic_cast<AdvancedEnemy*>(*it);
                            AdvancedEnemy* adv2 = dynamic_cast<AdvancedEnemy*>(*other);
                            if (adv1 && adv2) {
                                adv1->SetVelX(-adv1->GetVelX());
                                adv2->SetVelX(-adv2->GetVelX());
                            }
                        }
                    }
                }
                ++it;
            }
        }

        auto pows = P_player.get_pows();
        for (auto it = pows.begin(); it != pows.end();) {
            SDL_Rect pow_rect = (*it)->GetRect();
            if (pow_rect.y + pow_rect.h < 0) {
                delete *it;
                it = pows.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = pows.begin(); it != pows.end();) {
            bool hit = false;
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
                SDL_Rect pow_rect = (*it)->GetRect();
                SDL_Rect enemy_rect = (*enemy_it)->GetRect();
                if ((*enemy_it)->IsAlive() && SDL_HasIntersection(&pow_rect, &enemy_rect)) {
                    std::cout << "Bullet hit enemy! Damage: " << (*it)->GetDamage() << std::endl;
                    (*enemy_it)->TakeDamage((*it)->GetDamage());
                    hit = true;
                    if (!(*enemy_it)->IsAlive()) {
                        Enemy* basic_enemy = dynamic_cast<Enemy*>(*enemy_it);
                        AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*enemy_it);
                        Boss* boss = dynamic_cast<Boss*>(*enemy_it);
                        if (basic_enemy && !adv_enemy && !boss) {
                            score += 100;
                        } else if (adv_enemy && !boss) {
                            score += 200;
                        } else if (boss) {
                            SDL_Color winColor = {255, 255, 0, 255};
                            SDL_Texture* winTexture = loadTextTexture("YOU WIN", winColor, gRender);
                            if (winTexture) {
                                int w, h;
                                SDL_QueryTexture(winTexture, NULL, NULL, &w, &h);
                                SDL_Rect winRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                                SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                                SDL_RenderPresent(gRender);
                                SDL_Delay(2000);
                                SDL_DestroyTexture(winTexture);
                            }
                            is_quit = true;
                        }
                        // Tỉ lệ rơi item 30%
                        if (basic_enemy && !adv_enemy && rand() % 100 < 30) {
                            Item* fire_drop = new Item();
                            if (fire_drop->LoadImg("items/1.jpg", gRender)) {
                                fire_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fire_drop->SetBulletType(BULLET_FIRE);
                                items.push_back(fire_drop);
                            } else {
                                delete fire_drop;
                            }
                        } else if (adv_enemy && rand() % 100 < 30) {
                            Item* fast_drop = new Item();
                            if (fast_drop->LoadImg("items/2.jpg", gRender)) {
                                fast_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fast_drop->SetBulletType(BULLET_FAST);
                                items.push_back(fast_drop);
                            } else {
                                delete fast_drop;
                            }
                        }
                        delete *enemy_it;
                        enemy_it = enemies.erase(enemy_it);
                    } else {
                        ++enemy_it;
                    }
                    break;
                } else {
                    ++enemy_it;
                }
            }
            if (hit) {
                delete *it;
                it = pows.erase(it);
            } else {
                ++it;
            }
        }
        P_player.set_bullet_list(pows);

        for (auto it = enemies.begin(); it != enemies.end();) {
            SDL_Rect player_rect = P_player.GetRect();
            SDL_Rect enemy_rect = (*it)->GetRect();
            if ((*it)->IsAlive() && SDL_HasIntersection(&player_rect, &enemy_rect)) {
                (*it)->TakeDamage(9999);
                P_player.TakeDamage(1);
                if (!(*it)->IsAlive()) {
                    Enemy* basic_enemy = dynamic_cast<Enemy*>(*it);
                    AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*it);
                    Boss* boss = dynamic_cast<Boss*>(*it);
                    if (basic_enemy && !adv_enemy && !boss) {
                        score += 100;
                    } else if (adv_enemy && !boss) {
                        score += 200;
                    } else if (boss) {
                        SDL_Color winColor = {255, 255, 0, 255};
                        SDL_Texture* winTexture = loadTextTexture("YOU WIN", winColor, gRender);
                        if (winTexture) {
                            int w, h;
                            SDL_QueryTexture(winTexture, NULL, NULL, &w, &h);
                            SDL_Rect winRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                            SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                            SDL_RenderPresent(gRender);
                            SDL_Delay(2000);
                            SDL_DestroyTexture(winTexture);
                        }
                        is_quit = true;
                    }
                    // Tỉ lệ rơi item 30%
                    if (basic_enemy && !adv_enemy && rand() % 100 < 30) {
                        Item* fire_drop = new Item();
                        if (fire_drop->LoadImg("items/1.jpg", gRender)) {
                            fire_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                            fire_drop->SetBulletType(BULLET_FIRE);
                            items.push_back(fire_drop);
                        } else {
                            delete fire_drop;
                        }
                    } else if (adv_enemy && rand() % 100 < 30) {
                        Item* fast_drop = new Item();
                        if (fast_drop->LoadImg("items/2.jpg", gRender)) {
                            fast_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                            fast_drop->SetBulletType(BULLET_FAST);
                            items.push_back(fast_drop);
                        } else {
                            delete fast_drop;
                        }
                    }
                    delete *it;
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }

        for (auto enemy : enemies) {
            AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(enemy);
            Boss* boss = dynamic_cast<Boss*>(enemy);
            if (adv_enemy) {
                auto enemy_pows = adv_enemy->GetPows();
                for (auto it = enemy_pows.begin(); it != enemy_pows.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect player_rect = P_player.GetRect();
                    if (SDL_HasIntersection(&pow_rect, &player_rect)) {
                        P_player.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else if (pow_rect.y > HEIGHT) {
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else {
                        ++it;
                    }
                }
                adv_enemy->GetPows() = enemy_pows;
            } else if (boss) {
                auto boss_pows = boss->GetPows();
                for (auto it = boss_pows.begin(); it != boss_pows.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect player_rect = P_player.GetRect();
                    if (SDL_HasIntersection(&pow_rect, &player_rect)) {
                        P_player.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = boss_pows.erase(it);
                    } else if (pow_rect.y > HEIGHT || pow_rect.y < 0 || pow_rect.x < 0 || pow_rect.x > WIDTH) {
                        delete *it;
                        it = boss_pows.erase(it);
                    } else {
                        ++it;
                    }
                }
                boss->GetPows() = boss_pows;
            }
        }

        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->IsAlive()) {
                delete *it;
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        SDL_RenderClear(gRender);
        gBackground.render(gRender, NULL);

        for (auto item : items) {
            item->Render(gRender);
        }

        for (auto pow : P_player.get_pows()) {
            SDL_SetRenderDrawColor(gRender, 255, 165, 0, 255);
            SDL_Rect pow_rect = pow->GetRect();
            SDL_RenderDrawRect(gRender, &pow_rect);
            pow->Render(gRender);
        }

        for (auto enemy : enemies) {
            SDL_SetRenderDrawColor(gRender, 255, 0, 0, 255);
            SDL_Rect enemy_rect = enemy->GetRect();
            SDL_RenderDrawRect(gRender, &enemy_rect);
            enemy->Render(gRender);
        }

        P_player.Draw(gRender);

        // Hiển thị điểm số
        SDL_Color textColor = {255, 255, 255, 255};
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Texture* scoreTexture = loadTextTexture(scoreText, textColor, gRender);
        int scoreHeight = 0;
        if (scoreTexture) {
            int w, h;
            SDL_QueryTexture(scoreTexture, NULL, NULL, &w, &h);
            SDL_Rect scoreRect = {10, 10, w, h};
            SDL_RenderCopy(gRender, scoreTexture, NULL, &scoreRect);
            scoreHeight = h;
            SDL_DestroyTexture(scoreTexture);
        }

        // Hiển thị số máu
        std::string healthText = "Health: " + std::to_string(P_player.GetHealth());
        SDL_Texture* healthTexture = loadTextTexture(healthText, textColor, gRender);
        if (healthTexture) {
            int w, h;
            SDL_QueryTexture(healthTexture, NULL, NULL, &w, &h);
            SDL_Rect healthRect = {10, 10 + scoreHeight + 5, w, h};
            SDL_RenderCopy(gRender, healthTexture, NULL, &healthRect);
            SDL_DestroyTexture(healthTexture);
        }

        SDL_RenderPresent(gRender);
    }

    for (auto item : items) {
        delete item;
    }
    items.clear();

    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    close();
    return 0;
}
*/
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Baseoject.h"
#include "comfunc.h"
#include "avartar.hpp"
#include "item.h"
#include "Enemy.h"
#include <cstdlib>
#include <string>

// Khai báo toàn cục
Baseoject gBackground;
TTF_Font* gFont = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        success = false;
    } else {
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gRender == NULL) success = false;
        else {
            SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        }
        int imgl = IMG_INIT_PNG;
        if (!(IMG_Init(imgl) & imgl)) success = false;
    }

    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        success = false;
    } else {
        gFont = TTF_OpenFont("VN3D.TTF", 24);
        if (gFont == NULL) {
            std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
            success = false;
        }
    }

    return success;
}

bool loadBackground()
{
    return gBackground.LoadImg("anh/anh.jpg", gRender);
}

void close()
{
    gBackground.free();
    if (gFont != NULL) {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }
    TTF_Quit();
    SDL_DestroyRenderer(gRender);
    SDL_DestroyWindow(gWindow);
    gRender = NULL;
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (textSurface == NULL) {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return texture;
}

int main(int argc, char* argv[])
{
    if (!InitData()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadBackground()) {
        std::cout << "Failed to load background!" << std::endl;
        return -1;
    }

    // Player 1
    AVARTAR P_player1(1);
    if (!P_player1.LoadImg("nhanvat/left.png", gRender)) {
        std::cout << "Failed to load player 1 image!" << std::endl;
        return -1;
    }
    P_player1.SetPosition(500, 500);

    // Player 2
    AVARTAR P_player2(2);
    if (!P_player2.LoadImg("nhanvat/right.png", gRender)) {
        std::cout << "Failed to load player 2 image!" << std::endl;
        return -1;
    }
    P_player2.SetPosition(700, 500);

    std::vector<Item*> items;
    std::vector<Enemy*> enemies;
    const int MAX_ENEMIES = 7;
    Uint32 game_start_time = SDL_GetTicks();
    bool boss_spawned = false;
    int score = 0;

    Uint32 last_spawn_time = 0;
    const Uint32 spawn_delay = 2000;

    bool is_quit = false;

    while (!is_quit) {
        while (SDL_PollEvent(&gEvent) != 0) {
            if (gEvent.type == SDL_QUIT) {
                is_quit = true;
            }
            P_player1.HandleEvent(gEvent, gRender);
            P_player2.HandleEvent(gEvent, gRender);
        }

        if (!P_player1.IsAlive() || !P_player2.IsAlive()) {
            SDL_Color loseColor = {255, 0, 0, 255};
            SDL_Texture* loseTexture = loadTextTexture("YOU LOSE", loseColor, gRender);
            if (loseTexture) {
                int w, h;
                SDL_QueryTexture(loseTexture, NULL, NULL, &w, &h);
                SDL_Rect loseRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                SDL_RenderClear(gRender);
                gBackground.render(gRender, NULL);
                SDL_RenderCopy(gRender, loseTexture, NULL, &loseRect);
                SDL_RenderPresent(gRender);
                SDL_Delay(2000);
                SDL_DestroyTexture(loseTexture);
            }
            std::cout << "Game Over: One of the players died!" << std::endl;
            is_quit = true;
            break;
        }

        Uint32 current_time = SDL_GetTicks();
        bool only_enemy1 = (current_time - game_start_time < 10000);

        if (!boss_spawned && score >= 3000) {
            Boss* new_boss = new Boss();
            if (new_boss->LoadImg("quaivat/boss.png", gRender)) {
                new_boss->SetPosition(WIDTH / 2 - 50, 50);
                enemies.push_back(new_boss);
                boss_spawned = true;
                std::cout << "Boss spawned!" << std::endl;
            } else {
                std::cout << "Failed to load boss image!" << std::endl;
                delete new_boss;
            }
        }

        if (current_time - last_spawn_time >= spawn_delay && enemies.size() < MAX_ENEMIES) {
            if (only_enemy1 || (!only_enemy1 && rand() % 2 == 0)) {
                Enemy* new_enemy = new Enemy();
                if (new_enemy->LoadImg("quaivat/1.png", gRender)) {
                    int x = rand() % (WIDTH - 150);
                    new_enemy->SetPosition(x, -150);
                    enemies.push_back(new_enemy);
                } else {
                    std::cout << "Failed to load enemy image!" << std::endl;
                    delete new_enemy;
                }
            } else {
                AdvancedEnemy* new_enemy = new AdvancedEnemy();
                if (new_enemy->LoadImg("quaivat/2.png", gRender)) {
                    int x = rand() % (WIDTH - 150);
                    new_enemy->SetPosition(x, 0);
                    enemies.push_back(new_enemy);
                } else {
                    std::cout << "Failed to load advanced enemy image!" << std::endl;
                    delete new_enemy;
                }
            }
            last_spawn_time = current_time;
        }

        P_player1.UpdatePosition();
        P_player1.UpdatePows();
        P_player1.CheckCollisionWithItems(items);

        P_player2.UpdatePosition();
        P_player2.UpdatePows();
        P_player2.CheckCollisionWithItems(items);

        for (auto it = items.begin(); it != items.end();) {
            (*it)->Update();
            if (!(*it)->IsActive()) {
                delete *it;
                it = items.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->Update()) {
                delete *it;
                it = enemies.erase(it);
            } else {
                AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*it);
                Boss* boss = dynamic_cast<Boss*>(*it);
                if (adv_enemy) {
                    adv_enemy->Shoot(gRender);
                } else if (boss) {
                    boss->Shoot(gRender);
                }
                for (auto other = enemies.begin(); other != enemies.end(); ++other) {
                    if (*it != *other) {
                        SDL_Rect rect1 = (*it)->GetRect();
                        SDL_Rect rect2 = (*other)->GetRect();
                        if (SDL_HasIntersection(&rect1, &rect2)) {
                            AdvancedEnemy* adv1 = dynamic_cast<AdvancedEnemy*>(*it);
                            AdvancedEnemy* adv2 = dynamic_cast<AdvancedEnemy*>(*other);
                            if (adv1 && adv2) {
                                adv1->SetVelX(-adv1->GetVelX());
                                adv2->SetVelX(-adv2->GetVelX());
                            }
                        }
                    }
                }
                ++it;
            }
        }

        // Xử lý đạn của Player 1
        auto pows1 = P_player1.get_pows(); // Sử dụng auto thay vì auto&
        for (auto it = pows1.begin(); it != pows1.end();) {
            bool hit = false;
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
                SDL_Rect pow_rect = (*it)->GetRect();
                SDL_Rect enemy_rect = (*enemy_it)->GetRect();
                if ((*enemy_it)->IsAlive() && SDL_HasIntersection(&pow_rect, &enemy_rect)) {
                    std::cout << "Player 1 Bullet hit enemy! Damage: " << (*it)->GetDamage() << std::endl;
                    (*enemy_it)->TakeDamage((*it)->GetDamage());
                    hit = true;
                    if (!(*enemy_it)->IsAlive()) {
                        Enemy* basic_enemy = dynamic_cast<Enemy*>(*enemy_it);
                        AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*enemy_it);
                        Boss* boss = dynamic_cast<Boss*>(*enemy_it);
                        if (basic_enemy && !adv_enemy && !boss) {
                            score += 100;
                        } else if (adv_enemy && !boss) {
                            score += 200;
                        } else if (boss) {
                            SDL_Color winColor = {255, 255, 0, 255};
                            SDL_Texture* winTexture = loadTextTexture("YOU WIN", winColor, gRender);
                            if (winTexture) {
                                int w, h;
                                SDL_QueryTexture(winTexture, NULL, NULL, &w, &h);
                                SDL_Rect winRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                                SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                                SDL_RenderPresent(gRender);
                                SDL_Delay(2000);
                                SDL_DestroyTexture(winTexture);
                            }
                            is_quit = true;
                        }
                        if (basic_enemy && !adv_enemy && rand() % 100 < 30) {
                            Item* fire_drop = new Item();
                            if (fire_drop->LoadImg("items/1.jpg", gRender)) {
                                fire_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fire_drop->SetBulletType(BULLET_FIRE);
                                items.push_back(fire_drop);
                            } else {
                                delete fire_drop;
                            }
                        } else if (adv_enemy && rand() % 100 < 30) {
                            Item* fast_drop = new Item();
                            if (fast_drop->LoadImg("items/2.jpg", gRender)) {
                                fast_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fast_drop->SetBulletType(BULLET_FAST);
                                items.push_back(fast_drop);
                            } else {
                                delete fast_drop;
                            }
                        }
                        delete *enemy_it;
                        enemy_it = enemies.erase(enemy_it);
                    } else {
                        ++enemy_it;
                    }
                    break;
                } else {
                    ++enemy_it;
                }
            }
            if (hit) {
                delete *it;
                it = pows1.erase(it);
            } else {
                ++it;
            }
        }
        P_player1.set_bullet_list(pows1);

        // Xử lý đạn của Player 2
        auto pows2 = P_player2.get_pows(); // Sử dụng auto thay vì auto&
        for (auto it = pows2.begin(); it != pows2.end();) {
            bool hit = false;
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
                SDL_Rect pow_rect = (*it)->GetRect();
                SDL_Rect enemy_rect = (*enemy_it)->GetRect();
                if ((*enemy_it)->IsAlive() && SDL_HasIntersection(&pow_rect, &enemy_rect)) {
                    std::cout << "Player 2 Bullet hit enemy! Damage: " << (*it)->GetDamage() << std::endl;
                    (*enemy_it)->TakeDamage((*it)->GetDamage());
                    hit = true;
                    if (!(*enemy_it)->IsAlive()) {
                        Enemy* basic_enemy = dynamic_cast<Enemy*>(*enemy_it);
                        AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*enemy_it);
                        Boss* boss = dynamic_cast<Boss*>(*enemy_it);
                        if (basic_enemy && !adv_enemy && !boss) {
                            score += 100;
                        } else if (adv_enemy && !boss) {
                            score += 200;
                        } else if (boss) {
                            SDL_Color winColor = {255, 255, 0, 255};
                            SDL_Texture* winTexture = loadTextTexture("YOU WIN", winColor, gRender);
                            if (winTexture) {
                                int w, h;
                                SDL_QueryTexture(winTexture, NULL, NULL, &w, &h);
                                SDL_Rect winRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                                SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                                SDL_RenderPresent(gRender);
                                SDL_Delay(2000);
                                SDL_DestroyTexture(winTexture);
                            }
                            is_quit = true;
                        }
                        if (basic_enemy && !adv_enemy && rand() % 100 < 30) {
                            Item* fire_drop = new Item();
                            if (fire_drop->LoadImg("items/1.jpg", gRender)) {
                                fire_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fire_drop->SetBulletType(BULLET_FIRE);
                                items.push_back(fire_drop);
                            } else {
                                delete fire_drop;
                            }
                        } else if (adv_enemy && rand() % 100 < 30) {
                            Item* fast_drop = new Item();
                            if (fast_drop->LoadImg("items/2.jpg", gRender)) {
                                fast_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fast_drop->SetBulletType(BULLET_FAST);
                                items.push_back(fast_drop);
                            } else {
                                delete fast_drop;
                            }
                        }
                        delete *enemy_it;
                        enemy_it = enemies.erase(enemy_it);
                    } else {
                        ++enemy_it;
                    }
                    break;
                } else {
                    ++enemy_it;
                }
            }
            if (hit) {
                delete *it;
                it = pows2.erase(it);
            } else {
                ++it;
            }
        }
        P_player2.set_bullet_list(pows2);

        for (auto it = enemies.begin(); it != enemies.end();) {
            SDL_Rect player_rect1 = P_player1.GetRect();
            SDL_Rect player_rect2 = P_player2.GetRect();
            SDL_Rect enemy_rect = (*it)->GetRect();
            if ((*it)->IsAlive()) {
                if (SDL_HasIntersection(&player_rect1, &enemy_rect)) {
                    (*it)->TakeDamage(9999);
                    P_player1.TakeDamage(1);
                }
                if (SDL_HasIntersection(&player_rect2, &enemy_rect)) {
                    (*it)->TakeDamage(9999);
                    P_player2.TakeDamage(1);
                }
                if (!(*it)->IsAlive()) {
                    Enemy* basic_enemy = dynamic_cast<Enemy*>(*it);
                    AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*it);
                    Boss* boss = dynamic_cast<Boss*>(*it);
                    if (basic_enemy && !adv_enemy && !boss) {
                        score += 100;
                    } else if (adv_enemy && !boss) {
                        score += 200;
                    } else if (boss) {
                        SDL_Color winColor = {255, 255, 0, 255};
                        SDL_Texture* winTexture = loadTextTexture("YOU WIN", winColor, gRender);
                        if (winTexture) {
                            int w, h;
                            SDL_QueryTexture(winTexture, NULL, NULL, &w, &h);
                            SDL_Rect winRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h};
                            SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                            SDL_RenderPresent(gRender);
                            SDL_Delay(2000);
                            SDL_DestroyTexture(winTexture);
                        }
                        is_quit = true;
                    }
                    if (basic_enemy && !adv_enemy && rand() % 100 < 30) {
                        Item* fire_drop = new Item();
                        if (fire_drop->LoadImg("items/1.jpg", gRender)) {
                            fire_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                            fire_drop->SetBulletType(BULLET_FIRE);
                            items.push_back(fire_drop);
                        } else {
                            delete fire_drop;
                        }
                    } else if (adv_enemy && rand() % 100 < 30) {
                        Item* fast_drop = new Item();
                        if (fast_drop->LoadImg("items/2.jpg", gRender)) {
                            fast_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                            fast_drop->SetBulletType(BULLET_FAST);
                            items.push_back(fast_drop);
                        } else {
                            delete fast_drop;
                        }
                    }
                    delete *it;
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }

        for (auto enemy : enemies) {
            AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(enemy);
            Boss* boss = dynamic_cast<Boss*>(enemy);
            if (adv_enemy) {
                auto& enemy_pows = adv_enemy->GetPows();
                for (auto it = enemy_pows.begin(); it != enemy_pows.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect player_rect1 = P_player1.GetRect();
                    SDL_Rect player_rect2 = P_player2.GetRect();
                    if (SDL_HasIntersection(&pow_rect, &player_rect1)) {
                        P_player1.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else if (SDL_HasIntersection(&pow_rect, &player_rect2)) {
                        P_player2.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else if (pow_rect.y > HEIGHT) {
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else {
                        ++it;
                    }
                }
            } else if (boss) {
                auto& boss_pows = boss->GetPows();
                for (auto it = boss_pows.begin(); it != boss_pows.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect player_rect1 = P_player1.GetRect();
                    SDL_Rect player_rect2 = P_player2.GetRect();
                    if (SDL_HasIntersection(&pow_rect, &player_rect1)) {
                        P_player1.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = boss_pows.erase(it);
                    } else if (SDL_HasIntersection(&pow_rect, &player_rect2)) {
                        P_player2.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = boss_pows.erase(it);
                    } else if (pow_rect.y > HEIGHT || pow_rect.y < 0 || pow_rect.x < 0 || pow_rect.x > WIDTH) {
                        delete *it;
                        it = boss_pows.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }

        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->IsAlive()) {
                delete *it;
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        SDL_RenderClear(gRender);
        gBackground.render(gRender, NULL);

        for (auto item : items) {
            item->Render(gRender);
        }

        for (auto pow : P_player1.get_pows()) {
            SDL_SetRenderDrawColor(gRender, 255, 165, 0, 255);
            SDL_Rect pow_rect = pow->GetRect();
            SDL_RenderDrawRect(gRender, &pow_rect);
            pow->Render(gRender);
        }
        for (auto pow : P_player2.get_pows()) {
            SDL_SetRenderDrawColor(gRender, 0, 255, 0, 255);
            SDL_Rect pow_rect = pow->GetRect();
            SDL_RenderDrawRect(gRender, &pow_rect);
            pow->Render(gRender);
        }

        for (auto enemy : enemies) {
            SDL_SetRenderDrawColor(gRender, 255, 0, 0, 255);
            SDL_Rect enemy_rect = enemy->GetRect();
            SDL_RenderDrawRect(gRender, &enemy_rect);
            enemy->Render(gRender);
        }

        P_player1.Draw(gRender);
        P_player2.Draw(gRender);

        SDL_Color textColor = {255, 255, 255, 255};
        std::string scoreText = "Score: " + std::to_string(score);
        SDL_Texture* scoreTexture = loadTextTexture(scoreText, textColor, gRender);
        int scoreHeight = 0;
        if (scoreTexture) {
            int w, h;
            SDL_QueryTexture(scoreTexture, NULL, NULL, &w, &h);
            SDL_Rect scoreRect = {10, 10, w, h};
            SDL_RenderCopy(gRender, scoreTexture, NULL, &scoreRect);
            scoreHeight = h;
            SDL_DestroyTexture(scoreTexture);
        }

        std::string healthText1 = "P1 Health: " + std::to_string(P_player1.GetHealth());
        SDL_Texture* healthTexture1 = loadTextTexture(healthText1, textColor, gRender);
        if (healthTexture1) {
            int w, h;
            SDL_QueryTexture(healthTexture1, NULL, NULL, &w, &h);
            SDL_Rect healthRect1 = {10, 10 + scoreHeight + 5, w, h};
            SDL_RenderCopy(gRender, healthTexture1, NULL, &healthRect1);
            SDL_DestroyTexture(healthTexture1);
        }

        std::string healthText2 = "P2 Health: " + std::to_string(P_player2.GetHealth());
        SDL_Texture* healthTexture2 = loadTextTexture(healthText2, textColor, gRender);
        if (healthTexture2) {
            int w, h;
            SDL_QueryTexture(healthTexture2, NULL, NULL, &w, &h);
            SDL_Rect healthRect2 = {10, 10 + scoreHeight + 30, w, h};
            SDL_RenderCopy(gRender, healthTexture2, NULL, &healthRect2);
            SDL_DestroyTexture(healthTexture2);
        }

        SDL_RenderPresent(gRender);
    }

    for (auto item : items) {
        delete item;
    }
    items.clear();

    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    close();
    return 0;
}
