#pragma once
#include <map>
#include <string>
#include "SDL_image.h"

class TextureCache
{
public:
	TextureCache();
	~TextureCache();
	SDL_Texture* getTexture(std::string texturePath, SDL_Renderer* renderer);

private:
	std::map<std::string, SDL_Texture*> _textureMap;
};
