#include"avartar.hpp"

AVARTAR::~AVARTAR(){}
AVARTAR::AVARTAR()
{
    frame=0;
    x_pos=0;
    y_pos=0;
    x_val=0;
    y_val=0;
    width_frame=0;
    heigh_frame=0;
    status=-1;
}

bool AVARTAR::LoadImg(char* name,SDL_Renderer* screen)
{
    bool ret=Baseoject::LoadImg(name,screen);

    if (ret==true)
    {
        width_frame=rect_.w/8;
        heigh_frame=rect_.h;
    }
    return ret;

}
void AVARTAR::set_clips()
{
    if(width_frame>0 && heigh_frame>0)
    {
        frame_clip[0].x=0;
        frame_clip[0].y=0;
        frame_clip[0].w=width_frame;
        frame_clip[0].h=heigh_frame;

        frame_clip[1].x=width_frame;
        frame_clip[1].y=0;

        frame_clip[1].x=width_frame;
        frame_clip[1].y=0;

        frame_clip[2].x=2*width_frame;
        frame_clip[2].y=0;
        frame_clip[3].x=3*width_frame;
        frame_clip[3].y=0;
        frame_clip[4].x=4*width_frame;
        frame_clip[4].y=0;
        frame_clip[5].x=5*width_frame;
        frame_clip[5].y=0;
        frame_clip[6].x=6*width_frame;
        frame_clip[6].y=0;
        frame_clip[7].x=7*width_frame;
        frame_clip[7].y=0;
        frame_clip[8].x=8*width_frame;
        frame_clip[8].y=0;
    }
}
void AVARTAR::show(SDL_Renderer* des)
{
    if(status==WALK_LEFT)
    {
     LoadImg("nhanvat/left.png",des);
    }
    else
    {
        LoadImg("nhanvat/right.png",des);
    }


    if(Input_type.left==1 || Input_type.right==1 )
    {
        frame++;
    }
    else
    {
        frame=0;
    }
    if(frame>8) frame=0;
    rect_.x=x_pos;
    rect_.y=y_pos;
    SDL_Rect* Clipp=&frame_clip[frame];
    SDL_Rect renderQuad = {rect_.x,rect_.y,rect_.w,rect_.h};
    SDL_RenderCopy(des,p_objcet,Clipp,&renderQuad);
}
void AVARTAR::EVENT (SDL_Event events,SDL_Renderer* screen)
{
    if(events.type==SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
            case SDLK_RIGHT:
            {
                status=WALK_RIGHT;
                Input_type.right=1;
            }
            break;
             case SDLK_LEFT:
            {
                status=WALK_LEFT;
                Input_type.left=1;
            }
            break;
             default:
                break;
        }
    }
    else if (events.type==SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
            case SDLK_RIGHT:
            {
                status=WALK_RIGHT;
                Input_type.right=0;
            }
            break;
             case SDLK_LEFT:
            {
                status=WALK_LEFT;
                Input_type.left=0;
            }
            break;
        }
    }
}
