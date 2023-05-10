
#include "Threat.h"

Threat::Threat()
{
    w_frame_threat = 0;
    h_frame_threat = 0;
    x_val_threat = 0.0;
    y_val_threat = 0.0;
    x_threat = 0.0;
    y_threat = 10.0;
    isOnGround_threat = false;
    revive_time_threat = 0;
    frame_threat = 0;

    animationThreat_a_interval = 0;
    animationThreat_b_interval = 0;
    inputType_threat.left = 0;
    typeMove = STATIC_THREAT;
    xCollision = false;


}

Threat::~Threat()
{


}

SDL_Rect Threat::GetRectFrame()
{
  SDL_Rect rect;
  rect.x = rect_.x;
  rect.y = rect_.y;
  rect.w = rect_.w/THREAT_FRAME_NUM;
  rect.h = rect_.h;

  return rect;
}

bool Threat::loadThreat_img (std::string path, SDL_Renderer* screen)
{
    bool success = BaseObject::LoadImg(path, screen);
    if (success)
    {
        w_frame_threat = rect_.w / THREAT_FRAME_NUM;
        h_frame_threat = rect_.h;
    }
    return success;
}

void Threat::setThreat_frame ()
{
    if (w_frame_threat > 0 && h_frame_threat > 0)
    {
        //Bổ từng frame của threat rồi cho vào frame_clip_threat (cắt ảnh ra roi luu vao frame_clip)
        for (int i = 0; i < THREAT_FRAME_NUM; i++)
        {
            frame_clip_threat[i].x = w_frame_threat * i;
            frame_clip_threat[i].y = 0;
            frame_clip_threat[i].w = w_frame_threat;
            frame_clip_threat[i].h = h_frame_threat;
        }
    }
}

void Threat::showThreat_sprite (SDL_Renderer* des)
{
    if (!revive_time_threat)
    {
        rect_.x = x_threat - map_X_threat;
        rect_.y = y_threat - map_Y_threat;
        //Liên tục tăng frame
        frame_threat ++;
        if (frame_threat >= THREAT_FRAME_NUM) frame_threat = 0;
        //Lấy ra hình hiện tại (tại khung hình máy đang chạy)
        SDL_Rect* currentThreat_frame = &frame_clip_threat[frame_threat];
        //Lấy ra vị trí và kích thước của 1 frame tại thời điểm đang xét
        SDL_Rect renderQuand_threat = {rect_.x, rect_.y, w_frame_threat, h_frame_threat};
        //Apply frame từ texture hiện thời lên màn hình renderer với kích thước và vị trí như trên
        SDL_RenderCopy (des, p_object_, currentThreat_frame, &renderQuand_threat);
    }
}

void Threat::doThreat(Map& gMap, SDL_Renderer* screen)
{
    if (!revive_time_threat)
    {
        //purely physics!
        x_val_threat = 0;
        y_val_threat += GRAVITY;
        if (y_val_threat >= V_MAX) y_val_threat = V_MAX;

        //Cho threat động di chuyển

        if (inputType_threat.left == 1)
        {
            x_val_threat -= V_THREAT;

        }
        else if (inputType_threat.right == 1)
        {
            x_val_threat += V_THREAT;
        }
        //Kiểm tra nó có va chạm với bản đồ không?
        checkThreat_ground(gMap);
    }
    //Trong trường hợp threat đang hồi sinh
    else if (revive_time_threat > 0)
    {
        revive_time_threat --;
        //Đến khi nào hồi sinh xong thì mọi thông số trở về ban đầu
        if (!revive_time_threat)
        {
            initThreat();
        }
    }
}

void Threat::initThreat ()
{
    x_val_threat = 0;
    y_val_threat = 0;

    if (x_threat > 4 * TILE_SIZE)
    {
        x_threat -= 4*TILE_SIZE;
        animationThreat_a_interval -= 4*TILE_SIZE;
        animationThreat_b_interval -= 4*TILE_SIZE;
    }
    else x_threat = 0;
    y_threat = 0;
    revive_time_threat = 0;
    if (typeMove == STATIC_THREAT) inputType_threat.left = 0;
    else if (typeMove == MOVING_THREAT) inputType_threat.left = 1;
}

void Threat::checkThreat_ground(Map& mapData)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //Check horizontal
    //kiem tra xem threat nằm ở ô nào đến ô nào trên map
    int h_min = h_frame_threat < TILE_SIZE ? h_frame_threat:TILE_SIZE;
    //Chỉ số ô map liền kề trước threat
    x1 = (x_threat + x_val_threat)/TILE_SIZE;
    y1 = y_threat/TILE_SIZE;

    //Chỉ số ô map liền kề sau threat
    x2 = (x_threat + x_val_threat + w_frame_threat - err)/TILE_SIZE;
    y2 = (y_threat + h_min - err)/TILE_SIZE;

    if (x1 >=0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
    {
        if (x_val_threat > 0) //Threat di chuyển sang phải
        {
            if ((mapData.tile[y1][x2] != BLANK_TILE && mapData.tile[y1][x2] != COIN_TILE) || (mapData.tile[y2][x2] != BLANK_TILE && mapData.tile[y2][x2] != COIN_TILE))
            {
                x_threat = x2 * TILE_SIZE;
                x_threat -= w_frame_threat + err;
                x_val_threat = 0;
                xCollision = true;
            }
        }
        else if (x_val_threat < 0) //Threat di chuyển sang trái
        {
            if ((mapData.tile[y1][x1] != BLANK_TILE && mapData.tile[y1][x1] != COIN_TILE) || (mapData.tile[y2][x1] != BLANK_TILE && mapData.tile[y2][x1] != COIN_TILE))
            {
                x_threat = (x1 + err) * TILE_SIZE;
                x_val_threat = 0;
                xCollision = true;
            }
        }
    }

    //Check vertical
    int w_min = w_frame_threat < TILE_SIZE? w_frame_threat:TILE_SIZE;

    x1 = x_threat/TILE_SIZE;
    y1 = (y_threat + y_val_threat)/TILE_SIZE;

    x2 = (x_threat + w_min)/TILE_SIZE;
    y2 = (y_threat + y_val_threat + h_frame_threat - err)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >=0 && y2 < MAX_MAP_Y)
    {
        if (y_val_threat > 0) // Threat đang rơi
        {
            if ((mapData.tile[y2][x1] != BLANK_TILE && mapData.tile[y2][x1] != COIN_TILE) || (mapData.tile[y2][x2] != BLANK_TILE && mapData.tile[y2][x2] != COIN_TILE))
            {
                y_threat = y2 * TILE_SIZE;
                y_threat -= h_frame_threat;
                y_val_threat = 0;
                isOnGround_threat = true;
            }
        }
        else if (y_val_threat < 0) //Threat đang nhảy lên
        {
            if ((mapData.tile[y1][x1] != BLANK_TILE && mapData.tile[y1][x1] != COIN_TILE) || (mapData.tile[y1][x2] != BLANK_TILE && mapData.tile[y1][x2] != COIN_TILE))
            {
                y_threat = (y1 + err)* TILE_SIZE;
                y_val_threat = 0;
                isOnGround_threat = false;
            }
        }
    }
    //Nếu không có va chạm  gì với map nữa thì tăng quãng đường đi
    x_threat += x_val_threat;
    y_threat += y_val_threat;

    if (x_threat < 0) x_threat = 0;
    else if (x_threat + w_frame_threat > mapData.max_x_) x_threat = mapData.max_x_ - w_frame_threat - err;
    //Nếu nhân vật bị rơi xuống vực thẳm
    if (y_threat > mapData.max_y_) revive_time_threat = 60;
}

void Threat::ImplementMoveType_threat (SDL_Renderer* screen)
{
    if (typeMove == STATIC_THREAT) {}
    else if (typeMove == MOVING_THREAT)
    {
        if (isOnGround_threat == true)
        {
            if (x_threat > animationThreat_b_interval)
            {
                inputType_threat.left = 1;
                inputType_threat.right = 0;
                loadThreat_img ("image//threat_left.png", screen);

            }
            else if (x_threat < animationThreat_a_interval)
            {
                inputType_threat.left = 0;
                inputType_threat.right = 1;
                loadThreat_img ("image//threat_right.png", screen);

            }
            else if (inputType_threat.left == 1 && xCollision == true)
            {
                inputType_threat.left = 0;
                inputType_threat.right = 1;
                loadThreat_img ("image//threat_right.png", screen);
                xCollision = false;
            }
            else if (inputType_threat.right == 1 && xCollision == true)
            {
                inputType_threat.left = 1;
                inputType_threat.right = 0;
                loadThreat_img ("image//threat_left.png", screen);
                xCollision = false;
            }
        }
        else
        {
            if (inputType_threat.left == 1)
            {
                loadThreat_img ("image//threat_left.png", screen);
            }
        }
    }
}

void Threat::initThreat_bullet (Bullet* bullet, SDL_Renderer* screen)
{
    if (bullet != NULL)
    {
        bullet->setBullet_bulletType(O_LASER);
        bool success = bullet->LoadBulletImg(screen, D_LEFT);
        if (success)
        {
            bullet->setBullet_is_move(true);
            bullet->setBullet_bulletDirection(D_LEFT);
            bullet->SetRect (rect_.x , rect_.y + 0.2* h_frame_threat);
            bullet->setBulletVal (15,0);
            bulletThreat_list.push_back(bullet);
        }

    }
}

void Threat::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& mapData)
{
    for (int i = 0; i < bulletThreat_list.size(); i++)
    {
        Bullet* p_bullet = bulletThreat_list.at(i);
        if (p_bullet != NULL)
        {
            if (p_bullet->getBullet_is_move())
            {
                int activeBullet_distance = rect_.x - p_bullet->GetRect().x;
                //Nếu khoảng cách giữa nv chính vs threat nhỏ hơn ACTIVE_BULLET_DISTANCE thì mới phát đạn
                if (activeBullet_distance < ACTIVE_BULLET_DISTANCE)
                {
                    p_bullet->HandleBulletMove(x_limit, y_limit, mapData);
                    p_bullet->Render(screen);
                }
                else p_bullet-> setBullet_is_move (false);

            }
            else
            {
                //Chỉ cần 1 viên đạn, nếu nó đi quá giới hạn thì ta lại đưa nó về vị trí ban đầu (khác với mainCharacter)
                p_bullet->setBullet_is_move(true);
                p_bullet->SetRect(rect_.x , rect_.y + 10);
            }
        }
    }
}

void Threat::ClearThreatBullet (const int& index)
{
    int bulletRemaining = bulletThreat_list.size();
    //Nếu còn đạn viên đạn cần xóa nằm trong những viên còn lại
    if (bulletRemaining > 0 && index < bulletRemaining)
    {
        Bullet* bullet = bulletThreat_list.at(index);
        bulletThreat_list.erase(bulletThreat_list.begin() + index);

        if (bullet != NULL)
        {
            delete bullet;
            bullet = NULL;
        }
    }
}

void Threat::ResetBullet(Bullet* p_bullet)
{
  p_bullet->SetRect(x_threat + 20 , y_threat + 10);
  p_bullet->setBulletVal(15, 0);
}

