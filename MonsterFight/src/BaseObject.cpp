//Định nghĩa các hàm trong BaseObject.h
#include "BaseObject.h"

BaseObject::BaseObject()
{
    //Các thiết lập ban đầu
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

BaseObject::~BaseObject()
{
    //Gọi hàm Free () tại hàm hủy để khi chương trình kết thúc thì destructor sẽ gọi Free()
    Free();
}

bool BaseObject::LoadImg (std::string path, SDL_Renderer* screen)
{
    //Xóa vùng nhớ chứa Img cũ
    Free();

    SDL_Texture* new_texture = NULL;
    //Đọc tấm ảnh (kết quả trả về của IMG_Load là 1 Surface nên phải dùng SDL_Surface để lưu trữ)
    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {
        //Nếu load thành công thì ta sẽ xóa phông ảnh
        SDL_SetColorKey (load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        //Chuyển surface thành texture
        new_texture = SDL_CreateTextureFromSurface (screen, load_surface);
        if (new_texture != NULL)
        {
            //Nếu thành công thì mình bắt đầu lưu thông số cho ảnh
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
        //Giải phóng load_surface
        SDL_FreeSurface (load_surface);
    }
    //Lưu ảnh
    p_object_ = new_texture;

    return p_object_!= NULL;
}

//Load ảnh lên nền window
void BaseObject:: Render (SDL_Renderer* des, const SDL_Rect* clip /* = NULL */)
{
    SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
    //Hàm rednercopy đẩy toàn bộ thông số của p_object_ lên des với kích thước và vị trí nằm trong renderquad
    SDL_RenderCopy (des, p_object_, clip, &renderquad);
}

void BaseObject::Free()
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture (p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}
