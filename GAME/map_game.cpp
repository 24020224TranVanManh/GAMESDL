#include"Baseoject.h"
#include"comfunc.h"
#include"map_game.hpp"

void GameMap::LoadMAP(char* name)
{
    FILE* fi=NULL;
    fopen_s(&fi,name,"rb");
    if(fi==NULL)
    {
        return;
    }
    game_map.max_x=0;
    game_map.max_y=0;
    for (int i=0;i<Map_Y;i++)
    {
        for(int j=0;j<Map_X;j++)
        {
-           fscanf(fi,"%d",&game_map.tile[i][j]);
            int val=game_map.tile[i][j];
            if(val>0)
            {
                if(j>game_map.max_x)
                {
                    game_map.max_x=j;
                }
                 if(i>game_map.max_y)
                {
                    game_map.max_y=i;
                }
            }
        }
    }
     game_map.max_x= (game_map.max_x+1)*TILE_SIZE;
      game_map.max_y= (game_map.max_y+1)*TILE_SIZE;
      game_map.start_x=0;
      game_map.start_y=0;
      game_map.file_name=name;
      fclose(fi);
}
void GameMap::LoadTiles(SDL_Renderer* screen)
{
    char file_img[30];
    FILE*fi = NULL;
    for(int i=0;i< 20;i++)
    {
        sprintf(file_img,"anh/%d.jpg",i);
        fopen_s(&fi,file_img,"rb");
        if(fi==NULL)
        {
            continue;
        }
        fclose(fi);
        tile_mat[i].LoadImg(file_img,screen);
    }
}
void GameMap::DrawMap(SDL_Renderer* screen)
{
    int x1=0;
    int x2=0;
    int y1=0;
    int y2=0;
    int maP_x=0;
    int maP_y=0;
    maP_x=game_map.start_x/TILE_SIZE;
    maP_y=game_map.start_y/TILE_SIZE;
    x1= (game_map.start_x%TILE_SIZE)*-1;
    x2= x1+ WIDTH+ (x1==0 ? 0:TILE_SIZE);
    y1= (game_map.start_y%TILE_SIZE)*-1;
    y2= y1+ HEIGHT+ (y1==0 ? 0:TILE_SIZE);
    for(int i=y1;i<=y2;i=i+TILE_SIZE)
    {
        maP_x=game_map.start_x/TILE_SIZE;
        for(int j=x1;j<=x2;j=j+TILE_SIZE)
        {
             //if (maP_x >= 0 && maP_x < Map_X && maP_y >= 0 && maP_y < Map_Y)
            {int val=game_map.tile[maP_y][maP_x];
            if(val>0 && val<20)
            {
                tile_mat[val].SetRect(j,i);
                tile_mat[val].render(screen);
            }
            }
            maP_x++;
        }
        maP_y++;
    }
}
