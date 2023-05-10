
#ifndef MAIN_STATUS_H_
#define MAIN_STATUS_H_

#include "BaseObject.h"
#include "CommonFunc.h"

///Main Lives
class MainStatus : public BaseObject
{
public:
    MainStatus();
    ~MainStatus();

    //void Status_SetNum (const int& _num) {num = _num;};
    //Hàm dùng để lưu các vị trí tọa độ x để hiển thị các minh họa chỉ số sinh mạng lên màn hình vào mảng vector position_list
    void Status_AddPosition (const int& x);
    //Show các ảnh minh họa lên màn hình
    void Status_Show(SDL_Renderer* screen);
    //hàm load các tấm ảnh minh họa
    void Status_Init (SDL_Renderer* screen);

    void Status_IncreaseLive ();
    void Status_DecreaseLive ();

private:
    //Số ô để biểu thị số mạng chơi
    //int num;
    //vector lưu vị trí (tọa độ x) của ô hiển thị hình minh họa
    std::vector<int> position_list;

};

class Money : public BaseObject
{
public:
    Money();
    ~Money();

    void Money_Init(SDL_Renderer* screen);
    void Money_Show (SDL_Renderer* screen);
    void Money_SetPosition (const int& x, const int& y) {money_x_position = x; money_y_position = y;};
private:
    int money_x_position;
    int money_y_position;
};

class Watch : public BaseObject
{
public:
    Watch();
    ~Watch();

    void Watch_Init(SDL_Renderer* screen);
    void Watch_Show (SDL_Renderer* screen);
    void Watch_SetPosition (const int& x, const int& y) {Watch_x_position = x; Watch_y_position = y;};
private:
    int Watch_x_position;
    int Watch_y_position;
};

class Score : public BaseObject
{
public:
    Score();
    ~Score();

    void Score_Init(SDL_Renderer* screen);
    void Score_Show (SDL_Renderer* screen);
    void Score_SetPosition (const int& x, const int& y) {Score_x_position = x; Score_y_position = y;};
private:
    int Score_x_position;
    int Score_y_position;
};

///Boss HP
class BossStatus : public BaseObject
{
public:
    BossStatus();
    ~BossStatus();

    //void Status_SetNum (const int& _num) {num = _num;};
    //Hàm dùng để lưu các vị trí tọa độ x để hiển thị các minh họa chỉ số sinh mạng lên màn hình vào mảng vector position_list
    void BossStatus_AddPosition (const int& x);
    //Show các ảnh minh họa lên màn hình
    void BossStatus_Show(SDL_Renderer* screen);
    //hàm load các tấm ảnh minh họa
    void BossStatus_Init (SDL_Renderer* screen);

    void BossStatus_IncreaseHP ();
    void BossStatus_DecreaseHP ();

private:
    //Số ô để biểu thị số mạng chơi
    //int num;
    //vector lưu vị trí (tọa độ x) của ô hiển thị hình minh họa
    std::vector<int> BossStatus_position_list;

};

#endif // MAIN_STATUS_H_
