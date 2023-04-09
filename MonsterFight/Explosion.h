
#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class Explosion: public BaseObject
{
public:
    Explosion();
    ~Explosion();

    void setExplosion_frame();
    void setExplosion_frameIndex (const int & i) {explosion_frameIndex = i;};

    bool Explosion_LoadImg (std::string path, SDL_Renderer* screen);
    void Explosion_ShowSprite (SDL_Renderer* screen);
    int getExplosion_w_frame () const {return explosion_w_frame;};
    int getExplosion_h_frame () const {return explosion_h_frame;};


private:
    int explosion_w_frame;
    int explosion_h_frame;
    int explosion_frameIndex;
    SDL_Rect explosion_frame[EXPLOSION_FRAME_NUM];

};



#endif // EXPLOSION_H_



