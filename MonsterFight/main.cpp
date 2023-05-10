//hàm chứa hàm main và các hàm thiết lập cho môi trường SDL
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainCharacter.h"
#include "Timer.h"
#include "Threat.h"
#include "Explosion.h"
#include "Text.h"
#include "MainStatus.h"
#include "Geometry.h"
#include "Boss.h"


BaseObject g_background;
SDL_Surface* g_menu;
SDL_Surface* g_noti = NULL;

//Đối tượng font chữ
TTF_Font* g_font_1 = NULL;
TTF_Font* g_font_2 = NULL;
TTF_Font* g_font_3 = NULL;

///Menu
bool CheckMouseFocusWithRect (const int& x, const int& y, const int& rect_x, const int& rect_y, const int& rect_w, const int& rect_h)
{
    if (x >= rect_x && x <= rect_x + rect_w &&
        y >= rect_y && y <= rect_y +rect_h) return true;
    return false;
}


//return 2: thoát chương trình
int ShowMenu (SDL_Surface* screen, TTF_Font* font)
{
    //Load ảnh menu

    g_menu =IMG_Load ("image//menu.png");
    if (g_menu == NULL) return 2;
    //Khởi tạo các khối quản lý vị trí của các item
    SDL_Rect item_position_arr[MENU_ITEM_NUMBER];
    for (int i = 0; i < MENU_ITEM_NUMBER; i++)
    {
        item_position_arr[i].x = SCREEN_WIDTH/4;
        item_position_arr[i].y = 175 + 80*i;
    }
    //Khởi tạo nhãn cho item

    Text text_menu[MENU_ITEM_NUMBER];
    text_menu[0].Text_SetContent ("PLAY!");
    text_menu[0].Text_SetColorType (WHITE_TEXT);

    text_menu[1].Text_SetContent ("INSTRUCTIONS & HOW TO PLAY");
    text_menu[1].Text_SetColorType (WHITE_TEXT);

    text_menu[2].Text_SetContent ("EXIT");
    text_menu[2].Text_SetColorType (WHITE_TEXT);


    //Kiểm tra xem con trỏ chuột có đang trỏ tới nhãn item không
    bool is_selected [MENU_ITEM_NUMBER] = {0, 0, 0};

    //biến lấy tọa độ của chuột, dùng trong phần SDL_MOUSEMOTION
    int mouse_x = 0;
    int mouse_y = 0;

    //Tạo ra vòng lặp để lấy sự kiện của chuột trên màn hình menu
    SDL_Event m_event;

    while (true)
    {
        //Đầu tiên, load ảnh menu lên màn hình

        SDL_BlitSurface (g_menu, NULL, screen, NULL);

        //Load nhãn menu: VỀ nhà tra render text lên trên surface
        for (int i = 0; i < MENU_ITEM_NUMBER; i++)
        {
            text_menu[i].Text_TexttoSurface (font, screen, item_position_arr[i], g_window);
        }
        SDL_UpdateWindowSurface (g_window);

        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 2;
                break;
            case SDL_MOUSEMOTION:
                {
                    mouse_x = m_event.motion.x;
                    mouse_y = m_event.motion.y;

                    //Kiểm tra xem con trỏ chuột có trỏ đến nhãn item không (để chuyển màu nhãn item)
                    for (int i = 0; i < MENU_ITEM_NUMBER; i++)
                    {
                        if (CheckMouseFocusWithRect (mouse_x, mouse_y, item_position_arr[i].x, item_position_arr[i].y, text_menu[i].Text_GetWidth(), text_menu[i].Text_GetHeight()))
                        {
                            //Nếu chưa đổi màu thì đổi màu + lưu trạng thái là đang trỏ tới
                            if (is_selected[i] == false)
                            {
                                is_selected[i] = true;
                                text_menu[i].Text_SetColorType(RED_TEXT);
                            }
                        }
                        else
                        {
                            //Nếu con trỏ đang không trỏ tới thì trả lại trạng thái ban đầu
                            if (is_selected[i] == true)
                            {
                                is_selected[i] = false;
                                text_menu[i].Text_SetColorType(WHITE_TEXT);
                            }
                        }
                    }
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                {
                    mouse_x = m_event.button.x;
                    mouse_y = m_event.button.y;
                    for (int i = 0; i < MENU_ITEM_NUMBER; i++)
                    {
                        if (CheckMouseFocusWithRect (mouse_x, mouse_y, item_position_arr[i].x, item_position_arr[i].y, text_menu[i].Text_GetWidth(), text_menu[i].Text_GetHeight()))
                        {
                            return i;
                            SDL_FreeSurface (g_menu);
                            g_menu = NULL;
                        }
                    }
                }
                break;

            case SDL_KEYDOWN:
                {
                    //Nút Esc
                    if (m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        return 2;
                        SDL_FreeSurface (g_menu);

                        g_menu = NULL;
                    }
                }
                break;
            }
        }
    }
    return 2;
}
///Menu - End

//Hàm khỏi tạo các thông số cho môi trường của SDL
bool initData ()
{
    bool success = true;
    //Hàm SDL_Init() với hằng số SDL_INIT_VIDEO sẽ thiết lập môi trg ban đầu cho SDL, hàm trả về 1 số nguyên tương ứng với trạng thái
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;

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


        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) success = false;
        //Khởi tạo audio
        g_SFX[0] = Mix_LoadWAV ("sfx//main_bullet.wav");
        g_SFX[1] = Mix_LoadWAV ("sfx//threat_bullet.wav");
        g_SFX[2] = Mix_LoadWAV ("sfx//boss_bullet.wav");

        g_SFX[3] = Mix_LoadWAV ("sfx//main_death.wav");
        g_SFX[4] = Mix_LoadWAV ("sfx//threat_explosion.wav");
        g_SFX[5] = Mix_LoadWAV ("sfx//main_explosion.wav");

        g_SFX[6] = Mix_LoadWAV ("sfx//earning_coin.wav");
        g_SFX[7] = Mix_LoadWAV ("sfx//earning_dragon_ball.wav");

        g_SFX[8] = Mix_LoadWAV ("sfx//running_sounds.wav");
        g_SFX[9] = Mix_LoadWAV ("sfx//change_gun.wav");

        g_menu_music = Mix_LoadMUS ("mus//menu_music.wav");
        g_game_music = Mix_LoadMUS ("mus//game_music.wav");

        for (int i = 0; i < 6; i++) if (g_SFX[i] == NULL) success = false;
        if (g_game_music == NULL || g_menu_music == NULL) success = false;

        //Khởi tạo điều kiện của text 1
        if (TTF_Init() == -1 ) success = false;
        //Lấy font chữ từ folder
        g_font_1 = TTF_OpenFont("font//BruceForeverRegular.ttf", FONT_SIZE_1);
        if (g_font_1 == NULL) success = false;

        //Khởi tạo điều kiện của text 2
        g_font_2 = TTF_OpenFont("font//information.ttf", FONT_SIZE_2);
        if (g_font_2 == NULL) success = false;

        //Khởi tạo điều kiện của text 3
        g_font_3 = TTF_OpenFont("font//menu.ttf", FONT_SIZE_3);
        if (g_font_3 == NULL) success = false;
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

    //Free sound effect

    for (int i = 0; i < 10; i++)
    {
        Mix_FreeChunk (g_SFX[i]);
    }
    //Free mussic
    Mix_FreeMusic (g_menu_music);
    Mix_FreeMusic (g_game_music);

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;
    TTF_Quit();
    Mix_Quit();
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
    bool exit = false;

    bool is_quit = false;


    Timer fpsTimer;

    if (!initData()) return -1;
    if (!LoadBackground()) return -1;



    while (!exit)
    {
        Uint32 time_in_menu = SDL_GetTicks();

        //Show menu trước khi chạy game
        SDL_Surface* g_windowSurface = SDL_GetWindowSurface (g_window);
        Mix_PlayMusic (g_menu_music, -1);

        int menu_option = ShowMenu(g_windowSurface, g_font_3);
        if (menu_option == 2)
            exit = true;
        else if (menu_option == 1)
        {
            SDL_Surface* instruction = NULL;
            instruction = IMG_Load ("image//instructions.png");
            SDL_Rect instruction_rect = {0, 0, 1280, 640};

            bool back_menu = false;
            while (!back_menu)
            {   SDL_Event event;
                SDL_BlitSurface(instruction, NULL, g_windowSurface, &instruction_rect);
                SDL_UpdateWindowSurface (g_window);
                while (SDL_PollEvent (&event))
                switch (event.type)
                {
                case SDL_QUIT:
                    exit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        SDL_FreeSurface (instruction);
                        back_menu = true;
                    }
                }
            }
        }
        else if (menu_option == 0)
        {

            int monsterKilledCount = 0;

            //Đối tượng tạo map
            GameMap game_map;
            //Load file lưu trữ trạng thái map
            game_map.LoadMap("map//map01.dat");
            game_map.LoadTiles(g_screen);

            //Khai báo đối tượng
            MainCharacter player;
            player.LoadImg("image//player_right.png", g_screen);
            player.set_frame();

            //Đối tượng tạo các hình minh họa mạng cho nv chính
            MainStatus main_status;
            main_status.Status_Init(g_screen);

            //Đối tượng tạo ra các hình minh họa đồng tiền
            Money main_money;
            main_money.Money_Init(g_screen);

            //Đối tượng tạo ra các hình minh họa đồng hồ
            Watch watch;
            watch.Watch_Init(g_screen);

            //Đối tượng tạo ra các hình minh họa điểm số
            Score score;
            score.Score_Init(g_screen);

            //Đối tượng tạo ra các hình minh họa HP của Boss
            BossStatus HP;
            HP.BossStatus_Init(g_screen);

            //Khai báo threat
            std::vector<Threat*> ThreatList = MakeThreatList();

            //Khai báo đối tượng Boss
            BossObject boss;
            boss.Boss_LoadImg ("image//boss_object.png", g_screen);
            boss.set_clips();
            int Boss_x0 = 394 * TILE_SIZE;
            boss.set_xpos (Boss_x0);
            boss.set_ypos (10);

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

            Explosion BossExplosion;
            bool expSuccess_2 = BossExplosion.Explosion_LoadImg("image//exp3.png", g_screen);

            if (!expSuccess_2) return -1;
            //Tạo frame
            BossExplosion.setExplosion_frame();

            //Biến lưu số lần main chết
            int death_num = 0;

            //Biến lưu số lần Boss chết
            int Boss_death_num = 0;

            //Time text
            Text time_game;
            time_game.Text_SetColorType (WHITE_TEXT);

            //Mark text
            Text mark_game;
            mark_game.Text_SetColorType (WHITE_TEXT);
            UINT mark_count = 0;

            //Money text
            Text money_game;
            money_game.Text_SetColorType (WHITE_TEXT);

            //Boss HP text
            Text Boss_HP;
            Boss_HP.Text_SetColorType (BLACK_TEXT);


            is_quit = false;
            Mix_PlayMusic (g_game_music, -1);

            while (!is_quit)
            {

                fpsTimer.start();
                while (SDL_PollEvent (&g_event))
                {
                    if (g_event.type == SDL_QUIT)
                    {
                        is_quit = true;
                        exit = true;
                    }

                    player.HandleInputAction (g_event, g_screen, g_SFX);
                }

                //Set lại màu cho màn hình
                SDL_SetRenderDrawColor (g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
                //Xóa màn hình
                SDL_RenderClear(g_screen);

                g_background.Render(g_screen, NULL);

                player.getMapCoordination (mapData.start_x_, mapData.start_y_);

                player.HandleBullet(g_screen, mapData);

                player.play (mapData, g_SFX);

                player.ShowSprite (g_screen);

                //Draw Geometry
                GeometryFormat G_rect_size (0, 0, SCREEN_WIDTH, 60);
                ColorGeometry G_rect_color (49, 18, 105);
                Geometry::RenderRectangle (G_rect_size, G_rect_color, g_screen);

                for (int i = 0; i < 5; i++)
                {
                    GeometryFormat G_outline1_size (i, i, SCREEN_WIDTH - i*2, 60 - 2*i);
                    ColorGeometry G_outline1_color (120, 45, 255);
                    Geometry::RenderOutline (G_outline1_size, G_outline1_color, g_screen);
                }

                //Hiển thị các hình minh họa lên màn hình
                main_status.Status_Show(g_screen);
                main_money.Money_Show(g_screen);
                watch.Watch_Show(g_screen);
                score.Score_Show(g_screen);

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
                            Mix_PlayChannel (-1, g_SFX[5], 0);
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

                                    if (Collision_2)
                                    {
                                        if (mark_count >=5)
                                        mark_count -= 5;
                                        else mark_count = 0;
                                    }
                                    else if (Collision_3)
                                    {
                                        if (mark_count >=3)
                                        mark_count -= 3;
                                        else mark_count = 0;
                                    }

                                //Giảm đi 1 hình minh họa mạng nhân vật
                                main_status.Status_DecreaseLive();
                                main_status.Render(g_screen);
                                //Set lại vị trí cho player
                                player.SetRect(0, 0);
                                player.setPlayer_revivalTime(1);
                                SDL_Delay(1000);
                                continue;
                            }

                            else
                            {
                                //if(MessageBox(NULL, "YOU DIED!", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                                //{
                                    pThreat->Free();
                                    g_noti = IMG_Load ("image//gameover.png");
                                    SDL_BlitSurface (g_noti, NULL, g_windowSurface, NULL);
                                    SDL_UpdateWindowSurface (g_window);
                                    Mix_PauseMusic();
                                    SDL_Delay (5000);

                                    SDL_FreeSurface (g_noti);
                                    is_quit = true;
                                    break;
                                //}
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
                                    Mix_PlayChannel (-1, g_SFX[4], 0);
                                    if (tRect.h == 50) mark_count++;
                                    else if (tRect.h == 60) mark_count +=3;
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

                //check rơi xuống vực thẳm để trừ 1 mạng
                bool check = player.get_Abyss();
                if (check)
                {
                    Mix_PlayChannel (-1, g_SFX[3], 0);
                    death_num ++;
                    if (mark_count >=2) mark_count -= 2;
                    else mark_count = 0;
                    //Giảm đi 1 hình minh họa mạng nhân vật
                    main_status.Status_DecreaseLive();
                    main_status.Render(g_screen);

                    player.set_Abyss(false);



                if (death_num > LIVE_NUM)
                {
                    //if(MessageBox(NULL, "YOU DIED!", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                    //{
                        //pThreat->Free();
                        g_noti = IMG_Load ("image//gameover.png");
                        SDL_BlitSurface (g_noti, NULL, g_windowSurface, NULL);
                        SDL_UpdateWindowSurface (g_window);
                        Mix_PauseMusic();
                        SDL_Delay (5000);

                        SDL_FreeSurface (g_noti);

                        is_quit = true;
                        break;
                    //}
                }
                }

                if (monsterKilledCount == 2*THREAT_NUM)
                {
                    mapData.tile[3][399] = 3;

                    //Show boss
                    boss.SetMapXY(mapData.start_x_, mapData.start_y_);
                    //Boss rơi xuống
                    boss.DoBoss (mapData);
                    //Boss bắn đạn
                    boss.MakeBullet (g_screen, SCREEN_WIDTH*0.67, SCREEN_HEIGHT, mapData, g_SFX[2]);
                    boss.Show (g_screen);
                    HP.BossStatus_Show(g_screen);

                    //Show Boss HP text
                    std::string Text_HP = "BOSS: ";

                    Boss_HP.Text_SetContent(Text_HP);
                    Boss_HP.Text_TextToTexture(g_font_1,g_screen);
                    Boss_HP.Text_Render(g_screen, SCREEN_WIDTH * 0.6 - 85, 84);

                    ///Xử lý va chạm nv và boss

                    //Đạn main với Boss

                    for (int r = 0; r < bullet_array.size(); r++)
                    {
                        //Lấy frame của ảnh vụ nổ
                        int exp_width = BossExplosion.getExplosion_w_frame();
                        int exp_height = BossExplosion.getExplosion_h_frame();

                        //Lấy một viên đạn của player ra
                        Bullet* bullet = bullet_array.at(r);
                        if (bullet != NULL)
                        {
                            //Lấy vị trí và kích thước của 1 frame boss
                            SDL_Rect BossRect = {(boss.GetRect()).x,(boss.GetRect()).y, ((boss.GetRect()).w)/FRAME_NUM_32, (boss.GetRect()).h};
                            //Lấy vị trí và kích thước của bullet
                            SDL_Rect bRect = bullet->GetRect();
                            //Kiểm tra va chạm giữa threat và viên đạn
                            bool Collision_1;
                            Collision_1 = SDLCommonFunction::CheckCollision(bRect, BossRect);
                            if (Collision_1)
                            {
                                Mix_PlayChannel (-1, g_SFX[4], 0);
                                mark_count += 20;
                                Boss_death_num++;
                                HP.BossStatus_DecreaseHP();
                                HP.Render(g_screen);
                                //trước khi quái vật biến mất thì chèn hiệu ứng vụ nổ
                                for (int k = 0; k < EXPLOSION_FRAME_NUM; k++)
                                {
                                    //Vị trí chèn ảnh hiệu ứng vụ nổ: trừ đi để tâm ảnh va đúng giữa vị trí trúng đạn
                                    int x_pos = bullet->GetRect().x - exp_width*0.5;
                                    int y_pos = bullet->GetRect().y - exp_height*0.5;
                                    //Set khung hình nào được chiếu lên?
                                    BossExplosion.setExplosion_frameIndex(k);
                                    //Chiếu khung hình vừa set lên bằng ExplosionShowSprite với vị trí x_pos, y_pos từ SetRect()
                                    BossExplosion.SetRect(x_pos, y_pos);
                                    BossExplosion.Explosion_ShowSprite (g_screen);

                                }
                                //Nếu mà trúng rồi thì đạn và nếu boss bị bắn hết HP thì boss biến mất
                                player.ClearBullet(r);
                                if (Boss_death_num == BOSS_HP)
                                {
                                    boss.Free();
                                    boss.Boss_Free();
                                    boss.ClearBullet();
                                }
                            }
                        }
                    }

                    //Kiếm tra va chạm của nhân vật với đạn boss
                    SDL_Rect playerRect = {player.GetRect().x, player.GetRect().y, player.GetRect().w/SPRITE_NUM, player.GetRect().h};
                    bool Collision_2 = false;
                    std::vector<Bullet*> bulletBoss_array = boss.get_bullet_list();
                    for (int k = 0; k < bulletBoss_array.size(); k++)
                    {

                        Bullet* b_bullet = bulletBoss_array.at(k);
                        if (b_bullet != NULL)
                        {
                            Collision_2 = SDLCommonFunction::CheckCollision(b_bullet->GetRect(), playerRect);
                        }
                    }

                    //Kiểm tra va chạm của nhân vật với boss

                    //Lấy vị trí và kích thước của 1 frame boss
                    SDL_Rect BossRect = {(boss.GetRect()).x,(boss.GetRect()).y, ((boss.GetRect()).w)/FRAME_NUM_32, (boss.GetRect()).h};

                    bool Collision_3 = SDLCommonFunction::CheckCollision(playerRect, BossRect);

                    if (Collision_2 || Collision_3)
                    {
                        Mix_PlayChannel (-1, g_SFX[5], 0);
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

                                if (Collision_2)
                                {
                                    if (mark_count >=10)
                                    mark_count -= 10;
                                    else mark_count = 0;
                                }
                                else if (Collision_3)
                                {
                                    if (mark_count >=6)
                                    mark_count -= 6;
                                    else mark_count = 0;
                                }

                            //Giảm đi 1 hình minh họa mạng nhân vật
                            main_status.Status_DecreaseLive();
                            main_status.Render(g_screen);
                            //Set lại vị trí cho player
                            player.SetRect(0, 0);
                            player.setPlayer_revivalTime(1);
                            SDL_Delay(1000);
                            continue;
                        }

                        else
                        {
                            //if(MessageBox(NULL, "YOU DIED!", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                            //{
                                g_noti = IMG_Load ("image//gameover.png");
                                SDL_BlitSurface (g_noti, NULL, g_windowSurface, NULL);
                                SDL_UpdateWindowSurface (g_window);
                                Mix_PauseMusic();
                                SDL_Delay (5000);

                                SDL_FreeSurface (g_noti);

                                is_quit = true;
                                break;
                            //}
                        }
                    }
                }

                if (player.check_achieve_Dragon_ball())
                {
                    //if(MessageBox(NULL, "YOU WIN", "Info", MB_OK | MB_ICONINFORMATION) == IDOK)
                    //{
                        g_noti = IMG_Load ("image//youwin.png");
                        SDL_BlitSurface (g_noti, NULL, g_windowSurface, NULL);
                        SDL_UpdateWindowSurface (g_window);
                        Mix_PauseMusic();
                        SDL_Delay (5000);

                        SDL_FreeSurface (g_noti);

                        is_quit = true;
                        break;
                    //}
                }

                game_map.setMap(mapData);
                game_map.DrawMap (g_screen);

                //Show game time
                std::string Text_time = " ";
                Uint32 time_elapse_val = (SDL_GetTicks() - time_in_menu) / 1000;
                Uint32 time_countdown_val = MAX_TIME_PLAYING - time_elapse_val;

                if (time_countdown_val <= 0)
                {
                    //Kết thúc game
                    //if(MessageBox(NULL, "TIME UP!", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                    //{
                        g_noti = IMG_Load ("image//gameover.png");
                        SDL_BlitSurface (g_noti, NULL, g_windowSurface, NULL);
                        SDL_UpdateWindowSurface (g_window);
                        Mix_PauseMusic();
                        SDL_Delay (5000);

                        SDL_FreeSurface (g_noti);

                        is_quit = true;
                        break;
                    //}
                }
                else
                {
                    std::string string_time_content = std::to_string (time_countdown_val);
                    Text_time += string_time_content;
                    //khởi tạo text time cho game
                    time_game.Text_SetContent(Text_time);
                    //Chuyển chữ thành ảnh để hiển thị lên màn hình
                    time_game.Text_TextToTexture (g_font_1, g_screen);
                    //Hiển thị text (bằng ảnh) lên màn hình
                    time_game.Text_Render(g_screen, SCREEN_WIDTH - 200, 20);
                }

                //Show game mark
                std::string Text_mark = " ";
                std::string string_mark_content = std::to_string (mark_count);
                Text_mark += string_mark_content;

                mark_game.Text_SetContent(Text_mark);
                mark_game.Text_TextToTexture(g_font_1,g_screen);
                mark_game.Text_Render(g_screen, SCREEN_WIDTH * 0.5, 20);

                //Show game coin
                int money_count = player.Main_getCoinCount();
                std::string Text_coin = " ";
                std::string string_coin_content = std::to_string (money_count);
                Text_coin += string_coin_content;

                money_game.Text_SetContent(Text_coin);
                money_game.Text_TextToTexture(g_font_1,g_screen);
                money_game.Text_Render(g_screen, SCREEN_WIDTH * 0.3, 20);

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


        }
    }



    close();
    return 0;
}
