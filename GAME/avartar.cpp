/*
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
*/
#include "avartar.hpp"

AVARTAR::AVARTAR()
{
    player_texture = NULL;              // Khởi tạo texture là NULL
    player_rect = {0, 0, 150, 150};    // Đặt vị trí ban đầu (0, 0) và kích thước 150x150
    velX = 0;                           // Vận tốc ban đầu theo x = 0
    velY = 0;                           // Vận tốc ban đầu theo y = 0
    current_bullet_type = BULLET_NORMAL; // Mặc định sử dụng đạn thường
    last_shot_time = 0;
}

AVARTAR::~AVARTAR()
{
    if (player_texture != NULL) {       // Kiểm tra nếu texture tồn tại
        SDL_DestroyTexture(player_texture); // Giải phóng texture
        player_texture = NULL;          // Đặt lại về NULL để tránh lỗi
    }
}

bool AVARTAR::LoadImg(const char* name, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(name); // Tải ảnh từ đường dẫn
    if (surface == NULL) {              // Nếu tải thất bại
        std::cout << "Không thể load ảnh: " << name << " - " << IMG_GetError() << std::endl;
        return false;                   // Trả về false
    }
    player_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture
    SDL_SetTextureBlendMode(player_texture, SDL_BLENDMODE_BLEND); // Hỗ trợ độ trong suốt (alpha)
    SDL_FreeSurface(surface);           // Giải phóng surface sau khi dùng xong
    return (player_texture != NULL);    // Trả về true nếu texture được tạo thành công
}

void AVARTAR::SetPosition(int x, int y)
{
    player_rect.x = x;                  // Đặt tọa độ x
    player_rect.y = y;                  // Đặt tọa độ y
}

void AVARTAR::Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, player_texture, NULL, &player_rect); // Vẽ nhân vật lên renderer
}

void AVARTAR::HandleEvent(SDL_Event& e, SDL_Renderer* renderer)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) { // Nếu phím được nhấn (không lặp lại)
        switch (e.key.keysym.sym) {   // Kiểm tra phím nào được nhấn
            case SDLK_LEFT: velX = -1; break;  // Phím trái: di chuyển sang trái
            case SDLK_RIGHT: velX = 1; break;  // Phím phải: di chuyển sang phải
            case SDLK_UP: velY = -1; break;    // Phím lên: di chuyển lên trên
            case SDLK_DOWN: velY = 1; break;   // Phím xuống: di chuyển xuống dưới
            case SDLK_1:  // Phím "1": bắn đạn
                Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại (mili-giây)
                Uint32 delay = BULLET_NORMAL_DELAY;   // Delay mặc định (đạn thường)

                // Chọn delay dựa trên loại đạn hiện tại
                switch (current_bullet_type) {
                    case BULLET_NORMAL: delay = BULLET_NORMAL_DELAY; break; // Đạn thường: 500ms
                    case BULLET_FIRE: delay = BULLET_FIRE_DELAY; break;     // Đạn lửa: 700ms (chậm hơn)
                    case BULLET_FAST: delay = BULLET_FAST_DELAY; break;     // Đạn nhanh: 300ms (nhanh hơn)
                }

                if (current_time - last_shot_time >= delay) { // Nếu đã đủ thời gian delay
                    // Tạo một viên đạn mới động (dùng con trỏ để quản lý bộ nhớ)
                    POW* new_pow = new POW();  // Cấp phát động một đối tượng POW (đạn)
                    const char* bullet_path;   // Biến lưu đường dẫn đến hình ảnh đạn
                    int speed;                 // Biến lưu tốc độ của đạn
                    int width = 10, height = 20; // Kích thước mặc định của đạn (10x20 pixel)

                    // Chọn hình ảnh, tốc độ và kích thước dựa trên loại đạn hiện tại
                    switch (current_bullet_type) {
                        case BULLET_NORMAL:        // Đạn thường
                            bullet_path = "dan/1.png"; // Đường dẫn đến hình ảnh đạn thường
                            speed = BULLET_SPEED;      // Sử dụng hằng số BULLET_SPEED từ comfunc.h
                            width = 50;                // Chiều rộng
                            height = 50;               // Chiều cao
                            break;
                        case BULLET_FIRE:          // Đạn lửa
                            bullet_path = "dan/2.png"; // Đường dẫn đến hình ảnh đạn lửa
                            speed = BULLET_SPEED;      // Sử dụng hằng số BULLET_SPEED từ comfunc.h
                            width = 50;                // Chiều rộng
                            height = 50;               // Chiều cao
                            break;
                        case BULLET_FAST:          // Đạn nhanh
                            bullet_path = "dan/3.png"; // Đường dẫn đến hình ảnh đạn nhanh
                            speed = BULLET_SPEED * 2;  // Tốc độ gấp đôi so với BULLET_SPEED
                            width = 50;                 // Chiều rộng nhỏ hơn
                            height = 50;               // Chiều cao nhỏ hơn
                            break;
                    }
                if (new_pow->LoadImg(bullet_path, renderer)) { // Tải hình ảnh đạn
                    new_pow->SetSize(width, height); // Đặt kích thước đạn
                    new_pow->SetPosition(player_rect.x + player_rect.w / 3, player_rect.y); // Đặt vị trí đạn ở giữa nhân vật
                    new_pow->SetDirection(0, -speed); // Đặt hướng bay lên với tốc độ tương ứng
                    pows.push_back(new_pow);   // Thêm đạn vào danh sách
                    last_shot_time = current_time;
                } else {
                    std::cout << "Failed to load bullet image: " << IMG_GetError() << std::endl;
                    delete new_pow;            // Giải phóng nếu tải thất bại
                }
                break;
                }
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) { // Nếu phím được thả
        switch (e.key.keysym.sym) {     // Kiểm tra phím nào được thả
            case SDLK_LEFT:
            case SDLK_RIGHT: velX = 0; break; // Dừng di chuyển ngang
            case SDLK_UP:
            case SDLK_DOWN: velY = 0; break;  // Dừng di chuyển dọc
        }
    }
}

void AVARTAR::UpdatePows()
{
    for (auto& pow : pows) {            // Duyệt qua từng viên đạn
        pow->Update();                  // Cập nhật vị trí của đạn
    }
}

void AVARTAR::UpdatePosition()
{
    player_rect.x += velX;              // Cập nhật tọa độ x dựa trên vận tốc
    player_rect.y += velY;              // Cập nhật tọa độ y dựa trên vận tốc

    // Giới hạn nhân vật trong màn hình
    if (player_rect.x < 0) player_rect.x = 0; // Không cho ra khỏi cạnh trái
    if (player_rect.y < 0) player_rect.y = 0; // Không cho ra khỏi cạnh trên
    if (player_rect.x > WIDTH - player_rect.w) player_rect.x = WIDTH - player_rect.w; // Không cho ra khỏi cạnh phải
    if (player_rect.y > HEIGHT - player_rect.h) player_rect.y = HEIGHT - player_rect.h; // Không cho ra khỏi cạnh dưới
}

void AVARTAR::CheckCollisionWithItems(std::vector<Item*>& items)
{
    for (auto& item : items) {          // Duyệt qua từng vật phẩm
        if (!item->IsActive()) continue; // Bỏ qua nếu vật phẩm đã bị nhặt

        SDL_Rect item_rect = item->GetRect(); // Lấy hình chữ nhật của vật phẩm
        if (SDL_HasIntersection(&player_rect, &item_rect)) { // Kiểm tra va chạm giữa nhân vật và vật phẩm
            current_bullet_type = item->GetBulletType(); // Thay đổi loại đạn của nhân vật
            item->Deactivate();         // Đánh dấu vật phẩm đã bị nhặt (ẩn vật phẩm)
        }
    }

}
