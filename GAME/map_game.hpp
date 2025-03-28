
#ifndef MAP_GAME_H
#define  MAP_GAME_H
#include"Baseoject.h"
#include"comfunc.h"
class TILEMAT : public Baseoject
{
public:
    TILEMA(){;}
    ~TILEMAT(){;}
};
class GameMap
{
public:
    GameMap(){;}
    ~GameMap(){;}
    void LoadMAP(char* mAp);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap (SDL_Renderer* screen);
    Map getMap() const {return game_map;}
private:
    Map game_map;
    TILEMAT tile_mat[20];
};
#endif // MAP_GAME_H
