
#include "Geometry.h"

void Geometry::RenderRectangle (const GeometryFormat& size, const ColorGeometry& color, SDL_Renderer* screen)
{
    //Lấy kích thước hcn cần vẽ
    SDL_Rect fill_rect = {size.G_left, size.G_top, size.G_width, size.G_height};
    //Set màu vẽ hcn
    SDL_SetRenderDrawColor (screen, color.R, color.G, color.B, 0xFF);
    //Vẽ hcn đặc
    SDL_RenderFillRect (screen, &fill_rect);
}

void Geometry::RenderOutline (const GeometryFormat& size, const ColorGeometry& color, SDL_Renderer* screen)
{
    //Lấy kích thước hcn cần vẽ
    SDL_Rect out_rect = {size.G_left, size.G_top, size.G_width, size.G_height};
    //Set màu vẽ hcn
    SDL_SetRenderDrawColor (screen, color.R, color.G, color.B, 0xFF);
    //Vẽ hcn (đường viền)
    SDL_RenderDrawRect (screen, &out_rect);

}
