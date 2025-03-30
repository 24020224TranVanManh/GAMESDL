
#include "item.h" // Include header của lớp Item để sử dụng định nghĩa lớp

// Constructor mặc định của lớp Item
Item::Item() {
    item_rect = {0, 0, 30, 30}; // Khởi tạo hình chữ nhật với vị trí (0, 0) và kích thước 30x30
    item_texture = NULL; // Texture ban đầu là NULL (chưa tải hình ảnh)
    bullet_type = BULLET_NORMAL; // Đặt loại đạn mặc định là BULLET_NORMAL
    is_active = true; // Đặt trạng thái hoạt động ban đầu là true (vật phẩm đang tồn tại)
    velY = 0.5f; // Tốc độ rơi mặc định là 0.5 (chậm để người chơi có thể nhặt)
    posY = 0.0f; // Vị trí thực tế Y ban đầu là 0 (dùng float để chính xác)
}

// Destructor của lớp Item
Item::~Item() {
    if (item_texture != NULL) { // Kiểm tra xem texture đã được cấp phát chưa
        SDL_DestroyTexture(item_texture); // Giải phóng texture để tránh rò rỉ bộ nhớ
        item_texture = NULL; // Đặt lại con trỏ về NULL
    }
}

// Hàm tải hình ảnh cho vật phẩm
bool Item::LoadImg(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path); // Tải hình ảnh từ đường dẫn (path) thành surface
    if (!surface) { // Nếu tải hình ảnh thất bại
        std::cout << "Không thể load ảnh vật phẩm: " << IMG_GetError() << std::endl; // In thông báo lỗi từ SDL_image
        return false; // Trả về false để báo hiệu thất bại
    }
    item_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture để sử dụng với renderer
    SDL_SetTextureBlendMode(item_texture, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn để hỗ trợ độ trong suốt (alpha)
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture
    return (item_texture != NULL); // Trả về true nếu texture được tạo thành công, false nếu thất bại
}

// Hàm đặt vị trí cho vật phẩm
void Item::SetPosition(int x, int y) {
    item_rect.x = x; // Gán tọa độ X vào hình chữ nhật
    posY = static_cast<float>(y); // Gán vị trí thực tế Y (dùng float để chính xác)
    item_rect.y = y; // Gán tọa độ Y vào hình chữ nhật
}

// Hàm vẽ vật phẩm lên màn hình
void Item::Render(SDL_Renderer* renderer) {
    if (is_active && item_texture) { // Chỉ vẽ nếu vật phẩm đang hoạt động và có texture
        SDL_RenderCopy(renderer, item_texture, NULL, &item_rect); // Sao chép texture của vật phẩm lên màn hình tại vị trí item_rect
        // Tham số NULL nghĩa là sử dụng toàn bộ texture, không cắt xén
    }
}

// Hàm cập nhật vị trí của vật phẩm (rơi xuống)
void Item::Update() {
    if (is_active) { // Nếu vật phẩm đang hoạt động
        posY += velY; // Tăng vị trí thực tế Y dựa trên tốc độ rơi
        item_rect.y = static_cast<int>(posY); // Chuyển đổi vị trí Y từ float sang int để gán vào SDL_Rect
        if (item_rect.y > HEIGHT) { // Nếu vật phẩm rơi ra khỏi màn hình dưới
            is_active = false; // Vô hiệu hóa vật phẩm (biến mất)
        }
    }
}
