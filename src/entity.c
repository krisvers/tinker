#include <entity.h>
#include <gfx.h>
#include <primitive.h>
#include <texture.h>
#include <stddef.h>

void entity_draw_ent(struct Entity * ent) {
  for (u16 w = 0; w < ent->transform.w; w++) {
    for (u16 h = 0; h < ent->transform.h; h++) {
      gfx_set_cam_pixel(ent->transform.x + w, ent->transform.y + h, 0x0000FF + (w * h) * 0x000100);
    }
  }
}

void entity_draw_colored(struct ColoredEntity * ent) {
  for (u16 w = 0; w < ent->transform.w; w++) {
    for (u16 h = 0; h < ent->transform.h; h++) {
      gfx_set_cam_pixel(ent->transform.x + w, ent->transform.y + h, ent->color);
    }
  }
}

void entity_draw_textured(struct TexturedEntity * ent) {
  u32 * texture = texture_get_tex(ent->tex);
  u16 tex_width = texture_get_width();
  u16 tex_height = texture_get_height();
  
  for (f32 w = 0; w < ent->transform.w; w += tex_width / (f32) ent->transform.w) {
    for (f32 h = 0; h < ent->transform.h; h += tex_height / (f32) ent->transform.h) {
      gfx_set_cam_pixel(ent->transform.x + w, ent->transform.y + h, (texture == NULL) ? 0xFF00FF : texture[(int) (w + (h * tex_width))]);
    }
  }
}
