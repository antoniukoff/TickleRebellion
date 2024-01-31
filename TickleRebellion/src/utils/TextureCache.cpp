#include "TextureCache.h"
#include <iostream>


TextureCache::TextureCache() {}

TextureCache::~TextureCache() {}

SDL_Texture* TextureCache::getTexture(std::string texturePath, SDL_Renderer* renderer)
{

	auto mit = _textureMap.find(texturePath);

	if (mit == _textureMap.end())
	{
		SDL_Surface* surface = IMG_Load(texturePath.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr) {
			std::cout << "Error: " << SDL_GetError() << std::endl;
		}
		_textureMap.insert(make_pair(texturePath, texture));
		SDL_FreeSurface(surface);
		return texture;
	}
	
	return mit->second;
}