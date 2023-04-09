
#include "Explosion.h"

Explosion::Explosion()
{
    explosion_frameIndex = 0;
    explosion_h_frame = 0;
    explosion_w_frame = 0;
}

Explosion::~Explosion()
{


}

bool Explosion::Explosion_LoadImg(std::string path, SDL_Renderer* screen)
{
    bool success = BaseObject::LoadImg (path, screen);
    if (success)
    {
        explosion_w_frame = rect_.w / EXPLOSION_FRAME_NUM;
        explosion_h_frame = rect_.h;
    }
    return success;
}

void Explosion::setExplosion_frame()
{
    if (explosion_w_frame > 0 && explosion_h_frame > 0)
    {
        for (int i = 0; i < EXPLOSION_FRAME_NUM; i++)
        {
            explosion_frame[i] = {explosion_w_frame*i, 0, explosion_w_frame, explosion_h_frame};
        }
    }
}

void Explosion::Explosion_ShowSprite (SDL_Renderer* screen)
{
    SDL_Rect* current_frame = & explosion_frame[explosion_frameIndex];
    SDL_Rect RenderQuad = {rect_.x, rect_.y, explosion_w_frame,explosion_h_frame};
    SDL_RenderCopy (screen, p_object_, current_frame, &RenderQuad);
}
