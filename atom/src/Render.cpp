#include "Render.hpp"

#include <iostream>

Render::Render(const char* title, int width, int height): 

	win(NULL), renderer(NULL), title(title), width(width), height(height) /* bu constructor �a�r�ld���nda otmatik olarak class attribut�n� tan�ml�yor*/ { 
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "SDL INIT ERROR (Render.cpp): " << SDL_GetError() << std::endl;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	
	// SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN); // geli�tirme s�recinde kapal� kals�n
	// isFullscreen = true;

	if (win == NULL)
		std::cout << "CreateWindow Error (Render.cpp): " << SDL_GetError() << std::endl;

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
		std::cout << "CreateRenderer Error (Render.cpp): " << SDL_GetError() << std::endl;

}

Render::~Render() {
	//std::cout << "deconstructer called" << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Render::Clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // siyaha boyuyor
	SDL_RenderClear(renderer);
}

void Render::Update() {
	SDL_RenderPresent(renderer);
}

void Render::ToggleFullscreen() {
	SDL_SetWindowFullscreen(win, !isFullscreen);
	isFullscreen = !isFullscreen;
}

// getter
SDL_Renderer* Render::getRenderer() { return renderer; }
SDL_Window* Render::getWindow() { return win; }

// rect �iziyor ama i�i dolu
void Render::DrawRect(int x, int y, int w, int h, SDL_Color color) {
	
	SDL_Rect r{x, y, w, h};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &r);
}

// rect �iziyor ama i�i bo�
void Render::DrawRectOutline(int x, int y, int w, int h, SDL_Color color) {

	SDL_Rect r{x, y, w, h};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &r);
}

void Render::DrawLine(int x1, int y1, int x2, int y2, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}