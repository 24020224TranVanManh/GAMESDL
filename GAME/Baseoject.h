
#ifndef BASEOJECT_H
#define BASEOJECT_H

#include <SDL.h>       // Thư viện SDL để xử lý đồ họa và sự kiện
#include <SDL_image.h> // Thư viện SDL_image để tải hình ảnh (PNG, JPG, v.v.)

class Baseoject { // Lớp cơ sở Baseoject đại diện cho các đối tượng đồ họa cơ bản trong game
public:
    Baseoject(); // Constructor mặc định để khởi tạo đối tượng
    ~Baseoject(); // Destructor để giải phóng tài nguyên
    bool LoadImg(const char* path, SDL_Renderer* renderer); // Tải hình ảnh từ file vào texture
    void render(SDL_Renderer* renderer, const SDL_Rect* clip); // Vẽ texture lên màn hình, có thể cắt xén bằng clip
    void free(); // Giải phóng texture để tránh rò rỉ bộ nhớ

    // Hàm getter để lấy texture của đối tượng
    SDL_Texture* getTexture() const { return texture; } // Trả về con trỏ tới texture của đối tượng

private:
    SDL_Texture* texture; // Texture (hình ảnh) của đối tượng để vẽ lên màn hình
    int width; // Chiều rộng của texture (được cập nhật khi tải hình ảnh)
    int height; // Chiều cao của texture (được cập nhật khi tải hình ảnh)
};

#endif // BASEOJECT_H // Kết thúc bảo vệ header
