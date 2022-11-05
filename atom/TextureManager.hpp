#pragma once
#include "SDL.h"
#include "Render.hpp"
#include "SDL_image.h" //texture i�in gerekli
class TextureManager {
	
public:

	// assets dosyas�ndaki bir texture'�n konumunu parametre olarak al�yor. �rne�in: "assets/texture.png"
	static SDL_Texture* LoadTexture(const char* fileName); 

	// source ve destination rectangles i�in bknz: https://wiki.libsdl.org/SDL_RenderCopy
	static void Draw(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination); 

private:
	
};
