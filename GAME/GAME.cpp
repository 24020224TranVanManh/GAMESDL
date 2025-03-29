/*
#include <iostream>      // Thư viện nhập/xuất chuẩn
#include <SDL.h>         // Thư viện SDL chính
#include "Baseoject.h"   // Lớp cơ sở cho đối tượng nền
#include "comfunc.h"     // Các định nghĩa chung
#include "avartar.hpp"   // Lớp nhân vật chính
#include "item.h"        // Lớp vật phẩm
#include "enemy.h"       // Lớp quái
#include <cstdlib>       // Thư viện cho hàm rand() để tạo vị trí ngẫu nhiên

Baseoject gBackground;   // Đối tượng nền của game

bool InitData()
{
    bool success = true;            // Biến kiểm tra khởi tạo thành công
    int ret = SDL_Init(SDL_INIT_VIDEO); // Khởi tạo SDL với chế độ video
    if (ret < 0)                    // Nếu khởi tạo thất bại
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Cải thiện chất lượng render
    gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); // Tạo cửa sổ
    if (gWindow == NULL) {          // Nếu tạo cửa sổ thất bại
        success = false;
    } else {
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); // Tạo renderer với tăng tốc phần cứng
        if (gRender == NULL) success = false; // Nếu tạo renderer thất bại
        else {
            SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255); // Đặt màu nền trắng
        }
        int imgl = IMG_INIT_PNG;    // Khởi tạo hỗ trợ định dạng PNG
        if (!(IMG_Init(imgl) & imgl)) success = false; // Nếu khởi tạo thất bại
    }
    return success;                 // Trả về kết quả khởi tạo
}

bool loadBackground()
{
    return gBackground.LoadImg("anh/anh.jpg", gRender); // Tải ảnh nền
}

void close()
{
    gBackground.free();             // Giải phóng tài nguyên nền
    SDL_DestroyRenderer(gRender);   // Giải phóng renderer
    SDL_DestroyWindow(gWindow);     // Giải phóng cửa sổ
    gRender = NULL;                 // Đặt lại về NULL
    gWindow = NULL;                 // Đặt lại về NULL
    IMG_Quit();                     // Thoát SDL_image
    SDL_Quit();                     // Thoát SDL
}

int main(int argc, char* argv[])
{
    if (!InitData()) {              // Khởi tạo SDL và kiểm tra lỗi
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadBackground()) {        // Tải nền và kiểm tra lỗi
        std::cout << "Failed to load background!" << std::endl;
        return -1;
    }

    AVARTAR P_player;               // Tạo đối tượng nhân vật
    if (!P_player.LoadImg("nhanvat/left.png", gRender)) { // Tải ảnh nhân vật
        std::cout << "Failed to load player image!" << std::endl;
        return -1;
    }
    P_player.SetPosition(500, 500); // Đặt vị trí ban đầu cho nhân vật

    // Tạo danh sách vật phẩm
    std::vector<Item*> items;       // Vector lưu trữ các con trỏ tới vật phẩm

    // Tạo vật phẩm thay đổi đạn lửa
    Item* fire_item = new Item();   // Tạo vật phẩm mới
    if (fire_item->LoadImg("items/1.jpg", gRender)) { // Tải hình ảnh vật phẩm
        fire_item->SetPosition(200, 200); // Đặt vị trí vật phẩm tại (200, 200)
        fire_item->SetBulletType(BULLET_FIRE); // Đặt loại đạn mà vật phẩm cung cấp là đạn lửa
        items.push_back(fire_item);   // Thêm vật phẩm vào danh sách
    } else {
        std::cout << "Failed to load fire item image!" << std::endl;
        delete fire_item;             // Giải phóng nếu tải thất bại
    }

    // Tạo vật phẩm thay đổi đạn nhanh
    Item* fast_item = new Item();   // Tạo vật phẩm mới
    if (fast_item->LoadImg("items/2.jpg", gRender)) { // Tải hình ảnh vật phẩm
        fast_item->SetPosition(300, 200); // Đặt vị trí vật phẩm tại (300, 200)
        fast_item->SetBulletType(BULLET_FAST); // Đặt loại đạn mà vật phẩm cung cấp là đạn nhanh
        items.push_back(fast_item);   // Thêm vật phẩm vào danh sách
    } else {
        std::cout << "Failed to load fast item image!" << std::endl;
        delete fast_item;             // Giải phóng nếu tải thất bại
    }
    bool is_quit = false;           // Biến kiểm soát vòng lặp game

    while (!is_quit) {              // Vòng lặp chính của game
        while (SDL_PollEvent(&gEvent) != 0) { // Xử lý tất cả sự kiện
            if (gEvent.type == SDL_QUIT) { // Nếu nhấn nút thoát
                is_quit = true;
            }
            P_player.HandleEvent(gEvent, gRender); // Xử lý sự kiện cho nhân vật
        }
        P_player.UpdatePosition();  // Cập nhật vị trí nhân vật
        P_player.UpdatePows();      // Cập nhật vị trí tất cả đạn
        P_player.CheckCollisionWithItems(items); // Kiểm tra va chạm giữa nhân vật và vật phẩm
        auto pows = P_player.get_pows();
        P_player.set_bullet_list(pows); // Cập nhật lại danh sách đạn sau khi xóa

        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255); // Đặt màu nền trắng
        SDL_RenderClear(gRender);   // Xóa màn hình
        gBackground.render(gRender, NULL); // Vẽ nền

        // Vẽ tất cả vật phẩm
        for (auto item : items) {   // Duyệt qua danh sách vật phẩm
            item->Render(gRender);  // Vẽ từng vật phẩm lên màn hình
        }
        // Vẽ tất cả đạn
        for (auto pow : P_player.get_pows()) { // Duyệt qua danh sách đạn
            pow->Render(gRender);   // Vẽ từng viên đạn
        }

        P_player.Draw(gRender);     // Vẽ nhân vật
        SDL_RenderPresent(gRender); // Hiển thị mọi thứ lên màn hình
    }

    // Giải phóng vật phẩm
    for (auto item : items) {       // Duyệt qua danh sách vật phẩm
        delete item;                // Giải phóng từng vật phẩm
    }
    items.clear();                  // Xóa danh sách vật phẩm
    close();                        // Giải phóng tài nguyên và thoát
    return 0;
}*/
#include <iostream>      // Thư viện nhập/xuất chuẩn
#include <SDL.h>         // Thư viện SDL chính
#include "Baseoject.h"   // Lớp cơ sở cho đối tượng nền
#include "comfunc.h"     // Các định nghĩa chung
#include "avartar.hpp"   // Lớp nhân vật chính
#include "item.h"        // Lớp vật phẩm
#include "Enemy.h"       // Lớp quái vật
#include <cstdlib>       // Thư viện cho hàm rand() để tạo vị trí ngẫu nhiên

Baseoject gBackground;   // Đối tượng nền của game

// Hàm khởi tạo SDL và các thành phần cần thiết
bool InitData()
{
    bool success = true;            // Biến kiểm tra khởi tạo thành công
    int ret = SDL_Init(SDL_INIT_VIDEO); // Khởi tạo SDL với chế độ video
    if (ret < 0)                    // Nếu khởi tạo thất bại
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Cải thiện chất lượng render
    gWindow = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN); // Tạo cửa sổ
    if (gWindow == NULL) {          // Nếu tạo cửa sổ thất bại
        success = false;
    } else {
        gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED); // Tạo renderer với tăng tốc phần cứng
        if (gRender == NULL) success = false; // Nếu tạo renderer thất bại
        else {
            SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255); // Đặt màu nền trắng
        }
        int imgl = IMG_INIT_PNG;    // Khởi tạo hỗ trợ định dạng PNG
        if (!(IMG_Init(imgl) & imgl)) success = false; // Nếu khởi tạo thất bại
    }
    return success;                 // Trả về kết quả khởi tạo
}

// Tải hình ảnh nền cho game
bool loadBackground()
{
    return gBackground.LoadImg("anh/anh.jpg", gRender); // Tải ảnh nền
}

// Giải phóng tài nguyên và thoát SDL
void close()
{
    gBackground.free();             // Giải phóng tài nguyên nền
    SDL_DestroyRenderer(gRender);   // Giải phóng renderer
    SDL_DestroyWindow(gWindow);     // Giải phóng cửa sổ
    gRender = NULL;                 // Đặt lại về NULL
    gWindow = NULL;                 // Đặt lại về NULL
    IMG_Quit();                     // Thoát SDL_image
    SDL_Quit();                     // Thoát SDL
}

// Hàm chính của chương trình
int main(int argc, char* argv[])
{
    if (!InitData()) {              // Khởi tạo SDL và kiểm tra lỗi
        std::cout << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadBackground()) {        // Tải nền và kiểm tra lỗi
        std::cout << "Failed to load background!" << std::endl;
        return -1;
    }

    AVARTAR P_player;               // Tạo đối tượng nhân vật
    if (!P_player.LoadImg("nhanvat/left.png", gRender)) { // Tải ảnh nhân vật
        std::cout << "Failed to load player image!" << std::endl;
        return -1;
    }
    P_player.SetPosition(500, 500); // Đặt vị trí ban đầu cho nhân vật

    // Tạo danh sách vật phẩm
    std::vector<Item*> items;       // Vector lưu trữ các con trỏ tới vật phẩm

    // Tạo vật phẩm thay đổi đạn lửa
    Item* fire_item = new Item();   // Tạo vật phẩm mới
    if (fire_item->LoadImg("items/1.jpg", gRender)) { // Tải hình ảnh vật phẩm
        fire_item->SetPosition(200, 200); // Đặt vị trí vật phẩm tại (200, 200)
        fire_item->SetBulletType(BULLET_FIRE); // Đặt loại đạn mà vật phẩm cung cấp là đạn lửa
        items.push_back(fire_item);   // Thêm vật phẩm vào danh sách
    } else {
        std::cout << "Failed to load fire item image!" << std::endl;
        delete fire_item;             // Giải phóng nếu tải thất bại
    }

    // Tạo vật phẩm thay đổi đạn nhanh
    Item* fast_item = new Item();   // Tạo vật phẩm mới
    if (fast_item->LoadImg("items/2.jpg", gRender)) { // Tải hình ảnh vật phẩm
        fast_item->SetPosition(300, 200); // Đặt vị trí vật phẩm tại (300, 200)
        fast_item->SetBulletType(BULLET_FAST); // Đặt loại đạn mà vật phẩm cung cấp là đạn nhanh
        items.push_back(fast_item);   // Thêm vật phẩm vào danh sách
    } else {
        std::cout << "Failed to load fast item image!" << std::endl;
        delete fast_item;             // Giải phóng nếu tải thất bại
    }

    // Tạo danh sách quái vật
    std::vector<Enemy*> enemies;

    // Biến để kiểm soát thời gian tạo quái
    Uint32 last_spawn_time = 0;     // Thời gian lần cuối tạo quái
    const Uint32 spawn_delay = 2000; // Tạo quái mỗi 2 giây (2000ms)

    bool is_quit = false;           // Biến kiểm soát vòng lặp game

    while (!is_quit) {              // Vòng lặp chính của game
        while (SDL_PollEvent(&gEvent) != 0) { // Xử lý tất cả sự kiện
            if (gEvent.type == SDL_QUIT) { // Nếu nhấn nút thoát
                is_quit = true;
            }
            P_player.HandleEvent(gEvent, gRender); // Xử lý sự kiện cho nhân vật
        }

        // Kiểm tra nếu người chơi đã chết
        if (!P_player.IsAlive()) {
            std::cout << "Game Over: Player died!" << std::endl;
            is_quit = true;
            break;
        }

        // Tạo quái ngẫu nhiên theo thời gian
        Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại
        if (current_time - last_spawn_time >= spawn_delay) { // Nếu đã đủ thời gian delay
            Enemy* new_enemy = new Enemy(); // Tạo quái mới
            if (new_enemy->LoadImg("enemy/enemy.png", gRender)) {
                int x = rand() % (WIDTH - 150); // Vị trí X ngẫu nhiên (trừ đi chiều rộng quái)
                new_enemy->SetPosition(x, -150); // Xuất hiện phía trên màn hình
                enemies.push_back(new_enemy);   // Thêm quái vào danh sách
            } else {
                std::cout << "Failed to load enemy image!" << std::endl;
                delete new_enemy;
            }
            last_spawn_time = current_time; // Cập nhật thời gian tạo quái
        }

        P_player.UpdatePosition();  // Cập nhật vị trí nhân vật
        P_player.UpdatePows();      // Cập nhật vị trí tất cả đạn
        P_player.CheckCollisionWithItems(items); // Kiểm tra va chạm giữa nhân vật và vật phẩm

        // Cập nhật vị trí quái vật và xóa quái nếu ra khỏi map
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->Update()) { // Nếu quái ra khỏi map (Update trả về false)
                delete *it;         // Giải phóng quái
                it = enemies.erase(it); // Xóa quái khỏi danh sách
            } else {
                ++it;
            }
        }

        // Kiểm tra và xóa đạn nếu ra khỏi màn hình
        auto pows = P_player.get_pows(); // Lấy danh sách đạn
        for (auto it = pows.begin(); it != pows.end();) {
            SDL_Rect pow_rect = (*it)->GetRect();
            if (pow_rect.y + pow_rect.h < 0) { // Nếu đạn ra khỏi màn hình ở phía trên
                delete *it; // Giải phóng đạn
                it = pows.erase(it); // Xóa đạn khỏi danh sách
            } else {
                ++it;
            }
        }

        // Kiểm tra va chạm giữa đạn và quái
        for (auto it = pows.begin(); it != pows.end();) {
            bool hit = false;
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end();) {
                SDL_Rect pow_rect = (*it)->GetRect();
                SDL_Rect enemy_rect = (*enemy_it)->GetRect();
                if ((*enemy_it)->IsAlive() && SDL_HasIntersection(&pow_rect, &enemy_rect)) {
                    std::cout << "Bullet hit enemy!" << std::endl; // In thông báo để kiểm tra va chạm
                    (*enemy_it)->TakeDamage((*it)->GetDamage()); // Gây sát thương khi đạn trúng quái
                    hit = true;
                    if (!(*enemy_it)->IsAlive()) { // Nếu quái chết
                        delete *enemy_it; // Giải phóng quái
                        enemy_it = enemies.erase(enemy_it); // Xóa quái khỏi danh sách
                    } else {
                        ++enemy_it;
                    }
                    break;
                } else {
                    ++enemy_it;
                }
            }
            if (hit) {
                delete *it; // Giải phóng đạn
                it = pows.erase(it); // Xóa đạn khỏi danh sách
            } else {
                ++it;
            }
        }
        P_player.set_bullet_list(pows); // Cập nhật lại danh sách đạn

        // Kiểm tra va chạm giữa người chơi và quái
        for (auto it = enemies.begin(); it != enemies.end();) {
            SDL_Rect player_rect = P_player.GetRect();
            SDL_Rect enemy_rect = (*it)->GetRect();
            if ((*it)->IsAlive() && SDL_HasIntersection(&player_rect, &enemy_rect)) {
                (*it)->TakeDamage(9999); // Gây sát thương lớn để tiêu diệt quái ngay lập tức
                P_player.TakeDamage(1);  // Người chơi mất 1 máu khi va chạm
                if (!(*it)->IsAlive()) { // Nếu quái chết
                    delete *it;          // Giải phóng quái
                    it = enemies.erase(it); // Xóa quái khỏi danh sách
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }

        // Xóa quái đã chết (do đạn hoặc va chạm)
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->IsAlive()) {
                delete *it; // Giải phóng quái
                it = enemies.erase(it); // Xóa quái khỏi danh sách
            } else {
                ++it;
            }
        }

        // Vẽ mọi thứ lên màn hình
        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255); // Đặt màu nền trắng
        SDL_RenderClear(gRender);   // Xóa màn hình
        gBackground.render(gRender, NULL); // Vẽ nền

        // Vẽ tất cả vật phẩm
        for (auto item : items) {   // Duyệt qua danh sách vật phẩm
            item->Render(gRender);  // Vẽ từng vật phẩm lên màn hình
        }

        // Vẽ ô vuông bao quanh đạn (màu cam) và hình ảnh đạn
        for (auto pow : P_player.get_pows()) { // Duyệt qua danh sách đạn
            SDL_SetRenderDrawColor(gRender, 255, 165, 0, 255); // Màu cam (R=255, G=165, B=0)
            SDL_Rect pow_rect = pow->GetRect();
            SDL_RenderDrawRect(gRender, &pow_rect); // Vẽ ô vuông bao quanh đạn
            pow->Render(gRender); // Vẽ hình ảnh đạn
        }

        // Vẽ ô vuông bao quanh quái (màu đỏ) và hình ảnh quái
        for (auto enemy : enemies) {
            SDL_SetRenderDrawColor(gRender, 255, 0, 0, 255); // Màu đỏ (R=255, G=0, B=0)
            SDL_Rect enemy_rect = enemy->GetRect();
            SDL_RenderDrawRect(gRender, &enemy_rect); // Vẽ ô vuông bao quanh quái
            enemy->Render(gRender); // Vẽ hình ảnh quái
        }

        P_player.Draw(gRender);     // Vẽ nhân vật
        SDL_RenderPresent(gRender); // Hiển thị mọi thứ lên màn hình
    }

    // Giải phóng vật phẩm
    for (auto item : items) {       // Duyệt qua danh sách vật phẩm
        delete item;                // Giải phóng từng vật phẩm
    }
    items.clear();                  // Xóa danh sách vật phẩm

    // Giải phóng quái vật
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    close();                        // Giải phóng tài nguyên và thoát
    return 0;                       // Trả về 0 để báo hiệu chương trình kết thúc thành công
}
