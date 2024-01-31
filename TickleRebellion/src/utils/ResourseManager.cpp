#include "../utils/ResourseManager.h"

TextureCache ResourceManager::_textureCache	;

SDL_Texture* ResourceManager::getTexture(std::string texturePath, SDL_Renderer* renderer)
{
	return _textureCache.getTexture(texturePath, renderer);
}
