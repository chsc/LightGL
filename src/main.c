/*
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "scene.h"

#define WIN_WIDTH     640
#define WIN_HEIGHT    480
#define WIN_BPP       32
#define FRAME_UPDATE  100

int main(int argc, char* argv[]) {
	unsigned int stime, etime, dtime;
	SDL_Surface* framebuffer;
	SDL_Event event;
	int loop = 1;

	if ((SDL_Init(SDL_INIT_VIDEO) == -1)) {
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if ((framebuffer = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP,
			SDL_SWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		printf("Could not initialize video mode: %s.\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_WM_SetCaption("LightGL Software Rasterizer", NULL);

	if (sceneInit(framebuffer->pixels, framebuffer->w, framebuffer->h,
			framebuffer->format->Rshift, framebuffer->format->Gshift,
			framebuffer->format->Bshift) == -1) {
		printf("Could not initialize scene.\n");
		SDL_Quit();
		return EXIT_FAILURE;
	}

	dtime = 0;
	stime = SDL_GetTicks();

	while (loop) {
		sceneUpdate(dtime / 1000.0f);

		if (SDL_LockSurface(framebuffer) == -1) {
			printf("Could not lock framebuffer: %s.\n", SDL_GetError());
			sceneClose();
			SDL_Quit();
			return EXIT_FAILURE;
		}
		sceneRender();
		SDL_UnlockSurface(framebuffer);

		if (SDL_Flip(framebuffer) == -1) {
			printf("Could not flip framebuffer: %s.\n", SDL_GetError());
			sceneClose();
			SDL_Quit();
			return EXIT_FAILURE;
		}

		etime = SDL_GetTicks();
		dtime = etime - stime;
		stime = etime;

		while (SDL_PollEvent(&event) == 1) {
			switch (event.type) {
			case SDL_QUIT:
				loop = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_1:
					/*current_shader = BaryShader;*/
					break;
				case SDLK_2:
					/*current_shader = PhongShader;*/
					break;
				default:
					break;
				}
				break;
			}
		}
	}

	sceneClose();
	SDL_Quit();
	return EXIT_SUCCESS;
}
