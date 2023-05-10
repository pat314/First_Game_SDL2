//Lớp quản lý nhân vật
#ifndef MAIN_CHARACTER_H_
#define MAIN_CHARACTER_H_

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Bullet.h"


class MainCharacter: public BaseObject
{
public:
    MainCharacter();
    ~MainCharacter();

    bool LoadImg (std:: string path, SDL_Renderer* screen);
    //Hàm chạy frame để thành hình động
    void ShowSprite (SDL_Renderer* des);
    void HandleInputAction (SDL_Event events, SDL_Renderer* screen, Mix_Chunk* sound[]);
    //Hàm cắt sprite thành các frame
    void set_frame ();
    //Hàm lấy kích cỡ của 1 frame
    int getPlayer_w_frame () const {return w_frame;};
    int getPlayer_h_frame () const {return h_frame;};
    //Hàm điều khiển nhân vật chạy được trên map
    void play (Map& mapData, Mix_Chunk* sound[]);
    //Check xem da cham map chuwa
    void CheckGround (Map& mapData, Mix_Chunk* sound[]);

    void getMapCoordination (const int mapX, const int mapY){map_X = mapX; map_Y = mapY;};
    //Hàm tính toán di chuyển bản đồ cùng nhân vật (sao cho nhân vật luôn đứng ở trong khoảng xác định trên map)
    void CenterMap (Map& mapData);

    void UpdateImg (SDL_Renderer* des);

    //Hàm set - get danh sách đạn
    void setBulletList (std::vector<Bullet*> bullet_list) {bulletList = bullet_list;};
    std::vector<Bullet*> getBulletList() const {return bulletList;};
    //Hàm xử lý đạn bắn
    void HandleBullet (SDL_Renderer* des, Map& mapData);
    //Hàm bỏ ảnh viên đạn thứ index sau khi bắn trúng threat
    void ClearBullet (const int& index);
    //Hàm đếm tiền ăn được
    void CoinCounting();
    //hàm set thời gian hồi sinh
    int Main_getCoinCount () const {return coin_count;};
    void setPlayer_revivalTime (const int& rT) {timeRevival = rT;};

    void set_Abyss (const bool state) {Main_Abyss = state;};
    bool get_Abyss () const {return Main_Abyss;};

    void achieved_Dragon_ball (bool success) {is_achieved_Dragon_ball = success;};
    bool check_achieve_Dragon_ball () const {return is_achieved_Dragon_ball;};


protected:
    //Container chứa đạn nhân vật
    std::vector<Bullet*> bulletList;
    //Luong thay doi khi di chuyen
    float x_val;
    float y_val;
    //quãng đường đi được hiện tại
    float x;
    float y;
    //kich thuoc cua mot frame
    int w_frame;
    int h_frame;
    //Luu cac frame
    SDL_Rect frame_clip[8];
    //Bien luu kieu trang thai cho nhan vat chinh
    Control type;
    //Bien luu chi so cua frame (dang o frame nao?)
    int i_frame;
    //Bien luu trang thai cho nhan vat nhan tu ban phim
    int status;
    bool isOnGround;
    //Bien luu vi tri cua ban do: khi nhan vat di chuyen thi mep ban do o vi tri nao
    int map_X;
    int map_Y;
    //Biến lưu thời gian chuyển giao mạng của nhân vật (thời gian hồi sinh)
    int timeRevival;
    //Biến lưu loại đạn
    int mainChaBulletType;
    //Biến đếm lượng tiền ăn được
    int coin_count;
    //Check xem rơi xuống vực chưa
    int Main_Abyss;
    //Check xem lấy được ngọc rồng chưa
    bool is_achieved_Dragon_ball;
};



#endif // MAIN_CHARACTER_H_
