#ifndef GFX_H
#define GFX_H

#include <intdef.h>

struct Camera {
  f64 x, y;
};

struct ScreenCoords {
  u16 x, y;
};

struct Coords {
  f64 x, y;
};

void gfx_set_screen_pixel(u16 x, u16 y, u32 c);
void gfx_set_cam_pixel(i64 x, i64 y, u32 c);
void gfx_clear_screen();
struct ScreenCoords gfx_cam_to_screen(f64 x, f64 y);
struct Camera * gfx_get_camera();
void gfx_update();
int gfx_init(char * name, u16 w, u16 h);

#endif
