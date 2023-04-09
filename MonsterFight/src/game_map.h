
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

class TileMat : public BaseObject
{
public:
    TileMat(){;}
    ~TileMat(){;}

};

//Lớp quản lý đối tượng xây dựng map: đối tượng kết hợp với TileMat và struct Map để xây dựng lên bản đồ, fill toàn bộ những tile vào những vị trí map tương ứng
class GameMap
{
public:
    GameMap() {;}
    ~GameMap() {;}

    //Hàm đọc toàn bộ thông tin sơ đồ trong file
    void LoadMap (std::string name);
    //Hàm load toàn bộ hình ảnh cho map
    void LoadTiles (SDL_Renderer* screen);
    //hàm fill hình ảnh vào vị trí tương ứng
    void DrawMap (SDL_Renderer* screen);
    //Hàm để lấy dữ liệu map
    Map getMap () const {return game_map_;};
    //
    void setMap (Map& mapData) {game_map_ = mapData;};

private:
    //Chứa toàn bộ thông tin của bản đồ nhập từ file map
    Map game_map_;
    //Chứa thông tin về đối tượng hình ảnh fill vào map
    TileMat tile_img [MAX_TILES];
};


#endif // GAME_MAP_H_
