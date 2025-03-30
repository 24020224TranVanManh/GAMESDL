
#include "power.h" // Include header của lớp POW để sử dụng định nghĩa lớp

POW::POW() { // Constructor mặc định của lớp POW
    pow_rect = {0, 0, 10, 10}; // Khởi tạo hình chữ nhật của đạn với vị trí (0, 0) và kích thước 10x10
    pow_texture = NULL; // Texture ban đầu là NULL (chưa tải hình ảnh)
    damage = 1;         // Đặt sát thương mặc định là 1
    velX = 0.0f;        // Vận tốc ban đầu theo trục X là 0
    velY = 0.0f;        // Vận tốc ban đầu theo trục Y là 0
    posX = 0.0f;        // Vị trí thực tế ban đầu theo trục X là 0
    posY = 0.0f;        // Vị trí thực tế ban đầu theo trục Y là 0
}

POW::~POW() { // Destructor của lớp POW
    if (pow_texture != NULL) { // Kiểm tra xem texture đã được cấp phát chưa
        SDL_DestroyTexture(pow_texture); // Giải phóng texture để tránh rò rỉ bộ nhớ
        pow_texture = NULL; // Đặt lại con trỏ về NULL
    }
}

bool POW::LoadImg(const char* path, SDL_Renderer* renderer) { // Hàm tải hình ảnh cho đạn
    SDL_Surface* surface = IMG_Load(path); // Tải hình ảnh từ đường dẫn (path) thành surface
    if (!surface) { // Nếu tải hình ảnh thất bại
        std::cout << "Không thể load ảnh đạn: " << IMG_GetError() << std::endl; // In thông báo lỗi từ SDL_image
        return false; // Trả về false để báo hiệu thất bại
    }
    pow_texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture để sử dụng với renderer
    SDL_SetTextureBlendMode(pow_texture, SDL_BLENDMODE_BLEND); // Bật chế độ hòa trộn để hỗ trợ độ trong suốt (alpha)
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture
    return (pow_texture != NULL); // Trả về true nếu texture được tạo thành công, false nếu thất bại
}

void POW::SetPosition(int x, int y) { // Hàm đặt vị trí ban đầu cho đạn
    posX = static_cast<float>(x); // Gán tọa độ X thực tế (dùng float để chính xác)
    posY = static_cast<float>(y); // Gán tọa độ Y thực tế (dùng float để chính xác)
    pow_rect.x = x; // Cập nhật tọa độ X trong hình chữ nhật SDL_Rect
    pow_rect.y = y; // Cập nhật tọa độ Y trong hình chữ nhật SDL_Rect
}

void POW::SetSize(int w, int h) { // Hàm đặt kích thước cho đạn
    pow_rect.w = w; // Gán chiều rộng (width) cho hình chữ nhật
    pow_rect.h = h; // Gán chiều cao (height) cho hình chữ nhật
}

void POW::SetDamage(int dmg) { // Hàm đặt giá trị sát thương cho đạn
    damage = dmg; // Gán giá trị sát thương từ tham số vào biến damage
}

void POW::SetDirection(float vx, float vy) { // Hàm đặt hướng di chuyển (vận tốc) cho đạn
    velX = vx; // Gán vận tốc theo trục X
    velY = vy; // Gán vận tốc theo trục Y
}

void POW::Update() { // Hàm cập nhật vị trí của đạn mỗi frame
    posX += velX; // Cập nhật vị trí thực tế X dựa trên vận tốc X
    posY += velY; // Cập nhật vị trí thực tế Y dựa trên vận tốc Y
    pow_rect.x = static_cast<int>(posX); // Chuyển đổi vị trí X từ float sang int để gán vào SDL_Rect
    pow_rect.y = static_cast<int>(posY); // Chuyển đổi vị trí Y từ float sang int để gán vào SDL_Rect
}

void POW::Render(SDL_Renderer* renderer) { // Hàm vẽ đạn lên màn hình
    SDL_RenderCopy(renderer, pow_texture, NULL, &pow_rect); // Sao chép texture của đạn lên màn hình tại vị trí pow_rect
    // Tham số NULL nghĩa là sử dụng toàn bộ texture, không cắt xén
}
