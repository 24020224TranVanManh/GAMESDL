
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Baseoject.h"
#include "comfunc.h"
#include "avartar.hpp"
#include "item.h"
#include "Enemy.h"
#include <cstdlib>
#include <string>

// Khai báo các biến toàn cục
// Sự kiện SDL (chuột, bàn phím,...)
Baseoject gBackground;              // Đối tượng nền game
TTF_Font* gFont = NULL;             // Font chữ để hiển thị văn bản
Mix_Music* gMenuMusic = NULL;       // Nhạc nền cho menu
Mix_Music* gGameMusic = NULL;       // Nhạc nền khi chơi game
Mix_Chunk* gLoseSound = NULL;       // Âm thanh khi thua
Mix_Chunk* gWinSound = NULL;        // Âm thanh khi thắng

// Hàm khởi tạo SDL và các thành phần cần thiết
bool InitData()
{
    bool success = true; // Biến kiểm tra khởi tạo thành công
    // Khởi tạo SDL với video và âm thanh
    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (ret < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Cải thiện chất lượng render
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    // Tạo cửa sổ game với tiêu đề "GAME", vị trí không xác định, kích thước WIDTH x HEIGHT
    gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        success = false;
    } else {
        // Tạo renderer để vẽ đồ họa, sử dụng chế độ tăng tốc phần cứng
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gRender == NULL) {
            std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
            success = false;
        } else {
            // Đặt màu nền mặc định là trắng (255, 255, 255, 255)
            SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        }
        // Khởi tạo SDL_image để hỗ trợ tải ảnh PNG
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cout << "IMG_Init failed: " << IMG_GetError() << std::endl;
            success = false;
        }
    }

    // Khởi tạo SDL_ttf để hiển thị văn bản
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        success = false;
    } else {
        // Tải font chữ từ file "VN3D.TTF" với kích thước 24
        gFont = TTF_OpenFont("VN3D.TTF", 24);
        if (gFont == NULL) {
            std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
            success = false;
        }
    }

    // Khởi tạo SDL_mixer để xử lý âm thanh (tần số 44100Hz, định dạng mặc định, 2 kênh, buffer 2048)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    // Tải các file âm thanh
    gMenuMusic = Mix_LoadMUS("sounds/vaogame.mp3"); // Nhạc menu
    gGameMusic = Mix_LoadMUS("sounds/ingame.mp3");  // Nhạc trong game
    gLoseSound = Mix_LoadWAV("sounds/lose.mp3");    // Âm thanh thua
    gWinSound = Mix_LoadWAV("sounds/win.mp3");      // Âm thanh thắng

    // Kiểm tra xem các file âm thanh có tải thành công không
    if (!gMenuMusic || !gGameMusic || !gLoseSound || !gWinSound) {
        std::cout << "Failed to load sound files! Mix_Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    return success; // Trả về kết quả khởi tạo
}

// Hàm tải ảnh nền game
bool loadBackground()
{
    // Tải ảnh nền
    return gBackground.LoadImg("anh/anh.jpg", gRender);
}

// Hàm giải phóng tài nguyên
void close()
{
    gBackground.free(); // Giải phóng bộ nhớ của ảnh nền
    if (gFont != NULL) {
        TTF_CloseFont(gFont); // Giải phóng font chữ
        gFont = NULL;
    }
    TTF_Quit(); // Thoát SDL_ttf

    // Giải phóng các file âm thanh
    if (gMenuMusic != NULL) {
        Mix_FreeMusic(gMenuMusic); // Giải phóng nhạc menu
        gMenuMusic = NULL;
    }
    if (gGameMusic != NULL) {
        Mix_FreeMusic(gGameMusic); // Giải phóng nhạc trong game
        gGameMusic = NULL;
    }
    if (gLoseSound != NULL) {
        Mix_FreeChunk(gLoseSound); // Giải phóng âm thanh thua
        gLoseSound = NULL;
    }
    if (gWinSound != NULL) {
        Mix_FreeChunk(gWinSound); // Giải phóng âm thanh thắng
        gWinSound = NULL;
    }
    Mix_Quit(); // Thoát SDL_mixer

    SDL_DestroyRenderer(gRender); // Hủy renderer
    SDL_DestroyWindow(gWindow);   // Hủy cửa sổ game
    gRender = NULL;
    gWindow = NULL;
    IMG_Quit(); // Thoát SDL_image
    SDL_Quit(); // Thoát SDL
}

// Hàm tạo texture văn bản từ chuỗi và màu sắc
SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color, SDL_Renderer* renderer)
{
    // Tạo surface từ văn bản với font và màu được chỉ định
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (textSurface == NULL) {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return NULL;
    }
    // Chuyển surface thành texture để sử dụng trong renderer
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Giải phóng surface
    return texture;
}

// Hàm hiển thị menu chính và xử lý lựa chọn người chơi
int showMainMenu(SDL_Renderer* renderer)
{
    SDL_Color textColor = {255, 255, 255, 255};       // Màu trắng cho chữ
    SDL_Color highlightColor = {255, 255, 0, 255};    // Màu vàng khi hover

    // Tạo texture cho các lựa chọn trong menu
    SDL_Texture* onePlayerText = loadTextTexture("1 Player", textColor, renderer);
    SDL_Texture* twoPlayersText = loadTextTexture("2 Players", textColor, renderer);

    // Vị trí và kích thước của các nút trong menu
    SDL_Rect onePlayerRect = {WIDTH/2 - 100, HEIGHT/2 - 75, 200, 50}; // Nút "1 Player"
    SDL_Rect twoPlayersRect = {WIDTH/2 - 100, HEIGHT/2 + 25, 200, 50}; // Nút "2 Players"

    bool quitMenu = false; // Biến kiểm soát vòng lặp menu
    int selectedOption = -1; // Lựa chọn của người chơi (0: thoát, 1: 1 người, 2: 2 người)

    // Phát nhạc menu khi khởi động game
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(gMenuMusic, -1); // Lặp vô hạn
    }

    // Vòng lặp xử lý menu
    while (!quitMenu) {
        while (SDL_PollEvent(&gEvent) != 0) {
            if (gEvent.type == SDL_QUIT) { // Nếu người dùng đóng cửa sổ
                quitMenu = true;
                selectedOption = 0;
            } else if (gEvent.type == SDL_MOUSEMOTION) { // Khi chuột di chuyển
                int x = gEvent.motion.x;
                int y = gEvent.motion.y;
                // Hover nút "1 Player"
                if (x >= onePlayerRect.x && x <= onePlayerRect.x + onePlayerRect.w &&
                    y >= onePlayerRect.y && y <= onePlayerRect.y + onePlayerRect.h) {
                    SDL_DestroyTexture(onePlayerText);
                    onePlayerText = loadTextTexture("1 Player", highlightColor, renderer); // Đổi màu khi hover
                } else {
                    SDL_DestroyTexture(onePlayerText);
                    onePlayerText = loadTextTexture("1 Player", textColor, renderer);
                }
                // Hover nút "2 Players"
                if (x >= twoPlayersRect.x && x <= twoPlayersRect.x + twoPlayersRect.w &&
                    y >= twoPlayersRect.y && y <= twoPlayersRect.y + twoPlayersRect.h) {
                    SDL_DestroyTexture(twoPlayersText);
                    twoPlayersText = loadTextTexture("2 Players", highlightColor, renderer); // Đổi màu khi hover
                } else {
                    SDL_DestroyTexture(twoPlayersText);
                    twoPlayersText = loadTextTexture("2 Players", textColor, renderer);
                }
            } else if (gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_LEFT) { // Khi nhấn chuột trái
                int x = gEvent.button.x;
                int y = gEvent.button.y;
                // Chọn "1 Player"
                if (x >= onePlayerRect.x && x <= onePlayerRect.x + onePlayerRect.w &&
                    y >= onePlayerRect.y && y <= onePlayerRect.y + onePlayerRect.h) {
                    selectedOption = 1;
                    quitMenu = true;
                }
                // Chọn "2 Players"
                else if (x >= twoPlayersRect.x && x <= twoPlayersRect.x + twoPlayersRect.w &&
                           y >= twoPlayersRect.y && y <= twoPlayersRect.y + twoPlayersRect.h) {
                    selectedOption = 2;
                    quitMenu = true;
                }
            }
        }

        // Xóa màn hình và vẽ lại menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Đặt màu nền là đen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, onePlayerText, NULL, &onePlayerRect);   // Vẽ nút "1 Player"
        SDL_RenderCopy(renderer, twoPlayersText, NULL, &twoPlayersRect); // Vẽ nút "2 Players"
        SDL_RenderPresent(renderer); // Hiển thị lên màn hình
    }

    // Giải phóng texture của menu
    SDL_DestroyTexture(onePlayerText);
    SDL_DestroyTexture(twoPlayersText);

    // Dừng nhạc menu trước khi vào game
    Mix_HaltMusic();

    return selectedOption; // Trả về lựa chọn của người chơi
}

// Hàm chạy game chính
void runGame(int numPlayers)
{
    // Tạo nhân vật người chơi 1
    AVARTAR P_player1(1);
    if (!P_player1.LoadImg("nhanvat/left.png", gRender)) {
        std::cout << "Failed to load player 1 image!" << std::endl;
        return;
    }
    P_player1.SetPosition(500, 500); // Đặt vị trí ban đầu cho P1

    // Tạo nhân vật người chơi 2 (nếu chọn 2 người chơi)
    AVARTAR P_player2(2);
    if (numPlayers == 2 && !P_player2.LoadImg("nhanvat/right.png", gRender)) {
        std::cout << "Failed to load player 2 image!" << std::endl;
        return;
    }
    P_player2.SetPosition(700, 500); // Đặt vị trí ban đầu cho P2

    // Danh sách vật phẩm và kẻ thù
    std::vector<Item*> items;           // Danh sách các vật phẩm rơi
    std::vector<Enemy*> enemies;        // Danh sách các kẻ thù
    const int MAX_SHOOTING_ENEMIES = 5; // Số lượng tối đa kẻ thù bắn
    const int MAX_MOVING_ENEMIES = 5;   // Số lượng tối đa kẻ thù di chuyển
    Uint32 game_start_time = SDL_GetTicks(); // Thời gian bắt đầu game
    bool boss_spawned = false;          // Kiểm tra xem boss đã xuất hiện chưa
    int score = 0;                      // Điểm số của người chơi

    Uint32 last_spawn_time = 0;         // Thời gian sinh kẻ thù cuối cùng
    const Uint32 spawn_delay = 2000;    // Độ trễ giữa các lần sinh kẻ thù (2 giây)

    // Văn bản cảnh báo boss
    SDL_Texture* bossWarningText = NULL; // Texture cho văn bản "Boss Incoming!"
    SDL_Rect bossWarningRect;            // Vị trí và kích thước văn bản cảnh báo
    Uint32 bossWarningStartTime = 0;     // Thời gian bắt đầu hiển thị cảnh báo
    const Uint32 bossWarningDuration = 3000; // Thời gian hiển thị cảnh báo (3 giây)

    // Thiết lập hiệu ứng nền di chuyển
    int backgroundY = 0;                // Vị trí Y hiện tại của nền
    const int backgroundSpeed = 2;      // Tốc độ di chuyển nền (pixel mỗi frame)
    int backgroundHeight = HEIGHT;      // Chiều cao nền mặc định
    SDL_Surface* bgSurface = IMG_Load("anh/anh.jpg"); // Tải ảnh nền để lấy chiều cao thực tế
    if (bgSurface) {
        backgroundHeight = bgSurface->h; // Cập nhật chiều cao thực tế của ảnh nền
        SDL_FreeSurface(bgSurface);      // Giải phóng surface
    }

    // Phát nhạc nền khi bắt đầu chơi game
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(gGameMusic, -1); // Lặp vô hạn
    }

    bool is_quit = false; // Biến kiểm soát vòng lặp game

    // Vòng lặp chính của game
    while (!is_quit) {
        // Xử lý sự kiện từ người chơi
        while (SDL_PollEvent(&gEvent) != 0) {
            if (gEvent.type == SDL_QUIT) { // Thoát game nếu người dùng đóng cửa sổ
                is_quit = true;
            }
            P_player1.HandleEvent(gEvent, gRender); // Xử lý di chuyển và bắn đạn cho P1
            if (numPlayers == 2) P_player2.HandleEvent(gEvent, gRender); // Xử lý cho P2 nếu có
        }

        // Kiểm tra điều kiện thua game (máu của một trong hai người chơi về 0)
        if (!P_player1.IsAlive() || (numPlayers == 2 && !P_player2.IsAlive())) {
            SDL_Color loseColor = {255, 0, 0, 255}; // Màu đỏ cho văn bản "YOU LOSE"
            SDL_Texture* loseTexture = loadTextTexture("YOU LOSE", loseColor, gRender);
            if (loseTexture) {
                int w, h;
                SDL_QueryTexture(loseTexture, NULL, NULL, &w, &h); // Lấy kích thước văn bản
                SDL_Rect loseRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h}; // Đặt văn bản giữa màn hình
                SDL_RenderClear(gRender); // Xóa màn hình
                SDL_Rect bgRect1 = {0, backgroundY - backgroundHeight, WIDTH, backgroundHeight};
                SDL_Rect bgRect2 = {0, backgroundY, WIDTH, backgroundHeight};
                SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect1); // Vẽ nền lặp
                SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect2);
                SDL_RenderCopy(gRender, loseTexture, NULL, &loseRect); // Vẽ "YOU LOSE"
                SDL_RenderPresent(gRender); // Hiển thị màn hình thua

                // Dừng nhạc game và phát âm thanh thua
                Mix_HaltMusic();
                Mix_PlayChannel(-1, gLoseSound, 0);
                SDL_Delay(2000); // Chờ 2 giây để âm thanh phát xong
                SDL_DestroyTexture(loseTexture); // Giải phóng texture
            }
            std::cout << "Game Over: One of the players died!" << std::endl;
            is_quit = true; // Thoát vòng lặp game
            break;
        }

        Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại
        bool only_enemy1 = (current_time - game_start_time < 10000); // Chỉ sinh Enemy thường trong 10s đầu

        // Đếm số lượng kẻ thù hiện tại
        int shootingEnemiesCount = 0;
        int movingEnemiesCount = 0;
        for (auto enemy : enemies) {
            if (dynamic_cast<AdvancedEnemy*>(enemy) || dynamic_cast<Boss*>(enemy)) {
                shootingEnemiesCount++; // Đếm kẻ thù bắn (AdvancedEnemy hoặc Boss)
            } else if (dynamic_cast<Enemy*>(enemy)) {
                movingEnemiesCount++; // Đếm kẻ thù di chuyển (Enemy thường)
            }
        }

        // Sinh boss khi điểm >= 3000 và chưa có boss
        if (!boss_spawned && score >= 3000 && shootingEnemiesCount < MAX_SHOOTING_ENEMIES) {
            Boss* new_boss = new Boss(); // Tạo đối tượng boss mới
            if (new_boss->LoadImg("quaivat/boss.png", gRender)) {
                new_boss->SetPosition(WIDTH / 2 - 50, 50); // Đặt boss giữa màn hình phía trên
                enemies.push_back(new_boss); // Thêm boss vào danh sách kẻ thù
                boss_spawned = true; // Đánh dấu boss đã xuất hiện
                std::cout << "Boss spawned!" << std::endl;

                // Hiển thị cảnh báo "Boss Incoming!"
                SDL_Color warningColor = {255, 0, 0, 255}; // Màu đỏ cho cảnh báo
                bossWarningText = loadTextTexture("Boss Incoming!", warningColor, gRender);
                int w, h;
                SDL_QueryTexture(bossWarningText, NULL, NULL, &w, &h); // Lấy kích thước văn bản
                bossWarningRect = {WIDTH/2 - w/2, HEIGHT/4, w, h}; // Đặt giữa màn hình, cách đỉnh 1/4
                bossWarningStartTime = current_time; // Ghi lại thời gian bắt đầu cảnh báo
            } else {
                std::cout << "Failed to load boss image!" << std::endl;
                delete new_boss; // Xóa boss nếu tải ảnh thất bại
            }
        }

        // Sinh ngẫu nhiên 1 đến 3 kẻ thù mỗi lần khi đủ thời gian
        if (current_time - last_spawn_time >= spawn_delay) {
            int numEnemiesToSpawn = rand() % 3 + 1; // Sinh ngẫu nhiên từ 1 đến 3 kẻ thù
            for (int i = 0; i < numEnemiesToSpawn; i++) { // Vòng lặp để sinh từng con quái
                if (only_enemy1 || (!only_enemy1 && rand() % 2 == 0)) {
                    // Sinh Enemy thường nếu còn chỗ
                    if (movingEnemiesCount < MAX_MOVING_ENEMIES) {
                        Enemy* new_enemy = new Enemy(); // Tạo Enemy thường mới
                        if (new_enemy->LoadImg("quaivat/1.png", gRender)) {
                            int x = rand() % (WIDTH - 150); // Vị trí X ngẫu nhiên trên màn hình
                            new_enemy->SetPosition(x, -150); // Đặt Enemy ngoài màn hình phía trên
                            enemies.push_back(new_enemy); // Thêm vào danh sách kẻ thù
                            movingEnemiesCount++; // Tăng số lượng Enemy thường
                            std::cout << "Spawned a regular enemy!" << std::endl;
                        } else {
                            std::cout << "Failed to load enemy image!" << std::endl;
                            delete new_enemy; // Xóa nếu tải ảnh thất bại
                        }
                    }
                } else {
                    // Sinh AdvancedEnemy nếu còn chỗ
                    if (shootingEnemiesCount < MAX_SHOOTING_ENEMIES) {
                        AdvancedEnemy* new_enemy = new AdvancedEnemy(); // Tạo AdvancedEnemy mới
                        if (new_enemy->LoadImg("quaivat/2.png", gRender)) {
                            int x = rand() % (WIDTH - 150); // Vị trí X ngẫu nhiên
                            new_enemy->SetPosition(x, 0); // Đặt AdvancedEnemy ở đỉnh màn hình
                            enemies.push_back(new_enemy); // Thêm vào danh sách kẻ thù
                            shootingEnemiesCount++; // Tăng số lượng kẻ thù bắn
                            std::cout << "Spawned an advanced enemy!" << std::endl;
                        } else {
                            std::cout << "Failed to load advanced enemy image!" << std::endl;
                            delete new_enemy; // Xóa nếu tải ảnh thất bại
                        }
                    }
                }
            }
            last_spawn_time = current_time; // Cập nhật thời gian sinh cuối cùng
        }

        // Cập nhật vị trí và trạng thái của người chơi
        P_player1.UpdatePosition(); // Cập nhật vị trí P1 dựa trên phím bấm
        P_player1.UpdatePows(); // Cập nhật vị trí các viên đạn của P1
        P_player1.CheckCollisionWithItems(items); // Kiểm tra va chạm giữa P1 và vật phẩm

        if (numPlayers == 2) { // Nếu chơi 2 người
            P_player2.UpdatePosition(); // Cập nhật vị trí P2
            P_player2.UpdatePows(); // Cập nhật đạn của P2
            P_player2.CheckCollisionWithItems(items); // Kiểm tra va chạm với vật phẩm
        }

        // Cập nhật trạng thái vật phẩm
        for (auto it = items.begin(); it != items.end();) {
            (*it)->Update(); // Cập nhật vị trí vật phẩm (rơi xuống)
            if (!(*it)->IsActive()) { // Nếu vật phẩm ra khỏi màn hình hoặc bị nhặt
                delete *it; // Xóa vật phẩm
                it = items.erase(it); // Xóa khỏi danh sách
            } else {
                ++it;
            }
        }

        // Cập nhật trạng thái kẻ thù
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->Update()) { // Nếu kẻ thù ra khỏi màn hình
                delete *it; // Xóa kẻ thù
                it = enemies.erase(it); // Xóa khỏi danh sách
            } else {
                AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*it);
                Boss* boss = dynamic_cast<Boss*>(*it);
                if (adv_enemy) {
                    adv_enemy->Shoot(gRender); // AdvancedEnemy bắn đạn
                } else if (boss) {
                    boss->Shoot(gRender); // Boss bắn đạn
                }
                // Xử lý va chạm giữa các kẻ thù
                for (auto other = enemies.begin(); other != enemies.end(); ++other) {
                    if (*it != *other) { // Không kiểm tra với chính nó
                        SDL_Rect rect1 = (*it)->GetRect();
                        SDL_Rect rect2 = (*other)->GetRect();
                        if (SDL_HasIntersection(&rect1, &rect2)) { // Nếu hai kẻ thù va chạm
                            AdvancedEnemy* adv1 = dynamic_cast<AdvancedEnemy*>(*it);
                            AdvancedEnemy* adv2 = dynamic_cast<AdvancedEnemy*>(*other);
                            if (adv1 && adv2) {
                                adv1->SetVelX(-adv1->GetVelX()); // Đổi hướng di chuyển ngang
                                adv2->SetVelX(-adv2->GetVelX());
                            }
                        }
                    }
                }
                ++it;
            }
        }

        // Xử lý va chạm giữa đạn của P1 và kẻ thù
        auto pows1 = P_player1.get_pows(); // Lấy danh sách đạn của P1
        for (auto it = pows1.begin(); it != pows1.end();) {
            bool hit = false; // Biến kiểm tra đạn có trúng kẻ thù không
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
                SDL_Rect pow_rect = (*it)->GetRect(); // Vị trí đạn
                SDL_Rect enemy_rect = (*enemy_it)->GetRect(); // Vị trí kẻ thù
                if ((*enemy_it)->IsAlive() && SDL_HasIntersection(&pow_rect, &enemy_rect)) { // Nếu đạn trúng kẻ thù
                    std::cout << "Player 1 Bullet hit enemy! Damage: " << (*it)->GetDamage() << std::endl;
                    (*enemy_it)->TakeDamage((*it)->GetDamage()); // Gây sát thương cho kẻ thù
                    hit = true; // Đánh dấu đạn đã trúng
                    if (!(*enemy_it)->IsAlive()) { // Nếu kẻ thù chết
                        Enemy* basic_enemy = dynamic_cast<Enemy*>(*enemy_it);
                        AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(*enemy_it);
                        Boss* boss = dynamic_cast<Boss*>(*enemy_it);
                        if (basic_enemy && !adv_enemy && !boss) {
                            score += 100; // Cộng 100 điểm cho Enemy thường
                        } else if (adv_enemy && !boss) {
                            score += 200; // Cộng 200 điểm cho AdvancedEnemy
                        } else if (boss) { // Khi giết boss
                            SDL_Color winColor = {255, 255, 0, 255}; // Màu vàng cho "YOU WIN"
                            SDL_Texture* winTexture = loadTextTexture("YOU WIN", winColor, gRender);
                            if (winTexture) {
                                int w, h;
                                SDL_QueryTexture(winTexture, NULL, NULL, &w, &h); // Lấy kích thước
                                SDL_Rect winRect = {WIDTH/2 - w/2, HEIGHT/2 - h/2, w, h}; // Đặt giữa màn hình
                                SDL_RenderClear(gRender);
                                SDL_Rect bgRect1 = {0, backgroundY - backgroundHeight, WIDTH, backgroundHeight};
                                SDL_Rect bgRect2 = {0, backgroundY, WIDTH, backgroundHeight};
                                SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect1); // Vẽ nền
                                SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect2);
                                SDL_RenderCopy(gRender, winTexture, NULL, &winRect); // Vẽ "YOU WIN"
                                SDL_RenderPresent(gRender);

                                // Dừng nhạc game và phát âm thanh thắng
                                Mix_HaltMusic();
                                Mix_PlayChannel(-1, gWinSound, 0);
                                SDL_Delay(2000); // Chờ 2 giây để âm thanh phát xong
                                SDL_DestroyTexture(winTexture); // Giải phóng texture
                            }
                            is_quit = true; // Thoát game khi thắng
                        }
                        // Xác suất 30% rơi vật phẩm khi kẻ thù chết
                        if (basic_enemy && !adv_enemy && rand() % 100 < 30) {
                            Item* fire_drop = new Item(); // Tạo vật phẩm BULLET_FIRE
                            if (fire_drop->LoadImg("items/1.jpg", gRender)) {
                                fire_drop->SetPosition(enemy_rect.x, enemy_rect.y); // Đặt tại vị trí kẻ thù
                                fire_drop->SetBulletType(BULLET_FIRE);
                                items.push_back(fire_drop); // Thêm vào danh sách vật phẩm
                            } else {
                                delete fire_drop;
                            }
                        } else if (adv_enemy && rand() % 100 < 30) {
                            Item* fast_drop = new Item(); // Tạo vật phẩm BULLET_FAST
                            if (fast_drop->LoadImg("items/2.jpg", gRender)) {
                                fast_drop->SetPosition(enemy_rect.x, enemy_rect.y);
                                fast_drop->SetBulletType(BULLET_FAST);
                                items.push_back(fast_drop);
                            } else {
                                delete fast_drop;
                            }
                        }
                        delete *enemy_it; // Xóa kẻ thù
                        enemy_it = enemies.erase(enemy_it); // Xóa khỏi danh sách
                    } else {
                        ++enemy_it;
                    }
                    break; // Thoát vòng lặp khi đạn trúng
                } else {
                    ++enemy_it;
                }
            }
            if (hit) {
                delete *it; // Xóa đạn nếu trúng kẻ thù
                it = pows1.erase(it); // Xóa khỏi danh sách đạn
            } else {
                ++it;
            }
        }
        P_player1.set_bullet_list(pows1); // Cập nhật danh sách đạn của P1

        // Xử lý va chạm giữa đạn của P2 và kẻ thù (nếu có 2 người chơi)
        if (numPlayers == 2) {
            auto pows2 = P_player2.get_pows(); // Lấy danh sách đạn của P2
            for (auto it = pows2.begin(); it != pows2.end();) {
                bool hit = false;
                for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect enemy_rect = (*enemy_it)->GetRect();
                    if ((*enemy_it)->IsAlive() && SDL_HasIntersection(&pow_rect, &enemy_rect)) {
                        std::cout << "Player 2 Bullet hit enemy! Damage: " << (*it)->GetDamage() << std::endl;
                        (*enemy_it)->TakeDamage((*it)->GetDamage()); // Gây sát thương
                        hit = true;
                        if (!(*enemy_it)->IsAlive()) { // Nếu kẻ thù chết
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
                                    SDL_RenderClear(gRender);
                                    SDL_Rect bgRect1 = {0, backgroundY - backgroundHeight, WIDTH, backgroundHeight};
                                    SDL_Rect bgRect2 = {0, backgroundY, WIDTH, backgroundHeight};
                                    SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect1);
                                    SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect2);
                                    SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                                    SDL_RenderPresent(gRender);

                                    Mix_HaltMusic();
                                    Mix_PlayChannel(-1, gWinSound, 0);
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
            P_player2.set_bullet_list(pows2); // Cập nhật danh sách đạn của P2
        }

        // Xử lý va chạm giữa người chơi và kẻ thù
        for (auto it = enemies.begin(); it != enemies.end();) {
            SDL_Rect player_rect1 = P_player1.GetRect(); // Vị trí P1
            SDL_Rect player_rect2 = P_player2.GetRect(); // Vị trí P2
            SDL_Rect enemy_rect = (*it)->GetRect(); // Vị trí kẻ thù
            if ((*it)->IsAlive()) {
                if (SDL_HasIntersection(&player_rect1, &enemy_rect)) { // P1 va chạm với kẻ thù
                    (*it)->TakeDamage(5); // Kẻ thù mất 5 máu
                    P_player1.TakeDamage(1); // P1 mất 1 máu
                }
                if (numPlayers == 2 && SDL_HasIntersection(&player_rect2, &enemy_rect)) { // P2 va chạm
                    (*it)->TakeDamage(5);
                    P_player2.TakeDamage(1);
                }
                if (!(*it)->IsAlive()) { // Nếu kẻ thù chết do va chạm
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
                            SDL_RenderClear(gRender);
                            SDL_Rect bgRect1 = {0, backgroundY - backgroundHeight, WIDTH, backgroundHeight};
                            SDL_Rect bgRect2 = {0, backgroundY, WIDTH, backgroundHeight};
                            SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect1);
                            SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect2);
                            SDL_RenderCopy(gRender, winTexture, NULL, &winRect);
                            SDL_RenderPresent(gRender);

                            Mix_HaltMusic();
                            Mix_PlayChannel(-1, gWinSound, 0);
                            SDL_Delay(2000);
                            SDL_DestroyTexture(winTexture);
                        }
                        is_quit = true;
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

        // Xử lý va chạm giữa đạn của kẻ thù và người chơi
        for (auto enemy : enemies) {
            AdvancedEnemy* adv_enemy = dynamic_cast<AdvancedEnemy*>(enemy);
            Boss* boss = dynamic_cast<Boss*>(enemy);
            if (adv_enemy) {
                auto& enemy_pows = adv_enemy->GetPows(); // Lấy danh sách đạn của AdvancedEnemy
                for (auto it = enemy_pows.begin(); it != enemy_pows.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect player_rect1 = P_player1.GetRect();
                    SDL_Rect player_rect2 = P_player2.GetRect();
                    if (SDL_HasIntersection(&pow_rect, &player_rect1)) { // Đạn trúng P1
                        P_player1.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else if (numPlayers == 2 && SDL_HasIntersection(&pow_rect, &player_rect2)) { // Đạn trúng P2
                        P_player2.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else if (pow_rect.y > HEIGHT) { // Đạn ra khỏi màn hình
                        delete *it;
                        it = enemy_pows.erase(it);
                    } else {
                        ++it;
                    }
                }
            } else if (boss) {
                auto& boss_pows = boss->GetPows(); // Lấy danh sách đạn của Boss
                for (auto it = boss_pows.begin(); it != boss_pows.end();) {
                    SDL_Rect pow_rect = (*it)->GetRect();
                    SDL_Rect player_rect1 = P_player1.GetRect();
                    SDL_Rect player_rect2 = P_player2.GetRect();
                    if (SDL_HasIntersection(&pow_rect, &player_rect1)) {
                        P_player1.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = boss_pows.erase(it);
                    } else if (numPlayers == 2 && SDL_HasIntersection(&pow_rect, &player_rect2)) {
                        P_player2.TakeDamage((*it)->GetDamage());
                        delete *it;
                        it = boss_pows.erase(it);
                    } else if (pow_rect.y > HEIGHT || pow_rect.y < 0 || pow_rect.x < 0 || pow_rect.x > WIDTH) { // Đạn ra ngoài biên
                        delete *it;
                        it = boss_pows.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }

        // Cập nhật vị trí nền di chuyển
        backgroundY += backgroundSpeed; // Di chuyển nền xuống
        if (backgroundY >= backgroundHeight) {
            backgroundY = 0; // Đặt lại khi nền ra khỏi màn hình
        }

        // Vẽ lại toàn bộ màn hình
        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255); // Đặt màu nền là trắng
        SDL_RenderClear(gRender); // Xóa màn hình

        SDL_Rect bgRect1 = {0, backgroundY - backgroundHeight, WIDTH, backgroundHeight}; // Vùng nền trên
        SDL_Rect bgRect2 = {0, backgroundY, WIDTH, backgroundHeight}; // Vùng nền dưới
        SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect1); // Vẽ nền lặp
        SDL_RenderCopy(gRender, gBackground.getTexture(), NULL, &bgRect2);

        // Vẽ các vật phẩm
        for (auto item : items) {
            item->Render(gRender); // Vẽ từng vật phẩm lên màn hình
        }

        // Vẽ đạn của P1
        for (auto pow : P_player1.get_pows()) {
            SDL_SetRenderDrawColor(gRender, 255, 165, 0, 255); // Màu cam cho đạn P1
            SDL_Rect pow_rect = pow->GetRect();
            SDL_RenderDrawRect(gRender, &pow_rect); // Vẽ viền quanh đạn
            pow->Render(gRender); // Vẽ đạn
        }
        // Vẽ đạn của P2 (nếu có)
        if (numPlayers == 2) {
            for (auto pow : P_player2.get_pows()) {
                SDL_SetRenderDrawColor(gRender, 0, 255, 0, 255); // Màu xanh cho đạn P2
                SDL_Rect pow_rect = pow->GetRect();
                SDL_RenderDrawRect(gRender, &pow_rect);
                pow->Render(gRender);
            }
        }

        // Vẽ kẻ thù
        for (auto enemy : enemies) {
            SDL_SetRenderDrawColor(gRender, 255, 0, 0, 255); // Màu đỏ cho kẻ thù
            SDL_Rect enemy_rect = enemy->GetRect();
            SDL_RenderDrawRect(gRender, &enemy_rect); // Vẽ viền quanh kẻ thù
            enemy->Render(gRender); // Vẽ kẻ thù
        }

        // Vẽ người chơi
        P_player1.Draw(gRender); // Vẽ P1 lên màn hình
        if (numPlayers == 2) P_player2.Draw(gRender); // Vẽ P2 nếu có

        // Hiển thị điểm số và máu
        SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng cho văn bản
        std::string scoreText = "Score: " + std::to_string(score); // Chuỗi điểm số
        SDL_Texture* scoreTexture = loadTextTexture(scoreText, textColor, gRender);
        int scoreHeight = 0;
        if (scoreTexture) {
            int w, h;
            SDL_QueryTexture(scoreTexture, NULL, NULL, &w, &h); // Lấy kích thước
            SDL_Rect scoreRect = {10, 10, w, h}; // Đặt điểm số ở góc trên bên trái
            SDL_RenderCopy(gRender, scoreTexture, NULL, &scoreRect); // Vẽ điểm số
            scoreHeight = h; // Lưu chiều cao để đặt các văn bản khác
            SDL_DestroyTexture(scoreTexture); // Giải phóng texture
        }

        std::string healthText1 = "P1 Health: " + std::to_string(P_player1.GetHealth()); // Chuỗi máu P1
        SDL_Texture* healthTexture1 = loadTextTexture(healthText1, textColor, gRender);
        if (healthTexture1) {
            int w, h;
            SDL_QueryTexture(healthTexture1, NULL, NULL, &w, &h);
            SDL_Rect healthRect1 = {10, 10 + scoreHeight + 5, w, h}; // Đặt dưới điểm số
            SDL_RenderCopy(gRender, healthTexture1, NULL, &healthRect1); // Vẽ máu P1
            SDL_DestroyTexture(healthTexture1);
        }

        if (numPlayers == 2) { // Nếu có P2
            std::string healthText2 = "P2 Health: " + std::to_string(P_player2.GetHealth()); // Chuỗi máu P2
            SDL_Texture* healthTexture2 = loadTextTexture(healthText2, textColor, gRender);
            if (healthTexture2) {
                int w, h;
                SDL_QueryTexture(healthTexture2, NULL, NULL, &w, &h);
                SDL_Rect healthRect2 = {10, 10 + scoreHeight + 30, w, h}; // Đặt dưới máu P1
                SDL_RenderCopy(gRender, healthTexture2, NULL, &healthRect2); // Vẽ máu P2
                SDL_DestroyTexture(healthTexture2);
            }
        }

        // Hiển thị cảnh báo boss nếu còn trong thời gian
        if (bossWarningText && (current_time - bossWarningStartTime < bossWarningDuration)) {
            SDL_RenderCopy(gRender, bossWarningText, NULL, &bossWarningRect); // Vẽ cảnh báo
        } else if (bossWarningText) {
            SDL_DestroyTexture(bossWarningText); // Giải phóng texture nếu hết thời gian
            bossWarningText = NULL;
        }

        SDL_RenderPresent(gRender); // Hiển thị toàn bộ frame lên màn hình
    }

    // Dừng nhạc game khi thoát
    Mix_HaltMusic();

    // Giải phóng tài nguyên
    if (bossWarningText) {
        SDL_DestroyTexture(bossWarningText); // Giải phóng texture cảnh báo boss
    }

    for (auto item : items) {
        delete item; // Xóa từng vật phẩm
    }
    items.clear(); // Xóa danh sách vật phẩm

    for (auto enemy : enemies) {
        delete enemy; // Xóa từng kẻ thù
    }
    enemies.clear(); // Xóa danh sách kẻ thù
}

// Hàm chính của chương trình
int main(int argc, char* argv[])
{
    // Khởi tạo game
    if (!InitData()) {
        std::cout << "Failed to initialize!" << std::endl;
        return -1; // Thoát nếu khởi tạo thất bại
    }

    // Tải ảnh nền
    if (!loadBackground()) {
        std::cout << "Failed to load background!" << std::endl;
        close(); // Giải phóng tài nguyên
        return -1; // Thoát nếu tải nền thất bại
    }

    bool quitGame = false; // Biến kiểm soát vòng lặp chính
    while (!quitGame) {
        int menuChoice = showMainMenu(gRender); // Hiển thị menu và lấy lựa chọn
        switch (menuChoice) {
            case 0: // Thoát game
                quitGame = true;
                break;
            case 1: // Chơi 1 người
                runGame(1);
                break;
            case 2: // Chơi 2 người
                runGame(2);
                break;
        }
    }

    close(); // Giải phóng tài nguyên và thoát
    return 0; // Kết thúc chương trình
}
