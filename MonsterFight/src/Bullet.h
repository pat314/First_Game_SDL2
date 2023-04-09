
#ifndef BULLET_H_
#define BULLET_H_

#include "BaseObject.h"
#include "CommonFunc.h"

class Bullet : public BaseObject
{
public:
    Bullet();
    ~Bullet();
    //Hàm cài đặt giá trị cho biến
    void  setBulletVal (const int& xVal, const int& yVal) {x_val = xVal; y_val = yVal;};
    //hàm lấy giá trị của biến
    int getBullet_x_val () const {return x_val;};
    int getBullet_y_val () const {return y_val;};
    //Hàm set trạng thái cho isMove
    void setBullet_is_move (const bool& isMove) {is_move = isMove;};
    bool getBullet_is_move () const {return is_move;};
    //Hàm set-get hướng đạn
    void setBullet_bulletDirection (const int& bulletDir) {bulletDirection = bulletDir;};
    int getBullet_bulletDirection () const {return bulletDirection;};
    //Hàm set - get bulletType
    void setBullet_bulletType (const int& _bulletType) {bulletType = _bulletType;};
    int getBullet_bulletType () const {return bulletType;};


    //Hàm xử lý vấn đề di chuyển cho đạn
    void HandleBulletMove (const int& x_border, const int& y_border, Map& mapData);
    bool LoadBulletImg (SDL_Renderer* des, int bulletDirection);

private:
    //Biến lưu lượng di chuyển khi đạn chuyển động
    int x_val;
    int y_val;
    //Biến lưu trạng thái có chuyển động hay không? (Đạn chuyển động đến giới hạn nào đó thì dừng)
    bool is_move;
    //Biến quản lý hướng đạn
    int bulletDirection;
    //Biến quản lý loại đạn
    int bulletType;
    //Biến quản lý kích thước dài của đạn
    int bullet_frame_size;

};

#endif // BULLET_H_
