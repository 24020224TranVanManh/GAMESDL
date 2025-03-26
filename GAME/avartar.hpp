#include"Baseoject.h"
#include"comfunc.h"
#include"map_game.hpp"
#ifndef AVARTAR_h
#define AVARTAR_H
class AVARTAR: public Baseoject
{
public:
    AVARTAR();
    ~AVARTAR();
    enum WalkType
    {
        WALK_RIGHT = 0,
        WALK_LEFT = 1,
    };
    bool LoadImg(char* name,SDL_Renderer* screen);
    void show(SDL_Renderer* des);
    void EVENT (SDL_Event events, SDL_Renderer* screen);
    void set_clips();
private:
    float x_val;
    float y_val;

    float x_pos;
    float y_pos;

    int width_frame;
    int heigh_frame;
    SDL_Rect frame_clip[8];
    INPUT Input_type;
    int frame;
    int status;
};

#endif // AVARTAR_h

