#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <intdef.h>
#include <gfx.h>

static u32 * screen = NULL;
static u32 * double_buffer = NULL;
static u16 width = 0;
static u16 height = 0;
static SDL_Window * window;
static SDL_Surface * surface;
static struct Camera camera = {
  0, 0
};

void gfx_set_screen_pixel(u16 x, u16 y, u32 c) {
  if (screen == NULL) {
    return;
  }

  if (x >= width || y >= height) {
    return;
  }

  double_buffer[x + (y * width)] = c;
}

void gfx_clear_screen() {
  memset(double_buffer, 0, width * height * 4);
}

struct ScreenCoords gfx_cam_to_screen(f64 x, f64 y) {
  return (struct ScreenCoords) {
    (u16) x - camera.x, (u16) y - camera.y
  };
}

void gfx_set_cam_pixel(i64 x, i64 y, u32 c) {
  struct ScreenCoords scr_crds = gfx_cam_to_screen(x, y);

  if (scr_crds.x >= width || scr_crds.y >= height) {
    return;
  }

  gfx_set_screen_pixel(scr_crds.x, scr_crds.y, c);
}

void gfx_update() {
  memcpy(screen, double_buffer, width * height * 4);
  SDL_UpdateWindowSurface(window);
}

struct Camera * gfx_get_camera() {
  if (&camera != NULL) {
    return &camera;
  }

  return NULL;
}

int gfx_init(char * name, u16 w, u16 h) {
    if (SDL_Init(SDL_INIT_EVERYTHING != 0)) {
    fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow() returned NULL: %s\n", SDL_GetError());
    return 2;
  }

  surface = SDL_GetWindowSurface(window);
  if (surface == NULL) {
    fprintf(stderr, "SDL_GetWindowSurface() returned NULL: %s\n", SDL_GetError());
    return 3;
  }

  double_buffer = malloc(surface->w * surface->h * 4);
  if (double_buffer == NULL) {
    fprintf(stderr, "malloc() of size %d bytes failed\n", surface->w * surface->h * 4);
    return 4;
  }

  screen = (u32 *) surface->pixels;
  width = surface->w;
  height = surface->h;
}