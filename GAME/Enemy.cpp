/*
#include "Enemy.h"
#include <cmath>

// Constructor cho Enemy
Enemy::Enemy()
{
    enemy_rect = {0, 0, 50, 50};
    posY = 0.0f;
    velY = 3;
    enemy_texture = NULL;
    health = 3;
}

Enemy::~Enemy()
{
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
}

bool Enemy::Update()
{
    posY += velY;
    enemy_rect.y = static_cast<int>(posY);
    if (enemy_rect.y > HEIGHT) {
        return false;
    }
    return true;
}

void Enemy::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);
}

bool Enemy::LoadImg(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Không thể load ảnh quái: " << IMG_GetError() << std::endl;
        return false;
    }
    enemy_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(enemy_texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return (enemy_texture != NULL);
}

void Enemy::SetPosition(int x, int y)
{
    enemy_rect.x = x;
    posY = static_cast<float>(y);
    enemy_rect.y = y;
}

void Enemy::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
    std::cout << "Enemy health: " << health << std::endl;
}

// AdvancedEnemy
AdvancedEnemy::AdvancedEnemy()
{
    enemy_rect = {0, 0, 50, 50};
    velY = 0;
    posX = 0.0f;
    velX = 0.5f;
    enemy_texture = NULL;
    health = 5;
    last_shot_time = 0;
}

AdvancedEnemy::~AdvancedEnemy()
{
    for (auto pow : enemy_pows) {
        delete pow;
    }
    enemy_pows.clear();
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
}

bool AdvancedEnemy::Update()
{
    posX += velX;
    enemy_rect.x = static_cast<int>(posX);
    if (enemy_rect.x <= 0 || enemy_rect.x >= WIDTH - enemy_rect.w) {
        velX = -velX;
    }

    for (auto& pow : enemy_pows) {
        pow->Update();
    }

    return true;
}

void AdvancedEnemy::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);
    for (auto pow : enemy_pows) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 128); // Màu tím, alpha = 128 (mờ)
        SDL_Rect pow_rect = pow->GetRect();
        SDL_RenderDrawRect(renderer, &pow_rect); // Vẽ hình chữ nhật bao quanh đạn
        pow->Render(renderer);
    }
}

void AdvancedEnemy::Shoot(SDL_Renderer* renderer)
{
    Uint32 current_time = SDL_GetTicks();
    const Uint32 shoot_delay = 3500;
    if (current_time - last_shot_time >= shoot_delay) {
        POW* new_pow = new POW();
        if (new_pow->LoadImg("dan/4.png", renderer)) {
            new_pow->SetSize(30, 30);
            new_pow->SetPosition(enemy_rect.x + enemy_rect.w / 2 - 15, enemy_rect.y + enemy_rect.h);
            new_pow->SetDamage(1);
            new_pow->SetDirection(0, 0.5f);
            enemy_pows.push_back(new_pow);
            last_shot_time = current_time;
            std::cout << "AdvancedEnemy fired a bullet!" << std::endl;
        } else {
            std::cout << "Failed to load enemy bullet image!" << std::endl;
            delete new_pow;
        }
    }
}

void AdvancedEnemy::SetPosition(int x, int y)
{
    posX = static_cast<float>(x);
    enemy_rect.x = x;
    posY = static_cast<float>(y);
    enemy_rect.y = y;
}

// Boss
Boss::Boss()
{
    enemy_rect = {0, 0, 100, 100}; // Kích thước lớn hơn
    velY = 0;
    posX = 0.0f;
    velX = 0.5f; // Di chuyển ngang
    enemy_texture = NULL;
    health = 20; // Máu cao hơn
    last_shot_time = 0;
}

Boss::~Boss()
{
    for (auto pow : boss_pows) {
        delete pow;
    }
    boss_pows.clear();
    if (enemy_texture != NULL) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
}

bool Boss::Update()
{
    posX += velX;
    enemy_rect.x = static_cast<int>(posX);
    if (enemy_rect.x <= 0 || enemy_rect.x >= WIDTH - enemy_rect.w) {
        velX = -velX; // Đổi hướng khi chạm biên
    }

    for (auto& pow : boss_pows) {
        pow->Update();
    }

    return true;
}

void Boss::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect);
    for (auto pow : boss_pows) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128); // Màu đỏ, alpha = 128 (mờ)
        SDL_Rect pow_rect = pow->GetRect();
        SDL_RenderDrawRect(renderer, &pow_rect); // Vẽ hình chữ nhật bao quanh đạn
        pow->Render(renderer);
    }
}

void Boss::Shoot(SDL_Renderer* renderer)
{
    Uint32 current_time = SDL_GetTicks();
    const Uint32 shoot_delay = 3000; // Bắn mỗi 2 giây
    if (current_time - last_shot_time >= shoot_delay) {
        // Bắn đạn về 5 hướng (góc 0, 45, 90, 135, 180 độ)
        float angles[] = {0, M_PI/4, M_PI/2, 3*M_PI/4, M_PI}; // Đơn vị radian
        for (float angle : angles) {
            POW* new_pow = new POW();
            if (new_pow->LoadImg("dan/1.png", renderer)) {
                new_pow->SetSize(20, 20);
                new_pow->SetPosition(enemy_rect.x + enemy_rect.w / 2 - 10, enemy_rect.y + enemy_rect.h / 2);
                new_pow->SetDamage(2);
                float speed = 0.5f; // Tốc độ đạn
                new_pow->SetDirection(speed * cos(angle), speed * sin(angle)); // Tính hướng dựa trên góc
                boss_pows.push_back(new_pow);
            } else {
                delete new_pow;
            }
        }
        last_shot_time = current_time;
        std::cout << "Boss fired bullets in multiple directions!" << std::endl;
    }
}

void Boss::SetPosition(int x, int y)
{
    posX = static_cast<float>(x);
    enemy_rect.x = x;
    posY = static_cast<float>(y);
    enemy_rect.y = y;
}
*/
#include "Enemy.h" // Include header của lớp Enemy để sử dụng định nghĩa lớp
#include <cmath>   // Thư viện toán học để tính toán góc bắn (cos, sin) cho Boss

// Constructor mặc định cho Enemy cơ bản
Enemy::Enemy() {
    enemy_rect = {0, 0, 50, 50}; // Khởi tạo hình chữ nhật với vị trí (0, 0) và kích thước 50x50
    posY = 0.0f; // Vị trí thực tế Y ban đầu là 0 (dùng float để chính xác)
    velY = 3;    // Tốc độ rơi mặc định là 3 (di chuyển xuống dưới)
    enemy_texture = NULL; // Texture ban đầu là NULL (chưa tải hình ảnh)
    health = 3;  // Đặt máu ban đầu là 3
}

// Destructor của Enemy
Enemy::~Enemy() {
    if (enemy_texture != NULL) { // Kiểm tra xem texture đã được cấp phát chưa
        SDL_DestroyTexture(enemy_texture); // Giải phóng texture để tránh rò rỉ bộ nhớ
        enemy_texture = NULL; // Đặt lại con trỏ về NULL
    }
}

// Hàm cập nhật vị trí của Enemy cơ bản
bool Enemy::Update() {
    posY += velY; // Tăng vị trí thực tế Y dựa trên tốc độ rơi
    enemy_rect.y = static_cast<int>(posY); // Chuyển đổi vị trí Y từ float sang int để gán vào SDL_Rect
    if (enemy_rect.y > HEIGHT) { // Nếu Enemy rơi ra khỏi màn hình dưới
        return false; // Trả về false để báo hiệu Enemy cần bị xóa
    }
    return true; // Trả về true nếu Enemy vẫn còn trong màn hình
}

// Hàm vẽ Enemy lên màn hình
void Enemy::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect); // Sao chép texture của Enemy lên màn hình
    // Tham số NULL nghĩa là sử dụng toàn bộ texture, không cắt xén
}

// Hàm tải hình ảnh cho Enemy
bool Enemy::LoadImg(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path); // Tải hình ảnh từ đường dẫn (path) thành surface
    if (!surface) { // Nếu tải hình ảnh thất bại
        std::cout << "Không thể load ảnh quái: " << IMG_GetError() << std::endl; // In thông báo lỗi
        return false; // Trả về false để báo hiệu thất bại
    }
    enemy_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture
    SDL_SetTextureBlendMode(enemy_texture, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn để hỗ trợ độ trong suốt
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture
    return (enemy_texture != NULL); // Trả về true nếu texture được tạo thành công, false nếu thất bại
}

// Hàm đặt vị trí cho Enemy
void Enemy::SetPosition(int x, int y) {
    enemy_rect.x = x; // Gán tọa độ X vào hình chữ nhật
    posY = static_cast<float>(y); // Gán vị trí thực tế Y (dùng float để chính xác)
    enemy_rect.y = y; // Gán tọa độ Y vào hình chữ nhật
}

// Hàm xử lý khi Enemy bị mất máu
void Enemy::TakeDamage(int damage) {
    health -= damage; // Giảm máu theo giá trị sát thương nhận được
    if (health < 0) health = 0; // Đảm bảo máu không âm
    std::cout << "Enemy health: " << health << std::endl; // In trạng thái máu hiện tại
}

// Constructor mặc định cho AdvancedEnemy
AdvancedEnemy::AdvancedEnemy() {
    enemy_rect = {0, 0, 50, 50}; // Khởi tạo hình chữ nhật với vị trí (0, 0) và kích thước 50x50
    velY = 0; // Không di chuyển dọc (chỉ di chuyển ngang)
    posX = 0.0f; // Vị trí thực tế X ban đầu là 0 (dùng float để chính xác)
    velX = 0.5f; // Tốc độ di chuyển ngang mặc định là 0.5
    enemy_texture = NULL; // Texture ban đầu là NULL (chưa tải hình ảnh)
    health = 5;  // Đặt máu ban đầu là 5 (cao hơn Enemy cơ bản)
    last_shot_time = 0; // Thời gian bắn đạn cuối cùng ban đầu là 0 (ms)
}

// Destructor của AdvancedEnemy
AdvancedEnemy::~AdvancedEnemy() {
    for (auto pow : enemy_pows) { // Duyệt qua danh sách đạn của AdvancedEnemy
        delete pow; // Giải phóng bộ nhớ của từng viên đạn
    }
    enemy_pows.clear(); // Xóa danh sách đạn để đảm bảo không còn con trỏ rác
    if (enemy_texture != NULL) { // Kiểm tra xem texture đã được cấp phát chưa
        SDL_DestroyTexture(enemy_texture); // Giải phóng texture
        enemy_texture = NULL; // Đặt lại con trỏ về NULL
    }
}

// Hàm cập nhật vị trí của AdvancedEnemy
bool AdvancedEnemy::Update() {
    posX += velX; // Tăng vị trí thực tế X dựa trên vận tốc ngang
    enemy_rect.x = static_cast<int>(posX); // Chuyển đổi vị trí X từ float sang int để gán vào SDL_Rect
    if (enemy_rect.x <= 0 || enemy_rect.x >= WIDTH - enemy_rect.w) { // Nếu chạm biên trái hoặc phải của màn hình
        velX = -velX; // Đổi hướng di chuyển ngang
    }
    for (auto& pow : enemy_pows) { // Duyệt qua danh sách đạn
        pow->Update(); // Cập nhật vị trí của từng viên đạn
    }
    return true; // Trả về true vì AdvancedEnemy không ra khỏi màn hình theo chiều dọc
}

// Hàm vẽ AdvancedEnemy lên màn hình
void AdvancedEnemy::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect); // Sao chép texture của AdvancedEnemy lên màn hình
    for (auto pow : enemy_pows) { // Duyệt qua danh sách đạn
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn để hỗ trợ độ trong suốt
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 128); // Đặt màu tím với alpha 128 (nửa trong suốt) cho khung đạn
        SDL_Rect pow_rect = pow->GetRect(); // Lấy hình chữ nhật của đạn
        SDL_RenderDrawRect(renderer, &pow_rect); // Vẽ khung bao quanh đạn
        pow->Render(renderer); // Vẽ đạn lên màn hình
    }
}

// Hàm bắn đạn của AdvancedEnemy
void AdvancedEnemy::Shoot(SDL_Renderer* renderer) {
    Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại (ms)
    const Uint32 shoot_delay = 3500; // Thời gian chờ giữa các lần bắn là 3.5 giây
    if (current_time - last_shot_time >= shoot_delay) { // Nếu đủ thời gian chờ để bắn
        POW* new_pow = new POW(); // Tạo một viên đạn mới
        if (new_pow->LoadImg("dan/4.png", renderer)) { // Tải hình ảnh cho đạn
            new_pow->SetSize(30, 30); // Đặt kích thước đạn là 30x30
            new_pow->SetPosition(enemy_rect.x + enemy_rect.w / 2 - 15, enemy_rect.y + enemy_rect.h); // Đặt vị trí xuất phát từ giữa dưới của Enemy
            new_pow->SetDamage(1); // Đặt sát thương của đạn là 1
            new_pow->SetDirection(0, 0.5f); // Đặt hướng bắn xuống dưới (vận tốc Y dương)
            enemy_pows.push_back(new_pow); // Thêm đạn vào danh sách
            last_shot_time = current_time; // Cập nhật thời gian bắn cuối cùng
            std::cout << "AdvancedEnemy fired a bullet!" << std::endl; // In thông báo bắn đạn
        } else {
            std::cout << "Failed to load enemy bullet image!" << std::endl; // In lỗi nếu tải thất bại
            delete new_pow; // Giải phóng bộ nhớ nếu tải thất bại
        }
    }
}

// Hàm đặt vị trí cho AdvancedEnemy
void AdvancedEnemy::SetPosition(int x, int y) {
    posX = static_cast<float>(x); // Gán vị trí thực tế X (dùng float để chính xác)
    enemy_rect.x = x; // Gán tọa độ X vào hình chữ nhật
    posY = static_cast<float>(y); // Gán vị trí thực tế Y (dùng float để chính xác)
    enemy_rect.y = y; // Gán tọa độ Y vào hình chữ nhật
}

// Constructor mặc định cho Boss
Boss::Boss() {
    enemy_rect = {0, 0, 100, 100}; // Khởi tạo hình chữ nhật với vị trí (0, 0) và kích thước 100x100 (lớn hơn Enemy thường)
    velY = 0; // Không di chuyển dọc (chỉ di chuyển ngang)
    posX = 0.0f; // Vị trí thực tế X ban đầu là 0 (dùng float để chính xác)
    velX = 0.5f; // Tốc độ di chuyển ngang mặc định là 0.5
    enemy_texture = NULL; // Texture ban đầu là NULL (chưa tải hình ảnh)
    health = 40; // Đặt máu ban đầu là 20 (cao hơn Enemy thường)
    last_shot_time = 0; // Thời gian bắn đạn cuối cùng ban đầu là 0 (ms)
}

// Destructor của Boss
Boss::~Boss() {
    for (auto pow : boss_pows) { // Duyệt qua danh sách đạn của Boss
        delete pow; // Giải phóng bộ nhớ của từng viên đạn
    }
    boss_pows.clear(); // Xóa danh sách đạn để đảm bảo không còn con trỏ rác
    if (enemy_texture != NULL) { // Kiểm tra xem texture đã được cấp phát chưa
        SDL_DestroyTexture(enemy_texture); // Giải phóng texture
        enemy_texture = NULL; // Đặt lại con trỏ về NULL
    }
}

// Hàm cập nhật vị trí của Boss
bool Boss::Update() {
    posX += velX; // Tăng vị trí thực tế X dựa trên vận tốc ngang
    enemy_rect.x = static_cast<int>(posX); // Chuyển đổi vị trí X từ float sang int để gán vào SDL_Rect
    if (enemy_rect.x <= 0 || enemy_rect.x >= WIDTH - enemy_rect.w) { // Nếu chạm biên trái hoặc phải của màn hình
        velX = -velX; // Đổi hướng di chuyển ngang
    }
    for (auto& pow : boss_pows) { // Duyệt qua danh sách đạn
        pow->Update(); // Cập nhật vị trí của từng viên đạn
    }
    return true; // Trả về true vì Boss không ra khỏi màn hình theo chiều dọc
}

// Hàm vẽ Boss lên màn hình
void Boss::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, enemy_texture, NULL, &enemy_rect); // Sao chép texture của Boss lên màn hình
    for (auto pow : boss_pows) { // Duyệt qua danh sách đạn
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn để hỗ trợ độ trong suốt
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128); // Đặt màu đỏ với alpha 128 (nửa trong suốt) cho khung đạn
        SDL_Rect pow_rect = pow->GetRect(); // Lấy hình chữ nhật của đạn
        SDL_RenderDrawRect(renderer, &pow_rect); // Vẽ khung bao quanh đạn
        pow->Render(renderer); // Vẽ đạn lên màn hình
    }
}

// Hàm bắn đạn của Boss
void Boss::Shoot(SDL_Renderer* renderer) {
    Uint32 current_time = SDL_GetTicks(); // Lấy thời gian hiện tại (ms)
    const Uint32 shoot_delay = 3000; // Thời gian chờ giữa các lần bắn là 3 giây
    if (current_time - last_shot_time >= shoot_delay) { // Nếu đủ thời gian chờ để bắn
        float angles[] = {0, M_PI/4, M_PI/2, 3*M_PI/4, M_PI}; // Mảng các góc bắn (0°, 45°, 90°, 135°, 180°) tính bằng radian
        for (float angle : angles) { // Duyệt qua từng góc để bắn đạn
            POW* new_pow = new POW(); // Tạo một viên đạn mới
            if (new_pow->LoadImg("dan/1.png", renderer)) { // Tải hình ảnh cho đạn
                new_pow->SetSize(20, 20); // Đặt kích thước đạn là 20x20
                new_pow->SetPosition(enemy_rect.x + enemy_rect.w / 2 - 10, enemy_rect.y + enemy_rect.h / 2); // Đặt vị trí xuất phát từ giữa Boss
                new_pow->SetDamage(2); // Đặt sát thương của đạn là 2
                float speed = 0.5f; // Tốc độ đạn mặc định là 0.5
                new_pow->SetDirection(speed * cos(angle), speed * sin(angle)); // Tính vận tốc X và Y dựa trên góc (dùng cos và sin)
                boss_pows.push_back(new_pow); // Thêm đạn vào danh sách
            } else {
                delete new_pow; // Giải phóng bộ nhớ nếu tải thất bại
            }
        }
        last_shot_time = current_time; // Cập nhật thời gian bắn cuối cùng
        std::cout << "Boss fired bullets in multiple directions!" << std::endl; // In thông báo bắn đạn
    }
}

// Hàm đặt vị trí cho Boss
void Boss::SetPosition(int x, int y) {
    posX = static_cast<float>(x); // Gán vị trí thực tế X (dùng float để chính xác)
    enemy_rect.x = x; // Gán tọa độ X vào hình chữ nhật
    posY = static_cast<float>(y); // Gán vị trí thực tế Y (dùng float để chính xác)
    enemy_rect.y = y; // Gán tọa độ Y vào hình chữ nhật
}
