#ifndef TEXTURE_H
#define TEXTURE_H

#include <intdef.h>

int texture_load_atlas();
u16 texture_get_width();
u16 texture_get_height();
u32 * texture_get_tex(u16 tex);

#endif
