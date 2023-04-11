#include <primitive.h>
#include <intdef.h>
#include <gfx.h>

void primitive_draw_prim(struct Primitive * prim) {
  for (u16 w = 0; w < prim->w; w++) {
    for (u16 h = 0; h < prim->h; h++) {
      gfx_set_cam_pixel(prim->x + w, prim->y + h, (w * h) * 69420 + 0xFF0000);
    }
  }
}

void primitive_draw_colored(struct ColoredPrimitive * prim) {
  for (u16 w = 0; w < prim->transform.w; w++) {
    for (u16 h = 0; h < prim->transform.h; h++) {
      gfx_set_cam_pixel(prim->transform.x + w, prim->transform.y + h, prim->color);
    }
  }
}
