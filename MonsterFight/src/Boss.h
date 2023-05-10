

#ifndef BOSS_H_
#define BOSS_H_


#include "BaseObject.h"
#include "CommonFunc.h"
#include "Bullet.h"



class BossObject : public BaseObject
{
public:
    BossObject();
    ~BossObject();
    void set_x_val(int xp) {x_val_ = xp;}
    void set_y_val(int yp) {y_val_ = yp;}

    void set_xpos(const int& xps) {x_pos_ = xps;}
    void set_ypos(const int& yps) {y_pos_ = yps;}
    int get_x_pos() {return x_pos_;}
    int get_y_pos() {return y_pos_;}

    void Show(SDL_Renderer* des);
    bool Boss_LoadImg(std::string path, SDL_Renderer* screen);
    void set_clips();

    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}

    void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x, map_y_ = map_y;};

    void CheckToMap(Map& g_map);
    void CenterEntityOnMap(Map& g_map);
    void DoBoss(Map& g_map);
    void InitBoss();
    std::vector<Bullet*> get_bullet_list() const {return bullet_list_;}
    void set_bullet_list(const std::vector<Bullet*>& am_list) {bullet_list_ = am_list;}

    void InitBullet(SDL_Renderer* screen);
    void MakeBullet(SDL_Renderer* des, const int& x_limit, const int& y_limit, Map& mapData, Mix_Chunk* sound);
    bool is_Boss_Free () const {return is_free;};
    void Boss_Free () {is_free = true;};

    void ClearBullet ();

private:
    int map_x_;
    int map_y_;
    int on_ground_;
    int think_time_;
    Control input_type_;
    int frame_;
    SDL_Rect frame_clip_[FRAME_NUM_32];
    int x_pos_;
    int y_pos_;
    float x_val_;
    float y_val_;
    int width_frame_;
    int height_frame_;
    bool is_free;
    std::vector<Bullet*> bullet_list_;
};


#endif
