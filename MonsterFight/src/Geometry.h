
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "CommonFunc.h"

class Geometry
{
public:
    //Hàm dùng cho toàn bộ chương trình dùng để vẽ HCN
    static void RenderRectangle(const GeometryFormat& size, const ColorGeometry& color, SDL_Renderer* screen);
    //Hàm vẽ đường viền
    static void RenderOutline (const GeometryFormat& size, const ColorGeometry& color, SDL_Renderer* screen);
};


#endif // GEOMETRY_H_
