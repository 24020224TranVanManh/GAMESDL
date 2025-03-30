
#include "avartar.hpp" // Include header của lớp AVARTAR để sử dụng định nghĩa lớp

// Constructor: Khởi tạo nhân vật với tham số player_id để phân biệt người chơi
AVARTAR::AVARTAR(int player_id) {
    this->player_id = player_id; // Gán ID người chơi (1 cho Player 1, 2 cho Player 2)
    player_texture = NULL; // Khởi tạo texture của nhân vật là NULL (chưa tải hình ảnh)
    player_rect = {0, 0, 150, 150}; // Khởi tạo hình chữ nhật với vị trí (0, 0) và kích thước 150x150
    velX = 0; // Vận tốc ban đầu theo trục X là 0
    velY = 0; // Vận tốc ban đầu theo trục Y là 0
    current_bullet_type = BULLET_NORMAL; // Đặt loại đạn mặc định là BULLET_NORMAL
    last_shot_time = 0; // Thời gian bắn đạn cuối cùng ban đầu là 0 (ms)
    health = 5; // Đặt máu ban đầu của nhân vật là 3
}

// Destructor: Giải phóng tài nguyên của nhân vật
AVARTAR::~AVARTAR() {
    if (player_texture != NULL) { // Kiểm tra xem texture của nhân vật đã được cấp phát chưa
        SDL_DestroyTexture(player_texture); // Giải phóng texture để tránh rò rỉ bộ nhớ
        player_texture = NULL; // Đặt lại con trỏ về NULL
    }
    // Giải phóng tất cả đạn trong danh sách pows
    for (auto pow : pows) { // Duyệt qua danh sách các viên đạn
        delete pow; // Giải phóng bộ nhớ của từng viên đạn
    }
    pows.clear(); // Xóa danh sách đạn để đảm bảo không còn con trỏ rác
}

// Hàm tải hình ảnh cho nhân vật
bool AVARTAR::LoadImg(const char* name, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(name); // Tải hình ảnh từ đường dẫn (name) thành surface
    if (surface == NULL) { // Nếu tải hình ảnh thất bại
        std::cout << "Không thể load ảnh: " << name << " - " << IMG_GetError() << std::endl; // In thông báo lỗi
        return false; // Trả về false để báo hiệu thất bại
    }
    player_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture để vẽ lên màn hình
    SDL_SetTextureBlendMode(player_texture, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn để hỗ trợ độ trong suốt (alpha)
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture
    return (player_texture != NULL); // Trả về true nếu texture được tạo thành công, false nếu thất bại
}

// Hàm đặt vị trí cho nhân vật
void AVARTAR::SetPosition(int x, int y) {
    player_rect.x = x; // Gán tọa độ X vào hình chữ nhật của nhân vật
    player_rect.y = y; // Gán tọa độ Y vào hình chữ nhật của nhân vật
}

// Hàm vẽ nhân vật lên màn hình
void AVARTAR::Draw(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, player_texture, NULL, &player_rect); // Sao chép texture của nhân vật lên màn hình tại vị trí player_rect
    // Tham số NULL nghĩa là sử dụng toàn bộ texture, không cắt xén
}

// Hàm xử lý sự kiện (di chuyển và bắn đạn) dựa trên bàn phím
void AVARTAR::HandleEvent(SDL_Event& e, SDL_Renderer* renderer) {
    if (e.type == SDL_KEYDOWN) { // Nếu có phím được nhấn xuống
        if (player_id == 1) { // Xử lý cho Player 1 (dùng phím mũi tên và số 1)
            switch (e.key.keysym.sym) { // Kiểm tra phím nào được nhấn
                case SDLK_LEFT: velX = -2; break; // Nhấn phím LEFT: di chuyển sang trái (vận tốc X âm)
                case SDLK_RIGHT: velX = 2; break; // Nhấn phím RIGHT: di chuyển sang phải (vận tốc X dương)
                case SDLK_UP: velY = -2; break;   // Nhấn phím UP: di chuyển lên trên (vận tốc Y âm)
                case SDLK_DOWN: velY = 2; break;  // Nhấn phím DOWN: di chuyển xuống dưới (vận tốc Y dương)
                case SDLK_1: // Nhấn phím 1 hoặc phím 1 trên bàn phím số để bắn đạn
                case SDLK_KP_0:
                    Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại (ms)
                    Uint32 delay = BULLET_NORMAL_DELAY; // Thời gian chờ mặc định giữa các lần bắn
                    switch (current_bullet_type) { // Điều chỉnh thời gian chờ dựa trên loại đạn
                        case BULLET_NORMAL: delay = BULLET_NORMAL_DELAY; break; // Đạn thường: dùng delay mặc định
                        case BULLET_FIRE: delay = BULLET_FIRE_DELAY; break;     // Đạn lửa: delay lâu hơn
                        case BULLET_FAST: delay = BULLET_FAST_DELAY; break;     // Đạn nhanh: delay ngắn hơn
                    }
                    if (current_time - last_shot_time >= delay) { // Nếu đủ thời gian chờ để bắn
                        POW* new_pow = new POW(); // Tạo một viên đạn mới
                        const char* bullet_path; // Đường dẫn hình ảnh của đạn
                        int speed, width = 50, height = 50, damage = 3; // Thông số của đạn: tốc độ, kích thước, sát thương
                        switch (current_bullet_type) { // Chọn hình ảnh và tốc độ dựa trên loại đạn
                            case BULLET_NORMAL:
                                bullet_path = "dan/1.png"; // Hình ảnh đạn thường
                                speed = BULLET_SPEED; // Tốc độ mặc định
                                break;
                            case BULLET_FIRE:
                                bullet_path = "dan/2.png"; // Hình ảnh đạn lửa
                                speed = BULLET_SPEED; // Tốc độ mặc định
                                break;
                            case BULLET_FAST:
                                bullet_path = "dan/3.png"; // Hình ảnh đạn nhanh
                                speed = BULLET_SPEED * 2; // Tốc độ gấp đôi
                                break;
                        }
                        if (new_pow->LoadImg(bullet_path, renderer)) { // Tải hình ảnh cho đạn
                            new_pow->SetSize(width, height); // Đặt kích thước đạn (50x50)
                            new_pow->SetPosition(player_rect.x + player_rect.w / 2 - width / 2, player_rect.y - height); // Đặt vị trí xuất phát từ giữa đỉnh nhân vật
                            new_pow->SetDamage(damage); // Đặt sát thương của đạn
                            new_pow->SetDirection(0, -speed); // Đặt hướng bắn lên trên (vận tốc Y âm)
                            pows.push_back(new_pow); // Thêm đạn vào danh sách
                            last_shot_time = current_time; // Cập nhật thời gian bắn cuối cùng
                            std::cout << "Player 1 Bullet fired! Type: " << current_bullet_type << std::endl; // In thông báo bắn đạn
                        } else {
                            std::cout << "Failed to load bullet image: " << bullet_path << std::endl; // In lỗi nếu tải thất bại
                            delete new_pow; // Giải phóng bộ nhớ nếu tải thất bại
                        }
                    }
                    break;
            }
        } else if (player_id == 2) { // Xử lý cho Player 2 (dùng phím WASD và Space)
            switch (e.key.keysym.sym) { // Kiểm tra phím nào được nhấn
                case SDLK_a: velX = -2; break; // Nhấn phím A: di chuyển sang trái (vận tốc X âm)
                case SDLK_d: velX = 2; break;  // Nhấn phím D: di chuyển sang phải (vận tốc X dương)
                case SDLK_w: velY = -2; break; // Nhấn phím W: di chuyển lên trên (vận tốc Y âm)
                case SDLK_s: velY = 2; break;  // Nhấn phím S: di chuyển xuống dưới (vận tốc Y dương)
                case SDLK_SPACE: // Nhấn phím Space để bắn đạn
                    Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại (ms)
                    Uint32 delay = BULLET_NORMAL_DELAY; // Thời gian chờ mặc định giữa các lần bắn
                    switch (current_bullet_type) { // Điều chỉnh thời gian chờ dựa trên loại đạn
                        case BULLET_NORMAL: delay = BULLET_NORMAL_DELAY; break; // Đạn thường
                        case BULLET_FIRE: delay = BULLET_FIRE_DELAY; break;     // Đạn lửa
                        case BULLET_FAST: delay = BULLET_FAST_DELAY; break;     // Đạn nhanh
                    }
                    if (current_time - last_shot_time >= delay) { // Nếu đủ thời gian chờ để bắn
                        POW* new_pow = new POW(); // Tạo một viên đạn mới
                        const char* bullet_path; // Đường dẫn hình ảnh của đạn
                        int speed, width = 50, height = 50, damage = 3; // Thông số của đạn
                        switch (current_bullet_type) { // Chọn hình ảnh và tốc độ dựa trên loại đạn
                            case BULLET_NORMAL:
                                bullet_path = "dan/1.png"; // Hình ảnh đạn thường
                                speed = BULLET_SPEED; // Tốc độ mặc định
                                break;
                            case BULLET_FIRE:
                                bullet_path = "dan/2.png"; // Hình ảnh đạn lửa
                                speed = BULLET_SPEED; // Tốc độ mặc định
                                break;
                            case BULLET_FAST:
                                bullet_path = "dan/3.png"; // Hình ảnh đạn nhanh
                                speed = BULLET_SPEED * 2; // Tốc độ gấp đôi
                                break;
                        }
                        if (new_pow->LoadImg(bullet_path, renderer)) { // Tải hình ảnh cho đạn
                            new_pow->SetSize(width, height); // Đặt kích thước đạn (50x50)
                            new_pow->SetPosition(player_rect.x + player_rect.w / 2 - width / 2, player_rect.y - height); // Đặt vị trí xuất phát
                            new_pow->SetDamage(damage); // Đặt sát thương của đạn
                            new_pow->SetDirection(0, -speed); // Đặt hướng bắn lên trên
                            pows.push_back(new_pow); // Thêm đạn vào danh sách
                            last_shot_time = current_time; // Cập nhật thời gian bắn cuối cùng
                            std::cout << "Player 2 Bullet fired! Type: " << current_bullet_type << std::endl; // In thông báo bắn đạn
                        } else {
                            std::cout << "Failed to load bullet image: " << bullet_path << std::endl; // In lỗi nếu tải thất bại
                            delete new_pow; // Giải phóng bộ nhớ nếu tải thất bại
                        }
                    }
                    break;
            }
        }
    } else if (e.type == SDL_KEYUP) { // Nếu phím được thả ra
        if (player_id == 1) { // Xử lý cho Player 1
            switch (e.key.keysym.sym) { // Kiểm tra phím nào được thả
                case SDLK_LEFT: // Thả phím LEFT
                case SDLK_RIGHT: velX = 0; break; // Dừng di chuyển ngang
                case SDLK_UP: // Thả phím UP
                case SDLK_DOWN: velY = 0; break; // Dừng di chuyển dọc
            }
        } else if (player_id == 2) { // Xử lý cho Player 2
            switch (e.key.keysym.sym) { // Kiểm tra phím nào được thả
                case SDLK_a: // Thả phím A
                case SDLK_d: velX = 0; break; // Dừng di chuyển ngang
                case SDLK_w: // Thả phím W
                case SDLK_s: velY = 0; break; // Dừng di chuyển dọc
            }
        }
    }
}

// Hàm cập nhật vị trí của các viên đạn
void AVARTAR::UpdatePows() {
    for (auto it = pows.begin(); it != pows.end();) { // Duyệt qua danh sách đạn
        (*it)->Update(); // Cập nhật vị trí của từng viên đạn
        SDL_Rect pow_rect = (*it)->GetRect(); // Lấy hình chữ nhật của đạn
        if (pow_rect.y + pow_rect.h < 0) { // Nếu đạn ra khỏi màn hình phía trên
            delete *it; // Giải phóng bộ nhớ của đạn
            it = pows.erase(it); // Xóa đạn khỏi danh sách
        } else {
            ++it; // Tiếp tục với viên đạn tiếp theo nếu chưa ra khỏi màn hình
        }
    }
}

// Hàm cập nhật vị trí của nhân vật dựa trên vận tốc
void AVARTAR::UpdatePosition() {
    player_rect.x += velX; // Cập nhật tọa độ X dựa trên vận tốc X
    player_rect.y += velY; // Cập nhật tọa độ Y dựa trên vận tốc Y

    // Giới hạn nhân vật trong phạm vi màn hình
    if (player_rect.x < 0) player_rect.x = 0; // Không cho ra khỏi biên trái
    if (player_rect.y < 0) player_rect.y = 0; // Không cho ra khỏi biên trên
    if (player_rect.x > WIDTH - player_rect.w) player_rect.x = WIDTH - player_rect.w; // Không cho ra khỏi biên phải
    if (player_rect.y > HEIGHT - player_rect.h) player_rect.y = HEIGHT - player_rect.h; // Không cho ra khỏi biên dưới
}

// Hàm kiểm tra va chạm giữa nhân vật và vật phẩm
void AVARTAR::CheckCollisionWithItems(std::vector<Item*>& items) {
    for (auto& item : items) { // Duyệt qua danh sách vật phẩm
        if (!item->IsActive()) continue; // Bỏ qua nếu vật phẩm không còn hoạt động

        SDL_Rect item_rect = item->GetRect(); // Lấy hình chữ nhật của vật phẩm
        if (SDL_HasIntersection(&player_rect, &item_rect)) { // Nếu nhân vật va chạm với vật phẩm
            current_bullet_type = item->GetBulletType(); // Cập nhật loại đạn của nhân vật từ vật phẩm
            item->Deactivate(); // Vô hiệu hóa vật phẩm (biến mất sau khi nhặt)
        }
    }
}

// Hàm xử lý khi nhân vật bị mất máu
void AVARTAR::TakeDamage(int damage) {
    health -= damage; // Giảm máu của nhân vật theo giá trị sát thương nhận được
    if (health < 0) health = 0; // Đảm bảo máu không âm
    std::cout << "Player " << player_id << " health: " << health << std::endl; // In trạng thái máu hiện tại
}
