
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunc.h"

//Đối tượng ảnh (nói chung, về hình ảnh và các thông số kích thước)
class BaseObject
{
public:
    BaseObject();
    ~BaseObject();

    //Hàm set kích thước ảnh
    void SetRect (const int& x, const int& y) {rect_.x = x, rect_.y = y;}
    //Hàm lấy thông số tấm ảnh
    SDL_Rect GetRect () const {return rect_;}
    //Hàm lấy ra tấm ảnh (p_object_)
    SDL_Texture* GetObject() const {return p_object_;}

    //Hàm load ảnh bitmap lên trên đối tượng p_object_
    //(để sau đó p_objet_ load lên màn hình) (Khai bao virtual de cac lop ke thua BaseObject co the su dung lai duoc ham nay va them cac tinh nang moi)
    virtual bool LoadImg (std::string path, SDL_Renderer* screen);
    void Render (SDL_Renderer* des, const SDL_Rect* clip = NULL);

    //Hàm giải phóng đối tượng
    void Free();

protected:
    //Biến lưu trữ ảnh Texture
    SDL_Texture* p_object_;

    //Biến cài đặt kích thước cho ảnh
    SDL_Rect rect_;


};

#endif // BASE_OBJECT_H_
