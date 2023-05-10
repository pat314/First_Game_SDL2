
#include "Text.h"

Text::Text()
{
    //Mặc định là màu trắng
    Text_Color.r = 255;
    Text_Color.g = 255;
    Text_Color.b = 255;

    Text_Texture = NULL;

}

Text::~Text()
{

}

bool Text::Text_TextToTexture (TTF_Font* font, SDL_Renderer* screen)
{
    //Đưa chữ về ảnh surface
    SDL_Surface* Text_Surface = TTF_RenderText_Solid (font, Text_Content.c_str(), Text_Color);
    //Chuyển surface về texture
    if (Text_Surface != NULL)
    {
        Text_Texture = SDL_CreateTextureFromSurface (screen, Text_Surface);
        Text_Width = Text_Surface->w;
        Text_Height = Text_Surface->h;

        SDL_FreeSurface (Text_Surface);
    }
    return Text_Texture != NULL;
}

void Text::Text_TexttoSurface (TTF_Font* font, SDL_Surface* screen, SDL_Rect rect, SDL_Window* window)
{
    s_object_ = TTF_RenderText_Solid (font, Text_Content.c_str(), Text_Color);
    Text_Width = s_object_->w;
    Text_Height = s_object_->h;
    SDL_BlitSurface (s_object_, NULL, screen, &rect);
    //SDL_UpdateWindowSurface (window);
}

void Text::Text_Free()
{
    if (Text_Texture != NULL)
    {
        SDL_DestroyTexture (Text_Texture);
        Text_Texture = NULL;
    }
}

void Text::Text_SetColorRGB (Uint8 R, Uint8 G, Uint8 B)
{
    Text_Color.r = R;
    Text_Color.g = G;
    Text_Color.b = B;
}

void Text::Text_SetColorType (int Text_num_type)
{
    if (Text_num_type == RED_TEXT)
    {
        //RGB của màu đỏ
        SDL_Color color = {255, 0, 0};
        Text_Color = color;
    }
    else if (Text_num_type == WHITE_TEXT)
    {
        //RGB của màu trắng
        SDL_Color color = {255, 255, 255};
        Text_Color = color;
    }
    else if (Text_num_type == BLACK_TEXT)
    {
        //RGB của màu đen
        SDL_Color color = {0, 0, 0};
        Text_Color = color;
    }
}

void Text::Text_Render (SDL_Renderer* screen, int x, int y,
                        SDL_Rect* clip /*= NULL*/, double leaning_angle /*= 0.0*/,
                        SDL_Point* center /*= NULL*/, SDL_RendererFlip flip_type /*= SDL_FLIP_NONE*/)
{
    SDL_Rect Text_renderQuad = {x, y, Text_Width, Text_Height};
    if (clip != NULL)
    {
        Text_renderQuad.w = clip->w;
        Text_renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(screen, Text_Texture, clip, &Text_renderQuad, leaning_angle, center, flip_type);
}
