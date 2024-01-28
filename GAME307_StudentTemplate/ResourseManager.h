#pragma once
#include "TextureCache.h"

class ResourceManager
{
public:
	static SDL_Texture* getTexture(std::string texturePath, SDL_Renderer* renderer);


private:
	static TextureCache _textureCache;
};
