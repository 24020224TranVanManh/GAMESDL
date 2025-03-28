#include"comfunc.h"
#ifndef MOVE
#define MOVE

class Box
{
public:
    Box();
    ~Box();
    void Draw_Box (SDL_Rect* filled_rect,SDL_Renderer* renderer);
private:
    int x;
    int y;
    int size=10;


};
#endif // MOVE
