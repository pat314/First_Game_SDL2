
#include "game_map.h"

void GameMap::LoadMap(std::string name)
{
      std::ifstream input (name.c_str());
      if (!input) return;

      game_map_.max_x_ = game_map_.max_y_ = 0;
      for (int i = 0; i < MAX_MAP_Y; i++)
      {
        for (int j = 0; j < MAX_MAP_X; j++)
        {
          input >> game_map_.tile[i][j];

        }
      }

      game_map_.max_x_ = (MAX_MAP_X)*TILE_SIZE;
      game_map_.max_y_ = (MAX_MAP_Y)*TILE_SIZE;

      game_map_.start_x_ = game_map_.start_y_ = 0;

      game_map_.file_name = name;

      input.close();
}

void GameMap::LoadTiles(SDL_Renderer* screen)
{
    std::string filename[MAX_TILES];
    for (int i = 0 ; i < 10; i++)
    {
        filename[i] = "";
        filename[i] = filename[i] + "map//" + (char)(i + 48) + ".png";
        tile_img [i].LoadImg(filename[i], screen);
    }
}


void GameMap::DrawMap(SDL_Renderer* des)
{
  int map_x = 0;
  int x1 = 0;
  int x2 = 0;

  int map_y = 0;
  int y1 = 0;
  int y2 = 0;

  x1 = (game_map_.start_x_ %TILE_SIZE)*-1;
  x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

  y1 = (game_map_.start_y_%TILE_SIZE)*-1;
  y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 :TILE_SIZE);


  for (int i = y1; i < y2; i += TILE_SIZE)
  {
    map_x = game_map_.start_x_/TILE_SIZE;
    for (int j= x1; j < x2; j +=TILE_SIZE)
    {
      if (game_map_.tile[map_y][map_x] != 0)
      {
        tile_img [game_map_.tile[map_y][map_x]].SetRect(j, i);
        tile_img [game_map_.tile[map_y][map_x]].Render(des);
      }
      map_x++;
    }
    map_y++;
  }
}
