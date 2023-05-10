//hàm tạo các thống số, hàm chung cho chương trình
//Header Guard
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

//Các thư viện sẽ được include tại đây
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

//Các biến toàn chương trình (sử dụng static)
static SDL_Window* g_window = NULL;
//Đối tượng để load ảnh lên, hiểu như là nền
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//Thông số Screen
const double FPS = 60;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

//Thông số màu nền ảnh (để xóa nền)
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

//Thông số cho Tile map
const int TILE_SIZE = 64;
const int MAX_TILES = 10;
const int MAX_MAP_X = 400;
const int MAX_MAP_Y = 10;
const int BLANK_TILE = 0;
const int COIN_TILE = 4;

//Struct luu thong so cua map trong tile map
struct Map
{
    //Vị trí của ô
    int start_x_;
    int start_y_;
    //Chiều dài tối đa của bản đồ
    int max_x_;
    int max_y_;
    //Mảng lưu trạng thái của các ô
    int tile [MAX_MAP_Y][MAX_MAP_X];
    //Tên file
    std::string file_name;
};

//Thong so cho sprite
const int SPRITE_NUM = 8;


//Struct danh cho viec di chuyen nhan vat chinh
struct Control
{
    int left;
    int right;
    int up;
    int down;
    int jump;
};

//Thong so cho viec di chuyen
enum PlayerStatusType
{
    W_LEFT = 0,
    W_RIGHT = 1,
};

const double GRAVITY = 0.7;
const double V_MAX = 10;
const double V_PLAYER = 4;
const int H_JUMP = 10;
//Lượng sai số
const int err = 1;

//Thông số cho di chuyển đạn bắn
enum BulletDir
{
    D_RIGHT = 20,
    D_LEFT = 21
};

//Thông số cho loại đạn bắn
enum BulletTyp
{
    T_SPHERE = 50,
    T_LASER = 51,
    O_LASER = 52,
    O_SPHERE = 53
};

//Thông số cho Threat
const int THREAT_FRAME_NUM = 8;
const int THREAT_NUM = 20;
const int V_THREAT = 2;
const int ACTIVE_BULLET_DISTANCE = 300;


enum moveTyp
{
    STATIC_THREAT = 100,
    MOVING_THREAT = 101
};


namespace SDLCommonFunction
{
    //Hàm check va chạm: tham số chuyền vào là hai đối tượng va chạm
    bool CheckCollision (const SDL_Rect& Obj1, const SDL_Rect& Obj2);
}

//Chỉ số cho hiệu ứng vụ nổ
const int EXPLOSION_FRAME_NUM = 8;

//Thông số cho mạng nhân vật
const int LIVE_NUM = 8;

//Một số màu sắc cơ bản cho text
enum TextColor
{
    RED_TEXT = 0,
    WHITE_TEXT = 1,
    BLACK_TEXT = 2
};

const int MAX_TIME_PLAYING = 200;

//Kích cỡ font chữ
const int FONT_SIZE_1 = 18;
const int FONT_SIZE_2 = 72;
const int FONT_SIZE_3 = 30;

//Định nghĩa cho lớp Hình học
struct GeometryFormat
{
    GeometryFormat (int left, int top, int width, int height) {G_left = left; G_top = top; G_width = width; G_height = height;};

    int G_left;
    int G_top;
    int G_width;
    int G_height;
};

struct ColorGeometry
{
    ColorGeometry (Uint8 r, Uint8 g, Uint8 b) {R = r; G = g; B = b;};
    Uint8 R;
    Uint8 G;
    Uint8 B;
};


//Định nghĩa thông số cho Boss
#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10

#define BOSS_SPEED 2
#define BOSS_HEIGHT_VAL 18

#define FRAME_NUM_32 32
const int BOSS_HP = 5;

//Audio
static Mix_Chunk* g_SFX [10];

static Mix_Music* g_menu_music = NULL;
static Mix_Music* g_game_music = NULL;

//Menu

const int MENU_ITEM_NUMBER = 3;

#endif // COMMON_FUNCTION_H_

