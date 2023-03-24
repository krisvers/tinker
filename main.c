#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "game"
#define PIXEL_SCALE 5
#define TRANSPARENT_COLOR 0xFF00FF
#define BACKGROUND_COLOR 0xE0E4F4
#define TEXTURE_SIZE 8
#define TEXTURE_NUM 3

#define RGB(r, g, b) b | g << 8 | r << 16

// time is your enemy: fighting the clock literally (he steals your gf :'c )

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
	BLACK = 0X1F1020,
	TRANS = TRANSPARENT_COLOR,
};

enum color atlas[TEXTURE_NUM][TEXTURE_SIZE][TEXTURE_SIZE] = {
	// grass
	{
	{	LIME,	LIME,	LIME,	LIME,	LIME,	LIME,	LIME,	LIME,	},
	{	GREEN,	LIME,	GREEN,	GREEN,	LIME,	LIME,	GREEN,	GREEN,	},
	{	BROWN,	GREEN,	BROWN,	BROWN,	GREEN,	GREEN,	BROWN,	BROWN,	},
	{	BROWN,	TAN,	TAN,	BROWN,	BROWN,	BROWN,	TAN,	BROWN,	},
	{	BROWN,	BROWN,	GREY,	BROWN,	BROWN,	BROWN,	TAN,	BROWN,	},
	{	TAN,	BROWN,	BROWN,	BROWN,	TAN,	BROWN,	GREY,	TAN,	},
	{	BROWN,	BROWN,	BROWN,	TAN,	GREY,	BROWN,	BROWN,	BROWN,	},
	{	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	BROWN,	},
	},
	// glass
	{
	{	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	},
	{	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,	},
	{	WHITE,	TRANS,	TRANS,	WHITE,	TRANS,	TRANS,	TRANS,	WHITE,	},
	{	WHITE,	TRANS,	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,	},
	{	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,	},
	{	WHITE,	TRANS,	TRANS,	TRANS,	TRANS,	WHITE,	TRANS,	WHITE,	},
	{	WHITE,	TRANS,	TRANS,	TRANS,	WHITE,	TRANS,	TRANS,	WHITE,	},
	{	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	WHITE,	},
	},
	// player
	{
	{	TRANS,	TRANS,	BLACK,	BLACK,	BLACK,	BLACK,	TRANS,	TRANS,	},
	{	TRANS,	BLACK,	YELLOW,	YELLOW,	YELLOW,	YELLOW,	BLACK,	TRANS,	},
	{	TRANS,	BLACK,	DBROWN,	YELLOW,	YELLOW,	DBROWN,	BLACK,	TRANS,	},
	{	TRANS,	BLACK,	YELLOW,	YELLOW,	YELLOW,	YELLOW,	BLACK,	TRANS,	},
	{	TRANS,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	TRANS,	},
	{	TRANS,	TRANS,	BLACK,	TRANS,	TRANS,	BLACK,	TRANS,	TRANS,	},
	{	TRANS,	TRANS,	BLACK,	TRANS,	TRANS,	BLACK,	TRANS,	TRANS,	},
	{	TRANS,	BLACK,	BLACK,	TRANS,	TRANS,	BLACK,	BLACK,	TRANS,	},
	},
};

unsigned char keys[41];

struct Transform {
	double x, y;
	double w, h;
};

struct Camera {
	double x, y;
} camera;

struct ColorBox {
	struct Transform transform;
	unsigned int color;
};

struct TextureBox {
	struct Transform transform;
	unsigned int tex;
};

struct Entity {
	struct Transform transform;
	unsigned int tex;
};

void cam_pixel(long int x, long int y, unsigned int color) {
	if (surface == NULL) {
		printf("cam_pixel() error: no surface to draw to!\n");
		return;
	}
	
	if (color == TRANSPARENT_COLOR) {
		return;
	}

	for (unsigned int a = 0; a < PIXEL_SCALE; a++) {
		for (unsigned int b = 0; b < PIXEL_SCALE; b++) {
			unsigned int c = (x * PIXEL_SCALE) - ((long int) camera.x + (WINDOW_WIDTH + TEXTURE_SIZE * PIXEL_SCALE) / 2) - a + ((y * PIXEL_SCALE) + b + ((long int) camera.y + (WINDOW_HEIGHT - TEXTURE_SIZE * PIXEL_SCALE) / 2)) * WINDOW_WIDTH + PIXEL_SCALE - 1;
			if (c >= (WINDOW_WIDTH - 1) * (WINDOW_HEIGHT - 1)) {
				printf("a\n");
				break;
			}

			if (PIXEL_SCALE * x + a - (camera.x + (WINDOW_WIDTH + TEXTURE_SIZE * PIXEL_SCALE) / 2) >= WINDOW_WIDTH) {
				printf("b cam: %lf, %lf, in: %li, %li, abc: %u, %u, %u\n", camera.x, camera.y, x, y, a, b, c);
				break;
			}

			if (PIXEL_SCALE * y + b - (camera.y + (WINDOW_HEIGHT - TEXTURE_SIZE * PIXEL_SCALE) / 2) >= WINDOW_HEIGHT) {
				printf("c cam: %lf, %lf, in: %li, %li, abc: %u, %u, %u\n", camera.x, camera.y, x, y, a, b, c);
				break;
			}

			printf("d cam: %lf, %lf, in: %li, %li, abc: %u, %u, %u\n", camera.x, camera.y, x, y, a, b, c);
			((unsigned int *) surface->pixels)[c] = color;
		}
	}
}

void pixel(long int x, long int y, unsigned int color) {
	if (surface == NULL) {
		printf("pixel() error: no surface to draw to!\n");
		return;
	}
	
	if (x * PIXEL_SCALE > (WINDOW_WIDTH)) {
		printf("pixel() warning: x out of bounds for drawing!\n");
		return;
	}
	
	if (y * PIXEL_SCALE > (WINDOW_HEIGHT)) {
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

void clear() {
	for (unsigned int a = 0; a < WINDOW_WIDTH / PIXEL_SCALE + 1; a++) {
		for (unsigned int b = 0; b < WINDOW_HEIGHT / PIXEL_SCALE + 1; b++) {
			pixel(a, b, BACKGROUND_COLOR);
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
			cam_pixel(x + a, y + b, atlas[index][b][a]);
		}
	}
}

// doesn't work yet
void draw_tiled_texture(unsigned int x, unsigned int y, double w, double h, unsigned int index) {
	if (index >= TEXTURE_NUM) {
		printf("draw_tiled_texture() error: index is larger than the number of textures!\n");
		return;
	}
		
	for (unsigned int a = 0; a < (unsigned int) round(w * (TEXTURE_SIZE)); a++) {
		for (unsigned int b = 0; b < (unsigned int) round(h * (TEXTURE_SIZE)); b++) {
			unsigned int c = ((int) a / w);
			unsigned int d = ((int) b / h);
			
			if (c >= TEXTURE_SIZE) {
				printf("draw_tiled_texture() warning: c is out of bounds for the atlas!\n");
				break;
			}
		
			if (d >= TEXTURE_SIZE) {
				printf("draw_tiled_texture() warning: d is out of bounds for the atlas!\n");
				break;
			}
			
			cam_pixel(x + a, y + b, atlas[index][d][c]);
		}
	}
}

void draw_scaled_texture(unsigned int x, unsigned int y, double w, double h, unsigned int index) {
	if (index >= TEXTURE_NUM) {
		printf("draw_scaled_texture() error: index is larger than the number of textures!\n");
		return;
	}
	
	for (unsigned int a = 0; a < (unsigned int) round(w * (TEXTURE_SIZE)); a++) {
		for (unsigned int b = 0; b < (unsigned int) round(h * (TEXTURE_SIZE)); b++) {
			unsigned int c = ((int) (a / w));
			unsigned int d = ((int) (b / h));
			
			if (c >= TEXTURE_SIZE) {
				printf("draw_scaled_texture() warning: c is out of bounds for the atlas!\n");
				break;
			}
			
			if (d >= TEXTURE_SIZE) {
				printf("draw_scaled_texture() warning: d is out of bounds for the atlas!\n");
				break;
			}
			
			cam_pixel(x + a, y + b, atlas[index][d][c]);
		}
	}
}

void draw_entity(struct Entity * ent) {
	draw_scaled_texture(ent->transform.x, ent->transform.y, ent->transform.w, ent->transform.h, ent->tex);
}

unsigned char key_pressed(unsigned short key) {
	return keys[key / 8] & (1 << (key % 8)) ? 1 : 0;
}

int main(int argc, char ** argv) {
	SDL_Event event;

	if (argc == 1 || argv[1]) {}	// to not have unused parameter warning

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

	struct Entity test = {
		{ 0, 0, 1.0, 1.0 }, 2
	};

	//double i = 0.5;

	while (event.type != SDL_QUIT) {
		SDL_PollEvent(&event);

		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym > 322) {
					break;
				}

				if (event.key.keysym.sym == SDLK_ESCAPE) {
					goto loop_end;
				}

				keys[event.key.keysym.sym / 8] |= 1 << (event.key.keysym.sym % 8);
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym > 322) {
					break;
				}

				keys[event.key.keysym.sym / 8] = ~(~event.key.keysym.sym | 1 << (event.key.keysym.sym % 8));
				break;
			case SDL_QUIT:
				goto loop_end;
			default:
				break;
		}

		clear();
		pixel(0, 0, RED);
		//draw_scaled_texture(32, 32, i, 1.0, 1);
		draw_entity(&test);
		//draw_texture(16, 16, 0);
		//i += 0.01;
		test.transform.x += key_pressed(SDLK_d) / 10.0;
		test.transform.y += key_pressed(SDLK_s) / 10.0;
		test.transform.x -= key_pressed(SDLK_a) / 10.0;
		test.transform.y -= key_pressed(SDLK_w) / 10.0;
		camera.x += key_pressed(SDLK_d) * PIXEL_SCALE / 10.0;
		camera.x -= key_pressed(SDLK_a) * PIXEL_SCALE / 10.0;
		camera.y += key_pressed(SDLK_w) * PIXEL_SCALE / 10.0;
		camera.y -= key_pressed(SDLK_s) * PIXEL_SCALE / 10.0;
		SDL_UpdateWindowSurface(window);
	}

loop_end:
	
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
