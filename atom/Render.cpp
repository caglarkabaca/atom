#include "Render.hpp"

#include <iostream>

Render::Render(const char* title, int width, int height): 
	// renderer static olduðu için aþaðýdan sildim. Umarým sýkýntý olmaz.
	win(NULL), renderer(NULL), title(title), width(width), height(height) /* bu constructor çaðrýldýðýnda otmatik olarak class attributýný tanýmlýyor*/ { 
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "SDL INIT ERROR (Render.cpp): " << SDL_GetError() << std::endl;
	SDL_SetRelativeMouseMode(SDL_TRUE);

	win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	
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

// getter
SDL_Renderer* Render::getRenderer() { return renderer; }
SDL_Window* Render::getWindow() { return win; }

// rect çiziyor ama içi dolu
void Render::DrawRect(int x, int y, int w, int h, SDL_Color color) {
	
	SDL_Rect r{x, y, w, h};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &r);
}

// rect çiziyor ama içi boþ
void Render::DrawRectOutline(int x, int y, int w, int h, SDL_Color color) {

	SDL_Rect r{x, y, w, h};

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &r);
}

void Render::DrawLine(int x1, int y1, int x2, int y2, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
