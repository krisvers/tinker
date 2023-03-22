#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);

	printf("Test\n");
	
	SDL_Quit();

	return 0;
}
