#include "TextureManager.hpp"

// Texture aktarýrken gereken geçici surface vs. gibi iþlemleri burada yapýyoruz

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Render::renderer,tempSurface);
	SDL_FreeSurface(tempSurface);

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect source, SDL_Rect destination) {
	SDL_RenderCopy(Render::renderer, texture, &source, &destination);
}