#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <intdef.h>
#include <gfx.h>
#include <primitive.h>
#include <texture.h>
#include <entity.h>

#define WINDOW_NAME "test window"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Camera * camera;

int main() {
  SDL_Event event;

  {
    int err = texture_load_atlas();
    if (err != 0) {
      fprintf(stderr, "texture_load_atlas() error: %d\n", err);
      return 1;
    }
  }
  
  gfx_init(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
  camera = gfx_get_camera();

  struct TexturedEntity ent = {
    { 0, 0, 64, 64 },
    { 0, 0, 8, 8 },
    0
  };
    
  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);
    /*camera->x -= 0.1f;
    camera->y -= 0.1f;*/
    
    entity_draw_textured(&ent);

    gfx_update();
    gfx_clear_screen();
  }
}