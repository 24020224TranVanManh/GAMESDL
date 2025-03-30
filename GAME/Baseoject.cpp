
#include "Baseoject.h"
#include <iostream>

// Constructor mặc định của lớp Baseoject
Baseoject::Baseoject() {
    texture = NULL; // Khởi tạo texture là NULL (chưa tải hình ảnh)
    width = 0;      // Đặt chiều rộng ban đầu là 0
    height = 0;     // Đặt chiều cao ban đầu là 0
}

// Destructor của lớp Baseoject
Baseoject::~Baseoject() {
    free(); // Gọi hàm free() để giải phóng tài nguyên (texture) khi đối tượng bị hủy
}

// Hàm tải hình ảnh từ file vào texture
bool Baseoject::LoadImg(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path); // Tải hình ảnh từ đường dẫn (path) thành surface
    if (!surface) { // Nếu tải hình ảnh thất bại
        std::cout << "Unable to load image: " << IMG_GetError() << std::endl; // In thông báo lỗi từ SDL_image
        return false; // Trả về false để báo hiệu thất bại
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển surface thành texture để sử dụng với renderer
    if (texture) { // Nếu texture được tạo thành công
        width = surface->w; // Lấy chiều rộng từ surface và gán vào biến width
        height = surface->h; // Lấy chiều cao từ surface và gán vào biến height
    }
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture
    return texture != NULL; // Trả về true nếu texture được tạo thành công, false nếu thất bại
}

// Hàm vẽ texture lên màn hình
void Baseoject::render(SDL_Renderer* renderer, const SDL_Rect* clip) {
    SDL_Rect renderQuad = {0, 0, width, height}; // Tạo hình chữ nhật renderQuad với vị trí (0, 0) và kích thước của texture
    if (clip != NULL) { // Nếu có tham số clip (để cắt xén texture)
        renderQuad.w = clip->w; // Đặt chiều rộng của renderQuad theo clip
        renderQuad.h = clip->h; // Đặt chiều cao của renderQuad theo clip
    }
    SDL_RenderCopy(renderer, texture, clip, &renderQuad); // Sao chép texture lên màn hình
    // - Tham số clip: phần của texture được lấy để vẽ (NULL nếu dùng toàn bộ texture)
    // - Tham số renderQuad: vị trí và kích thước trên màn hình nơi texture được vẽ
}

// Hàm giải phóng texture
void Baseoject::free() {
    if (texture != NULL) { // Kiểm tra xem texture đã được cấp phát chưa
        SDL_DestroyTexture(texture); // Giải phóng texture để tránh rò rỉ bộ nhớ
        texture = NULL; // Đặt lại con trỏ về NULL
        width = 0;      // Đặt lại chiều rộng về 0
        height = 0;     // Đặt lại chiều cao về 0
    }
}
