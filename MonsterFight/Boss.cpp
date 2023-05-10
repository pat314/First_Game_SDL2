
#include "Boss.h"


BossObject::BossObject()
{
    frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0;
    y_pos_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    think_time_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    is_free = false;
}

BossObject::~BossObject()
{

}

bool BossObject::Boss_LoadImg(std::string path, SDL_Renderer* screen)
{
    bool success = BaseObject::LoadImg(path, screen);

    if (success)
    {
        width_frame_ = rect_.w/FRAME_NUM_32;
        height_frame_ = rect_.h;
    }

    return success;
}

void BossObject::set_clips()
{
    //Clip the sprites
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i = 0; i < FRAME_NUM_32; i++)
        {
            frame_clip_[i].x = width_frame_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}


void BossObject::Show(SDL_Renderer* des)
{
    if (!think_time_)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_- map_y_;
        frame_++;
        if( frame_ >= 32 ) frame_ = 0;

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

        SDL_RenderCopy(des, p_object_, currentClip, &renderQuad );
    }
}

void BossObject::DoBoss(Map& g_map)
{
    if (!think_time_)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;

        if (y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }

        if (input_type_.left == 1)
        {
            x_val_ -= BOSS_SPEED;
        }

        else if (input_type_.right == 1)
        {
            x_val_+= BOSS_SPEED;
        }

        if (input_type_.jump == 1)
        {
            if (on_ground_ == 1)
            {
                y_val_ = -BOSS_HEIGHT_VAL;
                on_ground_ == 0;
            }

            input_type_.jump = 0;
        }

        CheckToMap(g_map);

    }

    if (think_time_ > 0)
    {
        think_time_--;

        if (think_time_ == 0)
        {
            InitBoss();
        }
    }
}

void BossObject::InitBoss()
{
    x_val_ = 0;
    y_val_ = 0;

    if (x_pos_ > 4 * TILE_SIZE)
    {
        x_pos_ -= 4 * TILE_SIZE;
    }
    else
        x_pos_ = 0;
    y_pos_ = 0;
    think_time_ = 0;
    input_type_.left = 1;
}

void BossObject::CheckToMap(Map& g_map)
{
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    on_ground_ = 0;

    //Check Horizontal
    int height_min =   height_frame_ ;

    /*
    x1,y1***x2
    *       *
    *       *
    *       *
    *y2******

    */
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - err) / TILE_SIZE;

    y1 = (y_pos_) / TILE_SIZE;
    y2 = (y_pos_ + height_min - err) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_ > 0) // nhân vật di chuyển sang trái ===>
        {
            //Nếu ô liền kề không là blank tile
            if ((g_map.tile[y1][x2] != BLANK_TILE && g_map.tile[y1][x2] != COIN_TILE) || (g_map.tile[y2][x2] != BLANK_TILE && g_map.tile[y2][x2] != COIN_TILE))
            {
                // không thể di chuyển
                x_pos_ = x2 * TILE_SIZE;
                x_pos_ -= width_frame_ + 1;
                x_val_ = 0; // cannot moving
            }
        }
        else if (x_val_ < 0) // Di chuyển sang trái    <====
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE && g_map.tile[y1][x1] != COIN_TILE) || (g_map.tile[y2][x1] != BLANK_TILE && g_map.tile[y2][x1] != COIN_TILE))
            {
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }


    // Check vertical
    int width_min = width_frame_;

    x1 = (x_pos_) / TILE_SIZE;
    x2 = (x_pos_ + width_min) / TILE_SIZE;

    y1 = (y_pos_ + y_val_) / TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_ - err) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if (y_val_ > 0)
        {
            //Tương tự với Oy
            if ((g_map.tile[y2][x1] != BLANK_TILE) || (g_map.tile[y2][x2] != BLANK_TILE))
            {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;

                y_val_ = 0;

                on_ground_ = 1;
            }
        }
        else if (y_val_ < 0)
        {
            if ((g_map.tile[y1][x1] != BLANK_TILE) || (g_map.tile[y1][x2] != BLANK_TILE))
            {
                y_pos_ = (y1 + err) * TILE_SIZE;
                y_val_ = 0;
                on_ground_ = 0;
            }
        }
    }

    //Nếu không có va chạm với tile map.
    x_pos_ += x_val_;
    y_pos_ += y_val_;

    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ > g_map.max_x_)
    {
        x_pos_ = g_map.max_x_ - width_frame_ - err;
    }
    //Nếu rơi xuống vực
    if (y_pos_ > g_map.max_y_)
    {
        think_time_ = 60;
    }
}

void BossObject::InitBullet(SDL_Renderer* screen)
{
    Bullet* B_bullet = new Bullet();
    bool ret = B_bullet->LoadImg("image//boss_bullet.png", screen);
    if (ret)
    {
        B_bullet->setBullet_bulletDirection(D_LEFT);
        B_bullet->setBullet_is_move(true);
        B_bullet->SetRect(rect_.x - 50, rect_.y + height_frame_ - 30);
        B_bullet->setBulletVal(15, 0);
        bullet_list_.push_back(B_bullet);
    }
}


void BossObject::MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit, Map& mapData, Mix_Chunk* sound)
{
    if (frame_ == 18 && !is_Boss_Free())
    {
        InitBullet(des);
    }

    for (int i = 0; i < bullet_list_.size(); i++)
    {
        Bullet* Boss_bullet = bullet_list_.at(i);
        if (Boss_bullet != NULL)
        {
            if (Boss_bullet->getBullet_is_move())
            {
                Mix_PlayChannel (-1, sound, 0);
                Boss_bullet->HandleBulletMove(x_limit, y_limit, mapData);
                Boss_bullet->Render(des);
            }
            else
            {
                bullet_list_.erase(bullet_list_.begin() + i);

                if (Boss_bullet != NULL)
                {
                    delete Boss_bullet;
                    Boss_bullet = NULL;
                }
            }
        }
    }
}

void BossObject::ClearBullet ()
{
    int bulletRemaining = bullet_list_.size();
    //Xóa hết các viên đạn còn lại
    for (int i = 0; i < bulletRemaining; i++)
    {
        Bullet* bullet = bullet_list_.at(i);
        bullet_list_.erase(bullet_list_.begin() + i);

        if (bullet != NULL)
        {
            delete bullet;
            bullet = NULL;
        }
    }
}
