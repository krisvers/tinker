#ifndef ENTITY_H
#define ENTITY_H

#include <intdef.h>
#include <primitive.h>

struct Entity {
  struct Primitive hitbox;
  struct Primitive transform;
};

struct ColoredEntity {
  struct Primitive hitbox;
  struct Primitive transform;
  u32 color;
};

struct TexturedEntity {
  struct Primitive hitbox;
  struct Primitive transform;
  u16 tex;
};

void entity_draw_ent(struct Entity * ent);
void entity_draw_colored(struct ColoredEntity * ent);
void entity_draw_textured(struct TexturedEntity * ent);

#endif
