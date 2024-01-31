#include "../utils/SpriteSheet.h"
class Tile;

int SpriteSheet::w = 0;
int SpriteSheet::h = 0;
int SpriteSheet::tilesX = 0;
int SpriteSheet::tilesY = 0;

void SpriteSheet::QuerySpriteSheet(const int& tileSizeX_, const int& tileSizeY_, SDL_Texture* _texture)
{
	SpriteSheet::tilesX = tileSizeX_;
	SpriteSheet::tilesY = tileSizeY_;
	SDL_QueryTexture(_texture, NULL, NULL, &w, &h);
}

SDL_Rect SpriteSheet::GetUVTile(int indexX, int indexY)
{
	return {
	(int)(ceil(((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil(((float)(abs(indexY - tilesY) - 1) / (float)tilesY) * (float)h)),
	(int)(floor((((float)(indexX + 1) / (float)tilesX) * (float)w) -
	((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(floor((((float)(abs(indexY - tilesY) + 1) / (float)tilesY) * (float)h) -
	((float)(abs(indexY - tilesY)) / (float)tilesY) * (float)h))
	};
}

SDL_Rect SpriteSheet::GetSizedUVTile(int indexX, int indexY, int indexSizeX, int indexSizeY)
{
	return {
	(int)(ceil(((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(ceil(((float)(abs(indexY - tilesY) - indexSizeY) / (float)tilesY) * (float)h)),
	(int)(floor((((float)(indexX + indexSizeX) / (float)tilesX) * (float)w) -
	((float)(indexX) / (float)tilesX) * (float)w)),
	(int)(floor((((float)(abs(indexY - tilesY) + indexSizeY) / (float)tilesY) * (float)h) -
	((float)(abs(indexY - tilesY)) / (float)tilesY) * (float)h))
	};
}

void SpriteSheet::draw(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& uvRect, SDL_Rect destRect, float scale /*= 1.0f*/, bool needsResizing /*= false*/)
{
	SDL_RenderCopy(renderer, texture, &uvRect, &destRect);

}

void SpriteSheet::drawOutline(SDL_Renderer* renderer, const SDL_Rect& uvRect, SDL_Rect destRect, float scale /*= 1.0f*/, bool needsResizing /*= false*/)
{
	SDL_RenderDrawRect(renderer, &destRect);
}

void SpriteSheet::drawPlayer(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& uvRect, SDL_Rect destRect, float scale /*= 1.0f*/, bool needsResizing /*= false*/) {
	if (needsResizing)
	{
		int originalWidth = destRect.w;
		int originalHeight = destRect.h;

		float aspectRatio = static_cast<float>(uvRect.w) / uvRect.h;

		destRect.w = static_cast<int>(originalHeight * aspectRatio * scale);
		destRect.h = static_cast<int>(originalHeight * scale);

		//Adjust y-position to make the texture scale upwards
		destRect.y -= (destRect.h - originalHeight);

		SDL_RenderCopy(renderer, texture, &uvRect, &destRect);
	}
	else {

		destRect.w *= scale;
		destRect.h *= scale;
		SDL_RenderCopy(renderer, texture, &uvRect, &destRect);
	}
}
