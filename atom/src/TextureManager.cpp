#include "TextureManager.hpp"

TextureManager::TextureManager(Render& render): renderer(NULL) {
	// render dan rendereri �ekiyor, istersek direk renderi da tutabiliriz de
	// gerek yok
	renderer = render.getRenderer();
}

// Texture aktar�rken gereken ge�ici surface vs. gibi i�lemleri burada yap�yoruz
SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination) {
	SDL_RenderCopy(renderer, texture, &source, &destination);
}

// Pointerl� versiyonu
void TextureManager::DrawP(SDL_Texture* texture, SDL_Rect* source, SDL_Rect* destination) {
	SDL_RenderCopy(renderer, texture, source, destination);
}