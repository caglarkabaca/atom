#include <SDL.h>
#include <iostream>

#include "Test.h"

SDL_Rect rect;
bool isRunning = false;

int main(int, char* []) {
	SDL_Init(SDL_INIT_VIDEO);

	Test test;

	SDL_Window* window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);

	SDL_RenderClear(renderer);
	rect.x = 0;
	rect.y = 0;
	rect.h = 32;
	rect.w = 32;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);

	SDL_RenderDrawRect(renderer, &rect);

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	return 0;
}