#include <texture.h>
#include <intdef.h>
#include <stdio.h>
#include <stdlib.h>

#define TEX_FILE "./textures.kmg"
#define TEX_WIDTH 8
#define TEX_HEIGHT 8

static u32 * texture_atlas = NULL;

int texture_load_atlas() {
  FILE * fp = fopen(TEX_FILE, "r");
  if (fp == NULL) {
    return 1;
  }

  fseek(fp, 0L, SEEK_END);
  u64 size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  texture_atlas = malloc(size);
  if (texture_atlas == NULL) {
    fclose(fp);
    return 2;
  }

  if (fread(texture_atlas, size, 1, fp) != 1) {
    fclose(fp);
    free(texture_atlas);
    return 3;
  }

  fclose(fp);
}

u16 texture_get_width() {
  return TEX_WIDTH;
}

u16 texture_get_height() {
  return TEX_HEIGHT;
}

u32 * texture_get_tex(u16 tex) {
  if (texture_atlas == NULL) {
    return NULL;
  }
  
  return &texture_atlas[tex * (TEX_WIDTH * TEX_HEIGHT)];
}
