#pragma once
#include <SDL.h>

class SpriteSheet
{
private:
	SpriteSheet() {};
	~SpriteSheet() {};
public:
	static int w, h;
	static int tilesX, tilesY;

	static void QuerySpriteSheet(const int& tileSizeX, const int& tileSizeY, SDL_Texture* _texture);
	static SDL_Rect GetUVTile(int indexX, int indexY);
	static SDL_Rect GetSizedUVTile(int indexX, int indexY, int indexSizeX, int indexSizeY);
	static void draw(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& uvRect, SDL_Rect destRect, float scale = 1.0f, bool needsResizing = false);
	static void drawOutline(SDL_Renderer* renderer, const SDL_Rect& uvRect, SDL_Rect destRect, float scale = 1.0f, bool needsResizing = false);
	static void drawPlayer(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& uvRect, SDL_Rect destRect, float scale = 1.0f, bool needsResizing = false);
};

