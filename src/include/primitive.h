#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <intdef.h>

struct Primitive {
  f64 x, y;
  u16 w, h;
};

struct ColoredPrimitive {
  struct Primitive transform;
  u32 color;
};

struct TexturedPrimitive {
  struct Primitive transform;
  u16 tex;
};

void primitive_draw_prim(struct Primitive * prim);
void primitive_draw_colored(struct ColoredPrimitive * prim);
void primitive_draw_textured(struct TexturedPrimitive * prim);

#endif
