#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <random>
#include "../utils/SpriteSheet.h"

class Scene;
class Turret;
class Graph;
class Node;
class Path;

// Tile struct declaration 
struct Tile {
    Tile* child;
    SDL_Texture* tileTexture;
    SDL_Rect uvCoords;
    SDL_Rect destCoords;
    float scale = 1.0f;
    bool needsResizing = false;
    bool isWalkable = true;
    Node* tileNode = nullptr;
    char letter = ' ';

    void resizeTile();
};

class Level {
private:
    Scene* scene;
    SDL_Texture* mapTexture;
    SDL_Texture* background;
    Tile* topTile;
    std::vector<Tile*> m_tiles;
    std::vector<Node*> walkableTileNodes;
    std::vector<std::string> m_levelData;
    int mousePosX;
    int mousePosY;

    static std::mt19937 randomEngine;
    static std::uniform_real_distribution<float> scaleGenerator;

public:
    Level() = default;
    Level(const std::string& fileName, Scene* scene);
    ~Level();

    void loadMap(const int& tileSizeX, const int& tileSizeY, const char* filename);
    void clear();
    void sortTiles();
    void drawTiles();
    bool isMouseOverTile(const Tile* tile);
    Node* getTileNodeUnderMouse();
    void levelHandleEvents(const SDL_Event& event);
    bool canPlaceEntity();
    void drawTopTileOutline();


    int getWidth() const { return m_levelData[0].size(); }
    int getHeight() const { return m_levelData.size(); }

    std::vector<Node*> getWalkableTileNodes() const { return walkableTileNodes; }
    std::vector<Tile*> getTiles() const { return m_tiles; }
    Tile* getTile(int x, int y)
    {
        if (x < 0) x = 0;
        if (x >= m_levelData[0].size()) x = m_levelData[0].size() - 1;

        if (y < 0) y = 0;
        if (y >= m_levelData.size()) y = m_levelData.size() - 1;
        // get the position of a 1d array as if it was a 2d array(better cache-coherency)
        //each row represents a data of columns scattered around no the heap which does 
        //not make the 2D array contiguous
        return m_tiles[y * m_levelData[0].size() + x];
    }

};
