/*
#include <iostream>
#include <SDL.h>
#include "Baseoject.h"
#include "comfunc.h"
#include "avartar.hpp"

Baseoject gBackground;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

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
    return success;
}

bool loadBackground()
{
    return gBackground.LoadImg("anh/anh.jpg", gRender);
}

void close()
{
    gBackground.free();
    SDL_DestroyRenderer(gRender);
    SDL_DestroyWindow(gWindow);
    gRender = NULL;
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
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

    bool is_quit = false;
    while (!is_quit) {
        while (SDL_PollEvent(&gEvent) != 0) {
            if (gEvent.type == SDL_QUIT) {
                is_quit = true;
            }
            P_player.HandleEvent(gEvent, gRender);
        }

        P_player.UpdatePosition(); // Cập nhật vị trí nhân vật
        P_player.UpdatePows();     // Cập nhật vị trí đạn

        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        SDL_RenderClear(gRender);
        gBackground.render(gRender, NULL); // Vẽ nền

        // Vẽ tất cả đạn
        for (auto pow : P_player.get_pows()) {
            pow->Render(gRender);
        }

        P_player.Draw(gRender); // Vẽ nhân vật
        SDL_RenderPresent(gRender);
    }

    close();
    return 0;
}*/
#include <iostream>      // Thư viện nhập/xuất chuẩn
#include <SDL.h>         // Thư viện SDL chính
#include "Baseoject.h"   // Lớp cơ sở cho đối tượng nền
#include "comfunc.h"     // Các định nghĩa chung
#include "avartar.hpp"   // Lớp nhân vật chính
#include "item.h"        // Lớp vật phẩm

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
}
