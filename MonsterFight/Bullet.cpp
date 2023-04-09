
#include "Bullet.h"

Bullet::Bullet()
{
    x_val = 0;
    y_val = 0;
    is_move = false;
    bulletType = T_SPHERE;
}

Bullet::~Bullet()
{

}

bool Bullet:: LoadBulletImg (SDL_Renderer* des, int bulletDirection)
{
    bool success = true;
    if (bulletType == T_LASER && bulletDirection == D_RIGHT)
    {
        success = LoadImg ("image//player_bullet2.png", des);
        bullet_frame_size = 60;
    }
    else if (bulletType == T_LASER && bulletDirection == D_LEFT)
    {
        success = LoadImg ("image//player_bullet2_left.png", des);
        bullet_frame_size = 60;
    }
    else if (bulletType == T_SPHERE)
    {
        success = LoadImg ("image//player_bullet1.png", des);
        bullet_frame_size = 12;
    }
    else if (bulletType == O_LASER && bulletDirection == D_RIGHT)
    {
        success = LoadImg ("image//laser_bullet_threat_right.png", des);
        bullet_frame_size = 60;
    }
    else if (bulletType == O_LASER && bulletDirection == D_LEFT)
    {
        success = LoadImg ("image//laser_bullet_threat_left.png", des);
        bullet_frame_size = 60;
    }
    else if (bulletType == O_SPHERE)
    {
        success = LoadImg ("image//threat_bullet.png", des);
        bullet_frame_size = 12;
    }
    return success;
}

void Bullet::HandleBulletMove (const int& x_border, const int& y_border, Map& mapData)
{
    //kiểm tra xem đạn nằm ở ô nào đến ô nào trên map
    //Chỉ số ô map liền kề viên đạn
    int x1 = (rect_.x + mapData.start_x_)/TILE_SIZE;
    int x2 = (rect_.x + bullet_frame_size + mapData.start_x_)/TILE_SIZE;
    int y1 = rect_.y/TILE_SIZE;

    if (bulletDirection == D_RIGHT)
    {
        //Vị trí của viên đạn di chuyển sang phải
        rect_.x +=x_val;

        //Nếu đạn gặp chướng ngại vật thì cũng biến mất (không bắn được qua chướng ngại vật)
        if (mapData.tile[y1][x1] != BLANK_TILE || mapData.tile[y1][x2] != BLANK_TILE) is_move = false;
        //Nếu mà đạn vượt quá biên thì đạn sẽ biến mất
        else if (rect_.x > x_border)  is_move = false;
    }
    else if (bulletDirection == D_LEFT)
    {
        //Vị trí của viên đạn di chuyển sang trái
        rect_.x -= x_val;

        if (mapData.tile[y1][x1] != BLANK_TILE || mapData.tile[y1][x2] != BLANK_TILE) is_move = false;
        //Nếu mà đạn vượt quá biên thì đạn sẽ biến mất
        else if (rect_.x < 0) is_move = false;
    }
}
