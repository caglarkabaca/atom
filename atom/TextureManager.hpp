#pragma once
#include "SDL.h"
#include "Render.hpp"
#include "SDL_image.h" //texture için gerekli

class TextureManager {
	
public:
	TextureManager(Render& render);

	// assets dosyasýndaki bir texture'ýn konumunu parametre olarak alýyor. Örneðin: "assets/texture.png"
	SDL_Texture* LoadTexture(const char* fileName); 

	// source ve destination rectangles için bknz: https://wiki.libsdl.org/SDL_RenderCopy
	void Draw(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination); 

private:
	SDL_Renderer* renderer;
};
