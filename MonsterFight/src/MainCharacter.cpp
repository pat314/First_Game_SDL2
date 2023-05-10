
#include "MainCharacter.h"

MainCharacter::MainCharacter()
{
    i_frame = 0;
    x = 0;
    y = 180;
    x_val = 0;
    y_val = 0;
    w_frame = 0;
    h_frame = 0;
    status = W_RIGHT;
    type = {0,0,0,0,0};
    isOnGround = false;
    map_X = 0;
    map_Y = 0;
    timeRevival = 0;
    coin_count = 0;
    Main_Abyss = false;
    mainChaBulletType = T_SPHERE;
}

MainCharacter::~MainCharacter()
{

}

bool MainCharacter::LoadImg(std::string path, SDL_Renderer* screen)
{
    //Gọi hàm con đồng thời gọi cả hàm cha
    bool success = BaseObject::LoadImg(path, screen);

    //Load kich thước cho 1 frame
    if (success == true)
    {
        w_frame = rect_.w / SPRITE_NUM;
        h_frame = rect_.h;
    }

    return success;
}

void MainCharacter::set_frame()
{
    //cắt từng ảnh trong sprite rồi cho vào frame_clip (cắt ảnh ra roi luu vao frame_clip)
    if (w_frame > 0 && h_frame > 0)
    {
        for (int i = 0; i < SPRITE_NUM; i++)
        {
            frame_clip[i].x = w_frame * i;
            frame_clip[i].y = 0;
            frame_clip[i].w = w_frame;
            frame_clip[i].h = h_frame;
        }
    }
}

void MainCharacter::ShowSprite (SDL_Renderer* des)
{
    //Sẽ phải cho chạy frame tương úng với input từ bàn phím
    UpdateImg(des);

    if (type.left == 1 || type.right == 1)
    {
        i_frame ++;
    }
    else i_frame = 0;

    if (i_frame >=8) i_frame = 0;
    //vị trí = quãng đường đi được trừ đi một lượng map đã bị cuốn
    rect_.x = x - map_X;
    rect_.y = y - map_Y;

    SDL_Rect* current_frame = &frame_clip[i_frame];

    SDL_Rect renderQuad = {rect_.x, rect_.y, w_frame, h_frame};

    //Hiển thị p_object_ lên des (màn hình) với kích thước từ renderQuad và hình từ current_frame
    SDL_RenderCopy(des, p_object_, current_frame, &renderQuad);
}

void MainCharacter::HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* sound[])
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status = W_RIGHT;
                type.right = 1;
                type.left = 0;
            }
            break;

        case SDLK_LEFT:
            {
                status = W_LEFT;
                type.left = 1;
                type.right = 0;
            }
            break;
        case SDLK_UP:
            {
                type.jump = 1;
            }
            break;

        case SDLK_s:
            {
                Mix_PlayChannel (-1, sound[9], 0);
                mainChaBulletType = T_SPHERE;
            }
            break;

        case SDLK_l:
            {
                Mix_PlayChannel (-1, sound[9], 0);
                mainChaBulletType = T_LASER;
            }
            break;
        }
    }

    else if (events.type == SDL_KEYUP)
    {
                switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                type.right = 0;
            }
            break;

        case SDLK_LEFT:
            {
                type.left = 0;
            }
            break;
        }
    }

    if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_RIGHT)
        {
            type.jump = 2;
        }
        else if (events.button.button == SDL_BUTTON_LEFT)
        {

            //Tạo đạn
            Bullet* bullet = new Bullet();

            //Set trạng thái hướng đạn
            if (status == W_LEFT)
            {
                bullet ->setBullet_bulletDirection (D_LEFT);
                bullet->setBullet_bulletType (mainChaBulletType);
                bullet->LoadBulletImg (screen, D_LEFT);
                bullet -> SetRect(this->rect_.x, this ->rect_.y + h_frame*0.25);
            }
            else if (status == W_RIGHT)
            {
                bullet ->setBullet_bulletDirection (D_RIGHT);
                bullet->setBullet_bulletType (mainChaBulletType);
                bullet->LoadBulletImg (screen, D_RIGHT);
                bullet ->SetRect(this->rect_.x + w_frame - 20, this ->rect_.y + h_frame*0.25);
            }
            //Set vận tốc và trạng thái bắn đạn
            bullet->setBulletVal(20,0);
            bullet->setBullet_is_move(true);
            //"Nạp" đạn
            bulletList.push_back(bullet);
            Mix_PlayChannel (-1, sound[0], 0);
        }
    }
}

void MainCharacter:: HandleBullet (SDL_Renderer* des, Map& mapData)
{
    //Liên tục kiểm tra xem băng đạn có hay không?
    for (int i = 0; i < (int)(bulletList.size()); i++)
    {
        //Nếu có thì lấy ra viên đạn đó
        Bullet* bullet = bulletList.at(i);
        if (bullet != NULL)
        {
            //Đạn có cho phép bắn không?
            if (bullet->getBullet_is_move())
            {
                bullet->HandleBulletMove(SCREEN_WIDTH, SCREEN_HEIGHT, mapData);
                bullet->Render(des);
            }
            else
            {
                //xóa con trỏ tới viên đạn này trong danh sách đạn
                bulletList.erase(bulletList.begin() + i);
                //xóa viên đạn
                if (bullet != NULL)
                {
                    delete bullet;
                    bullet = NULL;
                }
            }
        }
    }
}

void MainCharacter::CoinCounting()
{
    coin_count++;
}

void MainCharacter::play(Map& mapData, Mix_Chunk* sound[])
{
    if (timeRevival == 0)
    {
        x_val = 0;
        //Cho nhân vật rơi với gia tốc gravity
        y_val += GRAVITY;

        //Khi nhân vật đạt tốc đô jtowis hạn thì nhân vật sẽ rơi với vận tốc không đổi
        if (y_val >= V_MAX)
        {
            y_val = V_MAX;
        }

        //Khi bấm left - right
        if (type.left == 1)
        {
            x_val -= V_PLAYER;
        }
        else if (type.right == 1)
        {
            x_val += V_PLAYER;
        }
        //Khi bấm chuột trái để nhảy lên
        if (type.jump == 1)
        {
            //Chỉ khi nào nhân vật trên mặt đất thì mới nhảy lên được
            if (isOnGround == true)
            {
                y_val = - H_JUMP;
                isOnGround = false;
            }
            type.jump = 0;
        } else if (type.jump == 2)
        {
            //Chỉ khi nào nhân vật trên mặt đất thì mới nhảy lên được
            if (isOnGround == true)
            {
                y_val = - 1.8*H_JUMP;
                isOnGround = false;
            }
            type.jump = 0;
        }

        CheckGround (mapData, sound);
        CenterMap (mapData);
    }
    //Nhân vật rơi xuống vực
    if (timeRevival > 0)//Phải reset lại trạng thái cho nv và cho nv rơi về vị trí khi còn trên mặt đất
    {
        timeRevival--;
        //Cho nhân vật rơi trở lại vị trí trước đó (khi còn đang ở trên mặt đất)
        if (timeRevival == 0)
        {
            //Nv rơi lại từ trên xuống dưới mặt đất sau khi hồi sinh nên lúc này nv k ở trên mặt đất
            isOnGround = false;
            //Nếu nhân vật rơi ở vực thẳm giữa bản đồ thì phải lùi lại 1 chút
            if (x > 4*TILE_SIZE) x -= 4*TILE_SIZE;
            else x = 0;
            y = 0;
            x_val = 0;
            y_val = 0;
        }
    }

}

void MainCharacter::CenterMap (Map& mapData)
{
    //Set vị trí trục x: Nv đi theo chiều ngang
    //Khi nhân vật di chuyển đến quá 2/3 màn hình thì map bắt đầu di chuyển
    mapData.start_x_ = x - (1*SCREEN_WIDTH/3);
    //Nếu mà nhân vật đi lùi (mà map đã kịch đầu) thì không lùi map nữa
    if (mapData.start_x_ < 0) mapData.start_x_ = 0;
    //Nếu mà nhân vật đi tiến (mà map đã kịch cuối) thì không tiến map nữa
    else if (mapData.start_x_ + SCREEN_WIDTH > mapData.max_x_) mapData.start_x_ = mapData.max_x_ - SCREEN_WIDTH;

    //Set vị trí trục y: Nv đi theo chiều dọc
    //Khi nhân vật di chuyển quá  2/3 màn hình thì map bắt đầu di chuyển
    mapData.start_y_ = y - (2*SCREEN_HEIGHT/3);
    //Nếu mà nhân vật đi xuống (mà map dã kịch cuối thì không lùi map nữa
    if (mapData.start_y_ < 0) mapData.start_y_ = 0;
    //Nếu nhân vật nhảy lên trên (mà map đã kịch trên) thì không tiến map nữa
    else if (mapData.start_y_ + SCREEN_HEIGHT > mapData.max_y_) mapData.start_y_ = mapData.max_y_ - SCREEN_HEIGHT;
}


void MainCharacter:: CheckGround (Map& mapData, Mix_Chunk* sound[])
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //Check horizontal
    //kKiếm tra xem nhân vật nằm ở ô nào trên map
    int h_min = h_frame < TILE_SIZE ? h_frame:TILE_SIZE;
    //Chỉ số ô map liền kề trước nhân vật
    x1 = (x + x_val)/TILE_SIZE;
    y1 = y/TILE_SIZE;

    //Chỉ số ô map liền kề sau nhân vật
    x2 = (x + x_val + w_frame - err)/TILE_SIZE;
    y2 = (y + h_min - err)/TILE_SIZE;

    /*
            (x1,y1)*****************(x2,y1)
               *     *           *     *
               *********** o ***********
               *     *    \|/    *     *
               *     *    / \    *     *
               *     *           *     *
               *     *           *     *
               *     *           *     *
            (x1,y2)*****************(x2,y2)
    */

    if (x1 >=0 && x2 < MAX_MAP_X && y1 > 0 && y2 < MAX_MAP_Y)
    {
        if (x_val > 0) //Nhân vật di chuyển sang phải
        {
            if (mapData.tile[y1][x2] != BLANK_TILE || mapData.tile[y2][x2] != BLANK_TILE)
            {
                if (mapData.tile[y1][x2] == 3)
                {

                    Mix_PlayChannel (-1, sound[7], 0);
                    mapData.tile[y1][x2] = 10;
                }
                else if (mapData.tile[y2][x2] == 3)
                {
                    Mix_PlayChannel (-1, sound[7], 0);
                    mapData.tile[y2][x2] = 10;
                }
                else if (mapData.tile[y1][x2] == 4)
                {
                    mapData.tile[y1][x2] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[y2][x2] == 4)
                {
                    mapData.tile[y2][x2] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[3][399] == 10) achieved_Dragon_ball ();

                else
                {
                    x = x2 * TILE_SIZE;
                    x -= w_frame + err;
                    x_val = 0;
                }
            }
        }
        else if (x_val < 0) //Nhân vật di chuyển sang trái
        {
            if (mapData.tile[y1][x1] != BLANK_TILE || mapData.tile[y2][x1] != BLANK_TILE)
            {
                if (mapData.tile[y1][x1] == 3)
                {
                    //if (mapData.tile[y1][x1] == 0) achieved_Dragon_ball ();
                    Mix_PlayChannel (-1, sound[7], 0);
                    mapData.tile[y1][x1] = 10;
                }
                else if (mapData.tile[y2][x1] == 3)
                {
                    //if (mapData.tile[y2][x1] == 0) achieved_Dragon_ball ();
                    Mix_PlayChannel (-1, sound[7], 0);
                    mapData.tile[y2][x1] == 10;
                }
                else if (mapData.tile[y1][x1] == 4)
                {
                    mapData.tile[y1][x1] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[y2][x1] == 4)
                {
                    mapData.tile[y2][x1] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[3][399] == 10) achieved_Dragon_ball ();

                else
                {
                    x = (x1 + err) * TILE_SIZE;
                    x_val = 0;
                }
            }
        }
    }

    //Check vertical
    int w_min = w_frame < TILE_SIZE? w_frame:TILE_SIZE;

    x1 = x/TILE_SIZE;
    y1 = (y + y_val)/TILE_SIZE;

    x2 = (x + w_min - err)/TILE_SIZE;
    y2 = (y + y_val + h_frame - err)/TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >=0 && y2 < MAX_MAP_Y)
    {
        if (y_val > 0) // Nhân vật đang rơi
        {
            if (mapData.tile[y2][x1] != BLANK_TILE || mapData.tile[y2][x2] != BLANK_TILE)
            {
                if (mapData.tile[y2][x1] == 3)
                {
                     //if (mapData.tile[y2][x1] == 0) achieved_Dragon_ball ();
                     mapData.tile[y2][x1] = 10;
                     Mix_PlayChannel (-1, sound[7], 0);
                }
                else if (mapData.tile[y2][x2] == 3)
                {
                    //if (mapData.tile[y2][x2] == 0) achieved_Dragon_ball ();
                    mapData.tile[y2][x2] == 10;
                    Mix_PlayChannel (-1, sound[7], 0);
                }
                else if (mapData.tile[y2][x1] == 4)
                {
                    mapData.tile[y2][x1] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[y2][x2] == 4)
                {
                    mapData.tile[y2][x2] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }

                else if (mapData.tile[y2][x1] == 5||mapData.tile[y2][x1] == 6)
                {
                    mapData.tile[y2][x1] = 0;
                }

                else if (mapData.tile[y2][x2] == 5|| mapData.tile[y2][x2] == 6)
                {
                    mapData.tile[y2][x2] = 0;
                }
                else if (mapData.tile[3][399] == 10) achieved_Dragon_ball ();

                else
                {
                    y = y2 * TILE_SIZE;
                    y -= h_frame + err;
                    y_val = 0;
                    isOnGround = true;
                }
            }
        }
        else if (y_val < 0) // Nhân vật đang nhảy lên
        {
            if (mapData.tile[y1][x1] != BLANK_TILE || mapData.tile[y1][x2] != BLANK_TILE)
            {

                if (mapData.tile[y1][x1] == 3)
                {
                    //if (mapData.tile[y1][x1] == 0) achieved_Dragon_ball ();
                    mapData.tile[y1][x1] = 10;
                    Mix_PlayChannel (-1, sound[7], 0);
                }
                else if (mapData.tile[y1][x2] == 3)
                {
                    //if (mapData.tile[y1][x2] == 0) achieved_Dragon_ball ();
                    mapData.tile[y1][x2] = 10;
                    Mix_PlayChannel (-1, sound[7], 0);
                }
                else if (mapData.tile[y1][x1] == 4)
                {
                    mapData.tile[y1][x1] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[y1][x2] == 4)
                {
                    mapData.tile[y1][x2] = 0;
                    CoinCounting();
                    Mix_PlayChannel (-1, sound[6], 0);
                }
                else if (mapData.tile[3][399] == 10) achieved_Dragon_ball ();

                else
                {
                    y = (y1 + err)* TILE_SIZE;
                    y_val = 0;
                    isOnGround = false;
                }
            }
        }
    }

    x += x_val;
    y += y_val;

    if (x < 0) x = 0;
    else if (x + w_frame > mapData.max_x_) x = mapData.max_x_ - w_frame - err;
    //Nếu nhân vật bị rơi xuống vực thẳm
    if (y > mapData.max_y_)
    {
        timeRevival = 60;

        set_Abyss (true);
    }
}

void MainCharacter::UpdateImg (SDL_Renderer* des)
{
    if (isOnGround == true)
    {
        if (status == W_LEFT)
        {
            LoadImg("image//player_left.png", des);
        }
        else
        {
            LoadImg("image//player_right.png", des);
        }
    }
    else
    {
        if (status == W_LEFT)
        {
            LoadImg("image//jum_left.png", des);
        }
        else
        {
            LoadImg("image//jum_right.png", des);
        }
    }
}

void MainCharacter::ClearBullet (const int& index)
{
    int bulletRemaining = bulletList.size();
    //Nếu còn đạn viên đạn cần xóa nằm trong những viên còn lại
    if (bulletRemaining > 0 && index < bulletRemaining)
    {
        Bullet* bullet = bulletList.at(index);
        bulletList.erase(bulletList.begin() + index);

        if (bullet != NULL)
        {
            delete bullet;
            bullet = NULL;
        }
    }
}
