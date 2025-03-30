/*
#include "avartar.hpp"

// Constructor: Khởi tạo nhân vật
AVARTAR::AVARTAR()
{
    player_texture = NULL;
    player_rect = {0, 0, 150, 150};
    velX = 0;                           // Vận tốc ban đầu theo x = 0
    velY = 0;                           // Vận tốc ban đầu theo y = 0
    current_bullet_type = BULLET_NORMAL; // Mặc định sử dụng đạn thường
    last_shot_time = 0;                 // Khởi tạo thời gian bắn cuối cùng
    health = 3;                         // Máu ban đầu của nhân vật là 3
}

// Destructor: Giải phóng texture của nhân vật
AVARTAR::~AVARTAR()
{
    if (player_texture != NULL) {       // Kiểm tra nếu texture tồn tại
        SDL_DestroyTexture(player_texture); // Giải phóng texture
        player_texture = NULL;          // Đặt lại về NULL để tránh lỗi
    }
}

// Tải hình ảnh cho nhân vật
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

// Đặt vị trí cho nhân vật
void AVARTAR::SetPosition(int x, int y)
{
    player_rect.x = x;                  // Đặt tọa độ x
    player_rect.y = y;                  // Đặt tọa độ y
}

// Vẽ nhân vật lên màn hình
void AVARTAR::Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, player_texture, NULL, &player_rect); // Vẽ nhân vật lên renderer
}

// Xử lý sự kiện (di chuyển, bắn đạn)
void AVARTAR::HandleEvent(SDL_Event& e, SDL_Renderer* renderer)
{
    if (e.type == SDL_KEYDOWN) { // Nếu phím được nhấn
        switch (e.key.keysym.sym) {   // Kiểm tra phím nào được nhấn
            case SDLK_LEFT: velX = -1; break;  // Phím trái: di chuyển sang trái
            case SDLK_RIGHT: velX = 1; break;  // Phím phải: di chuyển sang phải
            case SDLK_UP: velY = -1; break;    // Phím lên: di chuyển lên trên
            case SDLK_DOWN: velY = 1; break;   // Phím xuống: di chuyển xuống dưới
            case SDLK_1:  // Phím "1" trên bàn phím chính
            case SDLK_KP_1:  // Phím "1" trên bàn phím số
                Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại (mili-giây)
                Uint32 delay = BULLET_NORMAL_DELAY;   // Delay mặc định (đạn thường)

                // Chọn delay dựa trên loại đạn hiện tại
                switch (current_bullet_type) {
                    case BULLET_NORMAL: delay = BULLET_NORMAL_DELAY; break; // Đạn thường
                    case BULLET_FIRE: delay = BULLET_FIRE_DELAY; break;     // Đạn lửa
                    case BULLET_FAST: delay = BULLET_FAST_DELAY; break;     // Đạn nhanh
                }

                if (current_time - last_shot_time >= delay) { // Nếu đã đủ thời gian delay
                    // Tạo một viên đạn mới động (dùng con trỏ để quản lý bộ nhớ)
                    POW* new_pow = new POW();  // Cấp phát động một đối tượng POW (đạn)
                    const char* bullet_path;   // Biến lưu đường dẫn đến hình ảnh đạn
                    int speed;                 // Biến lưu tốc độ của đạn
                    int width = 50, height = 50; // Kích thước ô vuông của đạn
                    int damage;
                    // Chọn hình ảnh, tốc độ và kích thước dựa trên loại đạn hiện tại
                    switch (current_bullet_type) {
                        case BULLET_NORMAL:        // Đạn thường
                            bullet_path = "dan/1.png";
                            speed = BULLET_SPEED;  // BULLET_SPEED = 5 (từ comfunc.h)
                            damage = 3;
                            width = 50;
                            height = 50;
                            break;
                        case BULLET_FIRE:          // Đạn lửa
                            bullet_path = "dan/2.png";
                            speed = BULLET_SPEED;
                            damage = 3;
                            width = 50;
                            height = 50;
                            break;
                        case BULLET_FAST:          // Đạn nhanh
                            bullet_path = "dan/3.png";
                            speed = BULLET_SPEED * 2; // Tốc độ gấp đôi
                            damage = 3;
                            width = 50;
                            height = 50;
                            break;
                    }
                    if (new_pow->LoadImg(bullet_path, renderer)) { // Tải hình ảnh đạn
                        new_pow->SetSize(width, height); // Đặt kích thước đạn
                        new_pow->SetPosition(player_rect.x + player_rect.w / 2 - width / 2, player_rect.y - height); // Đặt vị trí đạn ở giữa và phía trên nhân vật
                        new_pow->SetDamage(damage);
                        new_pow->SetDirection(0, -speed); // Đặt hướng bay lên với tốc độ tương ứng
                        pows.push_back(new_pow);   // Thêm đạn vào danh sách
                        last_shot_time = current_time;
                        std::cout << "Bullet fired! Type: " << current_bullet_type << ", Speed: " << speed << std::endl; // In thông báo để kiểm tra
                    } else {
                        std::cout << "Failed to load bullet image: " << bullet_path << " - " << IMG_GetError() << std::endl;
                        delete new_pow;            // Giải phóng nếu tải thất bại
                    }
                } else {
                    std::cout << "Cannot shoot yet! Time remaining: " << (delay - (current_time - last_shot_time)) << "ms" << std::endl;
                }
                break;
        }
    }
    else if (e.type == SDL_KEYUP) { // Nếu phím được thả
        switch (e.key.keysym.sym) {     // Kiểm tra phím nào được thả
            case SDLK_LEFT:
            case SDLK_RIGHT: velX = 0; break; // Dừng di chuyển ngang
            case SDLK_UP:
            case SDLK_DOWN: velY = 0; break;  // Dừng di chuyển dọc
        }
    }
}

// Cập nhật vị trí của các viên đạn
void AVARTAR::UpdatePows()
{
    for (auto& pow : pows) {            // Duyệt qua từng viên đạn
        pow->Update();                  // Cập nhật vị trí của đạn
    }
}

// Cập nhật vị trí của nhân vật
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

// Kiểm tra va chạm với vật phẩm
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

// Xử lý khi nhân vật bị mất máu
void AVARTAR::TakeDamage(int damage)
{
    health -= damage; // Giảm máu theo sát thương nhận được
    if (health < 0) health = 0; // Đảm bảo máu không âm
    std::cout << "Player health: " << health << std::endl; // In ra máu hiện tại để kiểm tra
}
*/
#include "avartar.hpp"

// Constructor: Khởi tạo nhân vật
AVARTAR::AVARTAR(int player_id)
{
    this->player_id = player_id;
    player_texture = NULL;
    player_rect = {0, 0, 150, 150};
    velX = 0;
    velY = 0;
    current_bullet_type = BULLET_NORMAL;
    last_shot_time = 0;
    health = 3;
}

// Destructor: Giải phóng texture của nhân vật và các viên đạn
AVARTAR::~AVARTAR()
{
    if (player_texture != NULL) {
        SDL_DestroyTexture(player_texture);
        player_texture = NULL;
    }
    // Giải phóng tất cả đạn trong pows
    for (auto pow : pows) {
        delete pow;
    }
    pows.clear();
}

// Tải hình ảnh cho nhân vật
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

// Đặt vị trí cho nhân vật
void AVARTAR::SetPosition(int x, int y)
{
    player_rect.x = x;
    player_rect.y = y;
}

// Vẽ nhân vật lên màn hình
void AVARTAR::Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, player_texture, NULL, &player_rect);
}

// Xử lý sự kiện (di chuyển, bắn đạn)
void AVARTAR::HandleEvent(SDL_Event& e, SDL_Renderer* renderer)
{
    if (e.type == SDL_KEYDOWN) {
        if (player_id == 1) { // Player 1: LEFT, RIGHT, UP, DOWN, 1
            switch (e.key.keysym.sym) {
                case SDLK_LEFT: velX = -1; break;
                case SDLK_RIGHT: velX = 1; break;
                case SDLK_UP: velY = -1; break;
                case SDLK_DOWN: velY = 1; break;
                case SDLK_1:
                case SDLK_KP_1:
                    Uint32 current_time = SDL_GetTicks();
                    Uint32 delay = BULLET_NORMAL_DELAY;
                    switch (current_bullet_type) {
                        case BULLET_NORMAL: delay = BULLET_NORMAL_DELAY; break;
                        case BULLET_FIRE: delay = BULLET_FIRE_DELAY; break;
                        case BULLET_FAST: delay = BULLET_FAST_DELAY; break;
                    }
                    if (current_time - last_shot_time >= delay) {
                        POW* new_pow = new POW();
                        const char* bullet_path;
                        int speed, width = 50, height = 50, damage = 3;
                        switch (current_bullet_type) {
                            case BULLET_NORMAL:
                                bullet_path = "dan/1.png"; speed = BULLET_SPEED; break;
                            case BULLET_FIRE:
                                bullet_path = "dan/2.png"; speed = BULLET_SPEED; break;
                            case BULLET_FAST:
                                bullet_path = "dan/3.png"; speed = BULLET_SPEED * 2; break;
                        }
                        if (new_pow->LoadImg(bullet_path, renderer)) {
                            new_pow->SetSize(width, height);
                            new_pow->SetPosition(player_rect.x + player_rect.w / 2 - width / 2, player_rect.y - height);
                            new_pow->SetDamage(damage);
                            new_pow->SetDirection(0, -speed);
                            pows.push_back(new_pow);
                            last_shot_time = current_time;
                            std::cout << "Player 1 Bullet fired! Type: " << current_bullet_type << std::endl;
                        } else {
                            std::cout << "Failed to load bullet image: " << bullet_path << std::endl;
                            delete new_pow;
                        }
                    }
                    break;
            }
        } else if (player_id == 2) { // Player 2: W, A, S, D, SPACE
            switch (e.key.keysym.sym) {
                case SDLK_a: velX = -1; break;
                case SDLK_d: velX = 1; break;
                case SDLK_w: velY = -1; break;
                case SDLK_s: velY = 1; break;
                case SDLK_SPACE:
                    Uint32 current_time = SDL_GetTicks();
                    Uint32 delay = BULLET_NORMAL_DELAY;
                    switch (current_bullet_type) {
                        case BULLET_NORMAL: delay = BULLET_NORMAL_DELAY; break;
                        case BULLET_FIRE: delay = BULLET_FIRE_DELAY; break;
                        case BULLET_FAST: delay = BULLET_FAST_DELAY; break;
                    }
                    if (current_time - last_shot_time >= delay) {
                        POW* new_pow = new POW();
                        const char* bullet_path;
                        int speed, width = 50, height = 50, damage = 3;
                        switch (current_bullet_type) {
                            case BULLET_NORMAL:
                                bullet_path = "dan/1.png"; speed = BULLET_SPEED; break;
                            case BULLET_FIRE:
                                bullet_path = "dan/2.png"; speed = BULLET_SPEED; break;
                            case BULLET_FAST:
                                bullet_path = "dan/3.png"; speed = BULLET_SPEED * 2; break;
                        }
                        if (new_pow->LoadImg(bullet_path, renderer)) {
                            new_pow->SetSize(width, height);
                            new_pow->SetPosition(player_rect.x + player_rect.w / 2 - width / 2, player_rect.y - height);
                            new_pow->SetDamage(damage);
                            new_pow->SetDirection(0, -speed);
                            pows.push_back(new_pow);
                            last_shot_time = current_time;
                            std::cout << "Player 2 Bullet fired! Type: " << current_bullet_type << std::endl;
                        } else {
                            std::cout << "Failed to load bullet image: " << bullet_path << std::endl;
                            delete new_pow;
                        }
                    }
                    break;
            }
        }
    } else if (e.type == SDL_KEYUP) {
        if (player_id == 1) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                case SDLK_RIGHT: velX = 0; break;
                case SDLK_UP:
                case SDLK_DOWN: velY = 0; break;
            }
        } else if (player_id == 2) {
            switch (e.key.keysym.sym) {
                case SDLK_a:
                case SDLK_d: velX = 0; break;
                case SDLK_w:
                case SDLK_s: velY = 0; break;
            }
        }
    }
}

// Cập nhật vị trí của các viên đạn
void AVARTAR::UpdatePows()
{
    for (auto it = pows.begin(); it != pows.end();) {
        (*it)->Update();
        SDL_Rect pow_rect = (*it)->GetRect();
        if (pow_rect.y + pow_rect.h < 0) { // Nếu đạn ra khỏi màn hình
            delete *it;
            it = pows.erase(it);
        } else {
            ++it;
        }
    }
}

// Cập nhật vị trí của nhân vật
void AVARTAR::UpdatePosition()
{
    player_rect.x += velX;
    player_rect.y += velY;

    if (player_rect.x < 0) player_rect.x = 0;
    if (player_rect.y < 0) player_rect.y = 0;
    if (player_rect.x > WIDTH - player_rect.w) player_rect.x = WIDTH - player_rect.w;
    if (player_rect.y > HEIGHT - player_rect.h) player_rect.y = HEIGHT - player_rect.h;
}

// Kiểm tra va chạm với vật phẩm
void AVARTAR::CheckCollisionWithItems(std::vector<Item*>& items)
{
    for (auto& item : items) {
        if (!item->IsActive()) continue;

        SDL_Rect item_rect = item->GetRect();
        if (SDL_HasIntersection(&player_rect, &item_rect)) {
            current_bullet_type = item->GetBulletType();
            item->Deactivate();
        }
    }
}

// Xử lý khi nhân vật bị mất máu
void AVARTAR::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
    std::cout << "Player " << player_id << " health: " << health << std::endl;
}
