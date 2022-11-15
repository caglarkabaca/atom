#include "TextureManager.hpp"

TextureManager::TextureManager(Render& render): renderer(NULL) {
	// render dan rendereri çekiyor, istersek direk renderi da tutabiliriz de
	// gerek yok
	renderer = render.getRenderer();
}

// Texture aktarýrken gereken geçici surface vs. gibi iþlemleri burada yapýyoruz
SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination) {
	SDL_RenderCopy(renderer, texture, &source, &destination);
}

// Pointerlý versiyonu
void TextureManager::DrawP(SDL_Texture* texture, SDL_Rect* source, SDL_Rect* destination) {
	SDL_RenderCopy(renderer, texture, source, destination);
}