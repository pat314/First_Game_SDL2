
#include "MainStatus.h"

///Main Lives
MainStatus::MainStatus()
{
//    num = 0;
}

MainStatus::~MainStatus()
{

}

void MainStatus::Status_AddPosition (const int& x)
{
    position_list.push_back(x);
}

void MainStatus::Status_Init(SDL_Renderer* screen)
{
    bool success = LoadImg ("image//player_pw.png", screen);
    if (success)
    {
        if (position_list.size() > 0)
        {
            position_list.clear();
        }

        //Các vị trí ban đầu
        for (int i = 0; i < LIVE_NUM; i++)
            Status_AddPosition (20 + 40*i);
    }

}

void MainStatus::Status_Show (SDL_Renderer* screen)
{
    for (int i = 0; i < (int)(position_list.size()); i++)
    {
        rect_.x = position_list.at(i);
        rect_.y = 10;
        Render(screen);
    }
}

void MainStatus::Status_DecreaseLive ()
{
//    num--;
    position_list.pop_back();
}

void MainStatus::Status_IncreaseLive ()
{
 //   num++;
    int last_position = position_list.back();
    last_position += 40;
    position_list.push_back(last_position);
}

///Class Money:
Money::Money()
{
    money_x_position = 0;
    money_y_position = 0;
}

Money::~Money()
{

}

void Money::Money_Init(SDL_Renderer* screen)
{
    bool success = LoadImg("image//money.png", screen);
    Money_SetPosition(SCREEN_WIDTH*0.3-35, 12);
}

void Money::Money_Show(SDL_Renderer* screen)
{
    rect_.x = money_x_position;
    rect_.y = money_y_position;
    rect_.w = 40;
    rect_.h = 40;
    Render(screen);
}

///Class Watch:
Watch::Watch()
{
    Watch_x_position = 0;
    Watch_y_position = 0;
}

Watch::~Watch()
{

}

void Watch::Watch_Init(SDL_Renderer* screen)
{
    bool success = LoadImg("image//watch.png", screen);
    Watch_SetPosition(SCREEN_WIDTH - 230, 10);
}

void Watch::Watch_Show(SDL_Renderer* screen)
{
    rect_.x = Watch_x_position;
    rect_.y = Watch_y_position;

    Render(screen);
}

///Class Score:
Score::Score()
{
    Score_x_position = 0;
    Score_y_position = 0;
}

Score::~Score()
{

}

void Score::Score_Init(SDL_Renderer* screen)
{
    bool success = LoadImg("image//point.png", screen);
    Score_SetPosition(SCREEN_WIDTH*0.5 - 45, 5);
}

void Score::Score_Show(SDL_Renderer* screen)
{
    rect_.x = Score_x_position;
    rect_.y = Score_y_position;

    Render(screen);
}

///Boss HP
BossStatus::BossStatus()
{
//    num = 0;
}

BossStatus::~BossStatus()
{

}

void BossStatus::BossStatus_AddPosition (const int& x)
{
    BossStatus_position_list.push_back(x);
}

void BossStatus::BossStatus_Init(SDL_Renderer* screen)
{
    bool success = LoadImg ("image//hp.png", screen);
    if (success)
    {
        //num = 5;
        if (BossStatus_position_list.size() > 0)
        {
            BossStatus_position_list.clear();
        }

        //Các vị trí ban đầu
        for (int i = 0; i < BOSS_HP; i++)
            BossStatus_AddPosition (SCREEN_WIDTH*0.6 + 59*i);
    }

}

void BossStatus::BossStatus_Show (SDL_Renderer* screen)
{
    for (int i = 0; i < (int)(BossStatus_position_list.size()); i++)
    {
        rect_.x = BossStatus_position_list.at(i);
        rect_.y = 90;
        Render(screen);
    }
}

void BossStatus::BossStatus_DecreaseHP ()
{
//    num--;
    BossStatus_position_list.pop_back();
}

void BossStatus::BossStatus_IncreaseHP ()
{
 //   num++;
    int last_position = BossStatus_position_list.back();
    last_position += 59;
    BossStatus_position_list.push_back(last_position);
}
