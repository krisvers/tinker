#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "game"
#define PIXEL_SCALE 5
#define TRANSPARENT_COLOR 0xFF00FF
#define TEXTURE_SIZE 8
#define TEXTURE_NUM 2

#define RGB(r, g, b) b << 24 | g << 16 | r << 8

SDL_Window * window;
SDL_Surface * surface;

enum color {
	RED = 0x9D303B,
	ORANGE = 0xE4943A,
	YELLOW = 0xF5EDBA,
	LIME = 0xC0C741,
	GREEN = 0x647D34,
	CYAN = 0x7EC4C1,
	AQUA = 0x34859D,
	LBLUE = 0x8C8FAE,
	BLUE = 0x17434B,
	PURPLE = 0x70377F,
	PINK = 0xD26471,
	TAN = 0xD79B7D,
	BROWN = 0x9A6348,
	DBROWN = 0x3E2137,
	GREY = 0x584563,
	WHITE = 0xEBEBEB,
	TRANS = TRANSPARENT_COLOR,
};

enum color atlas[TEXTURE_NUM][TEXTURE_SIZE][TEXTURE_SIZE] = {
	// grass
	LIME,		LIME,		LIME,		LIME,		LIME,		LIME,		LIME,		LIME,
	GREEN,	LIME,		GREEN,	GREEN,	LIME,		LIME,		GREEN,	GREEN,
	BROWN,	GREEN,	BROWN,	BROWN,	GREEN,	GREEN,	BROWN,	BROWN,
	BROWN,	TAN,		TAN,		BROWN,	BROWN,	BROWN,	TAN,		BROWN,
	BROWN,	BROWN,	GREY,		BROWN,	BROWN,	BROWN,	TAN,		BROWN,
	TAN,		BROWN,	BROWN,	BROWN,	TAN,		BROWN,	GREY,		TAN,
	BROWN,	BROWN,	BROWN,	TAN,		GREY,		BROWN,	BROWN,	BROWN,
	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,
	// glass
	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,
	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,
	WHITE,	TRANS,	TRANS,	WHITE,	TRANS,	TRANS,	TRANS,	WHITE,
	WHITE,	TRANS,	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,
	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,
	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,	TRANS,	WHITE,
	WHITE,	TRANS,	TRANS,	TRANS,	WHITE,	TRANS,	TRANS,	WHITE,
	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,
};

struct Transform {
	int x, y;
	double w, h;
};

struct ColorBox {
	struct Transform transform;
	unsigned int color;
};

struct TextureBox {
	struct Transform transform;
	unsigned int tex;
};

void pixel(unsigned int x, unsigned int y, unsigned int color) {
	if (surface == NULL) {
		printf("pixel() error: no surface to draw to!\n");
		return;
	}
	
	if (x * PIXEL_SCALE > (WINDOW_WIDTH - 1)) {
		printf("pixel() warning: x out of bounds for drawing!\n");
		return;
	}
	
	if (y * PIXEL_SCALE > (WINDOW_HEIGHT - 1)) {
		printf("pixel() warning: y out of bounds for drawing!\n");
		return;
	}
	
	if (color == TRANSPARENT_COLOR) {
		return;
	}

	for (unsigned int a = 0; a < PIXEL_SCALE; a++) {
		for (unsigned int b = 0; b < PIXEL_SCALE; b++) {
			if ((x * PIXEL_SCALE) + a + ((y * PIXEL_SCALE) + b) * WINDOW_WIDTH >= (WINDOW_WIDTH - 1) * (WINDOW_HEIGHT - 1)) {
				break;
			}

			((unsigned int *) surface->pixels)[(x * PIXEL_SCALE) + a + ((y * PIXEL_SCALE) + b) * WINDOW_WIDTH] = color;
		}
	}
}

void draw_texture(unsigned int x, unsigned int y, unsigned int index) {
	if (index >= TEXTURE_NUM) {
		printf("draw_texture() error: index is larger than number of textures!\n");
		return;
	}
	
	for (unsigned int a = 0; a < TEXTURE_SIZE; a++) {
		for (unsigned int b = 0; b < TEXTURE_SIZE; b++) {
			pixel(x + a, y + b, atlas[index][b][a]);
		}
	}
}

void draw_scaled_texture(unsigned int x, unsigned int y, double w, double h, unsigned int index) {
	if (index >= TEXTURE_NUM) {
		printf("draw_scaled_texture() error: index is larger than the number of textures!\n");
		return;
	}
	
	for (unsigned int a = 0; a < (unsigned int) w * (TEXTURE_SIZE + 1); a++) {
		for (unsigned int b = 0; b < (unsigned int) h * (TEXTURE_SIZE + 1); b++) {
			pixel(x + a, y + b, atlas[index][(int) (b / h)][(int) (a / w)]);
		}
	}
}

int main() {
	SDL_Event event;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (window == NULL) {
		printf("window failed to create: %s\n", SDL_GetError());
		return -2;
	}

	surface = SDL_GetWindowSurface(window);
	if (surface == NULL) {
		printf("surface failed to obtain window surface: %s\n", SDL_GetError());
		return -3;
	}

	while (event.type != SDL_QUIT) {
		SDL_PollEvent(&event);
		pixel(0, 0, RGB(0xFF, 0xFF, 0x00));
		draw_scaled_texture(32, 32, 3.3, 6.9, 1);
		draw_texture(16, 16, 0);
		SDL_UpdateWindowSurface(window);
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
