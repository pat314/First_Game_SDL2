
#ifndef TEXT_H_
#define TEXT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class Text
{
public:
    Text();
    ~Text();

    //bool Text_LoadFromFile (std::string path);

    bool Text_TextToTexture (TTF_Font* font, SDL_Renderer* screen);

    void Text_TexttoSurface (TTF_Font* font, SDL_Surface* screen, SDL_Rect rect, SDL_Window* window);

    void Text_Free();

    void Text_SetColorRGB (Uint8 R, Uint8 G, Uint8 B);
    void Text_SetColorType (int Text_num_type);
    SDL_Color Text_GetColor () const {return Text_Color;};

    //Hàm hiển thị text lên màn hình (nơi hiển thị text - màn hình, vị trí hiển thị, hiệu ứng ảnh động? = NULL, góc nghiêng )
    void Text_Render (SDL_Renderer* screen, int x, int y, SDL_Rect* clip = NULL, double leaning_angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip_type = SDL_FLIP_NONE);

    int Text_GetWidth() const {return Text_Width;};
    int Text_GetHeight() const {return Text_Height;};

    void Text_SetContent (const std::string& text) {Text_Content = text;};
    std::string Text_GetContent () const {return Text_Content;};
private:
    //Biến lưu nội dung của text
    std::string Text_Content;
    //Màu sắc của text
    SDL_Color Text_Color;
    //Biến quản lý text
    SDL_Texture* Text_Texture;
    //Chiều rộng, chiều cao của text
    int Text_Width;
    int Text_Height;

    //Biến lưu trữ ảnh Surface
    SDL_Surface* s_object_;

};

#endif // TEXT_H_
