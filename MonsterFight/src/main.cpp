//hàm chứa hàm main và các hàm thiết lập cho môi trường SDL
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainCharacter.h"
#include "Timer.h"
#include "Threat.h"
#include "Explosion.h"


BaseObject g_background;

//Hàm khỏi tạo các thông số cho môi trường của SDL
bool initData ()
{
    bool success = true;
    //Hàm SDL_Init() với hằng số SDL_INIT_VIDEO sẽ thiết lập môi trg ban đầu cho SDL, hàm trả về 1 số nguyên tương ứng với trạng thái
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    //Thiết lập chế độ tỉ lệ và chất lượng
    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow ("Monster Fight!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL)  success = false;
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) success = false;
        else
        {
            SDL_SetRenderDrawColor (g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) success = false;
        }
    }
    return success;
}

//Hàm load ảnh background (load ảnh nói chung nên dùng đối tượng BaseObject)
bool LoadBackground ()
{
    bool ret = g_background.LoadImg("image//background2.png", g_screen);
    if (ret) return true;
    return false;
}

void close ()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

std::vector<Threat*> MakeThreatList()
{
    std::vector<Threat*> listThreat;

    Threat* animationThreat = new Threat[THREAT_NUM];
    for (int i = 0; i < THREAT_NUM; i++)
    {
        Threat* pThreat = (animationThreat + i);
        if (pThreat != NULL)
        {
            pThreat->loadThreat_img("image//threat_left.png", g_screen);
            pThreat->setThreat_frame();
            pThreat->initThreat();
            pThreat->setThreat_position((1350*i+6085)%(MAX_MAP_X*TILE_SIZE), 200);
            pThreat->setThreat_typeMove (MOVING_THREAT);
            //Threat sẽ di chuyển trong khoảng có độ dài 120 từ pos1 đến pos2
            int pos1 = pThreat->getThreat_x_pos() - 60;
            int pos2 = pThreat->getThreat_x_pos() + 60;
            pThreat->setThreat_animationInterval (pos1, pos2);
            pThreat->setThreat_inputLeft(1);

            listThreat.push_back(pThreat);
        }
    }


    Threat* objectThreat = new Threat[THREAT_NUM];

    for (int i = 0; i < THREAT_NUM; i++)
    {
        Threat* p_Threat = (objectThreat + i);
        if (p_Threat != NULL)
        {
            p_Threat->loadThreat_img("image//threat_level.png", g_screen);
            p_Threat->setThreat_frame();
            p_Threat->initThreat();
            p_Threat->setThreat_inputLeft(0);
            //2250 3385
            p_Threat->setThreat_position(730+ (2250*i+3385)%(MAX_MAP_X*TILE_SIZE), 250);
            p_Threat->setThreat_typeMove (STATIC_THREAT);

            Bullet* p_bullet = new Bullet();
            p_Threat -> initThreat_bullet (p_bullet, g_screen);

            listThreat.push_back(p_Threat);
        }
    }
    return listThreat;
}

int main(int arc, char* argv[])
{
    int monsterKilledCount = 0;
    Timer fpsTimer;

    if (!initData()) return -1;
    if (!LoadBackground()) return -1;

    //Đối tượng tạo map
    GameMap game_map;
    //Load file lưu trữ trạng thái map
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);

    //Khai báo đối tượng
    MainCharacter player;
    player.LoadImg("image//player_right.png", g_screen);
    player.set_frame();

    //Khai báo threat
    std::vector<Threat*> ThreatList = MakeThreatList();

    Map mapData = game_map.getMap();

    Explosion threatExplosion;
    bool expSuccess = threatExplosion.Explosion_LoadImg("image//exp3.png", g_screen);

    if (!expSuccess) return -1;
    //Tạo frame
    threatExplosion.setExplosion_frame();

    Explosion playerExplosion;
    bool expSuccess_1 = playerExplosion.Explosion_LoadImg("image//exp3.png", g_screen);

    if (!expSuccess_1) return -1;
    //Tạo frame
    playerExplosion.setExplosion_frame();

    int death_num = 0;

    bool is_quit = false;
    while (!is_quit)
    {
        fpsTimer.start();
        while (SDL_PollEvent (&g_event))
        {
            if (g_event.type == SDL_QUIT) is_quit = true;

            player.HandleInputAction (g_event, g_screen);
        }

        //Set lại màu cho màn hình
        SDL_SetRenderDrawColor (g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        //Xóa màn hình
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);



        player.getMapCoordination (mapData.start_x_, mapData.start_y_);

        player.HandleBullet(g_screen, mapData);

        player.play (mapData);

        player.ShowSprite (g_screen);
        for (int i = 0; i < ThreatList.size(); i++)
        {
            Threat* pThreat = ThreatList.at(i);
            if (pThreat != NULL)
            {
                pThreat->getThreat_MapXY(mapData.start_x_, mapData.start_y_);
                pThreat->ImplementMoveType_threat(g_screen);
                pThreat->doThreat(mapData, g_screen);
                pThreat->MakeBullet (g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, mapData);
                pThreat->showThreat_sprite(g_screen);

                //Kiếm tra va chạm của nhân vật với đạn threat
                SDL_Rect playerRect = {player.GetRect().x, player.GetRect().y, player.GetRect().w/SPRITE_NUM, player.GetRect().h};
                bool Collision_2 = false;
                std::vector<Bullet*> bulletThreat_array = pThreat->getThreat_bulletList();
                for (int k = 0; k < bulletThreat_array.size(); k++)
                {

                    Bullet* t_bullet = bulletThreat_array.at(k);
                    if (t_bullet != NULL)
                    {
                        Collision_2 = SDLCommonFunction::CheckCollision(t_bullet->GetRect(), playerRect);
                        if (Collision_2)
                        {
                            pThreat->ResetBullet(t_bullet);
                            break;
                        }
                    }
                }

                //Kiểm tra va chạm của nhân vật với threat
                SDL_Rect threatRect = {(pThreat->GetRect()).x,(pThreat->GetRect()).y, ((pThreat->GetRect()).w)/SPRITE_NUM, (pThreat->GetRect()).h};

                bool Collision_3 = SDLCommonFunction::CheckCollision(playerRect, threatRect);

                if (Collision_2 || Collision_3)
                {
                    //Lấy frame của ảnh vụ nổ
                    int exp_width = playerExplosion.getExplosion_w_frame();
                    int exp_height = playerExplosion.getExplosion_h_frame();

                    for (int m = 0; m < EXPLOSION_FRAME_NUM; m++)
                    {
                        //Vị trí chèn ảnh hiệu ứng vụ nổ: trừ đi để tâm ảnh va đúng giữa vị trí trúng đạn
                        int x_pos = (player.GetRect().x + player.getPlayer_w_frame()*0.5) - exp_width*0.5;
                        int y_pos = (player.GetRect().y + player.getPlayer_h_frame()*0.5) - exp_height*0.5;
                        //Set khung hình nào được chiếu lên?
                        playerExplosion.setExplosion_frameIndex(m);
                        //Chiếu khung hình vừa set lên bằng ExplosionShowSprite với vị trí x_pos, y_pos từ SetRect()
                        playerExplosion.SetRect(x_pos, y_pos);
                        playerExplosion.Explosion_ShowSprite (g_screen);
                    }

                    death_num ++;
                    if (death_num <= LIVE_NUM)
                    {
                        //Set lại vị trí cho player
                        player.SetRect(0, 0);
                        player.setPlayer_revivalTime(1);
                        SDL_Delay(1000);
                        continue;
                    }

                    else
                    {
                        if(MessageBox(NULL, "GAME OVER", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                        {
                            pThreat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                    }


                }
            }
        }

        //Xử lý va chạm giữa đạn của nhân vật game với quái vật
        std::vector <Bullet*> bullet_array = player.getBulletList();
        for (int r = 0; r < bullet_array.size(); r++)
        {
            //Lấy frame của ảnh vụ nổ
            int exp_width = threatExplosion.getExplosion_w_frame();
            int exp_height = threatExplosion.getExplosion_h_frame();

            //Lấy một viên đạn của player ra
            Bullet* bullet = bullet_array.at(r);
            if (bullet != NULL)
            {
                for (int t = 0; t < ThreatList.size(); t++)
                {
                    //Kiểm tra từng threat
                    Threat* threatObj = ThreatList.at(t);
                    if (threatObj != NULL)
                    {
                        //Lấy vị trí và kích thước của 1 frame threat
                        SDL_Rect tRect = {threatObj->GetRect().x, threatObj->GetRect().y, threatObj->getThreat_w_frame(), threatObj->getThreat_h_frame()};
                        //Lấy vị trí và kích thước của bullet
                        SDL_Rect bRect = bullet->GetRect();
                        //Kiểm tra va chạm giữa threat và viên đạn
                        bool Collision_1;
                        Collision_1 = SDLCommonFunction::CheckCollision(bRect, tRect);
                        if (Collision_1)
                        {
                            monsterKilledCount++;
                            //trước khi quái vật biến mất thì chèn hiệu ứng vụ nổ
                            for (int k = 0; k < EXPLOSION_FRAME_NUM; k++)
                            {
                                //Vị trí chèn ảnh hiệu ứng vụ nổ: trừ đi để tâm ảnh va đúng giữa vị trí trúng đạn
                                int x_pos = bullet->GetRect().x - exp_width*0.5;
                                int y_pos = bullet->GetRect().y - exp_height*0.5;
                                //Set khung hình nào được chiếu lên?
                                threatExplosion.setExplosion_frameIndex(k);
                                //Chiếu khung hình vừa set lên bằng ExplosionShowSprite với vị trí x_pos, y_pos từ SetRect()
                                threatExplosion.SetRect(x_pos, y_pos);
                                threatExplosion.Explosion_ShowSprite (g_screen);

                            }
                            //Nếu mà trúng rồi thì đạn và threat biến mất
                            player.ClearBullet(r);
                            threatObj->Free();
                            ThreatList.erase (ThreatList.begin() + t);
                        }

                    }
                }
            }
        }

        if (monsterKilledCount == 2*THREAT_NUM)
        {
            SDL_Delay(1000);
            if(MessageBox(NULL, "YOU WIN", "Info", MB_OK | MB_ICONINFORMATION) == IDOK)
                        {
                            //pThreat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
        }

        game_map.setMap(mapData);
        game_map.DrawMap (g_screen);



        SDL_RenderPresent(g_screen);

        int realTime = fpsTimer.get_tick();

        int timePF = 1000/FPS; //(ms)
        //Nếu mà thời gian thực chạy nhanh hơn FPS thì phải tạo độ trễ
        if (realTime < timePF)
        {
            int timeDelay = timePF - realTime;
            //Hàm tạo ra độ trễ cho ảnh
            SDL_Delay(timeDelay);
        }
    }

    for (int i = 0; i < ThreatList.size(); i++)
    {
        Threat* p_threat = ThreatList.at(i);
        if (p_threat != NULL)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    ThreatList.clear();

    close();
    return 0;
}
