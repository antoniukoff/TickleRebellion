#include "Level.h"
#include "fstream"
#include <iostream>
#include "SDL_image.h"
#include "../core/Scene.h"
#include "../core/Node.h"


std::mt19937 Level::randomEngine = std::mt19937(std::random_device()());
std::uniform_real_distribution<float> Level::scaleGenerator = std::uniform_real_distribution<float>(2, 2.7);


Level::Level(const std::string& fileName, Scene* scene)
    :scene(scene),
    mousePosX(0),
    mousePosY(0),
    topTile(nullptr),
    mapTexture(nullptr)
{
    std::ifstream file;
    file.open(fileName);

    // Error checking
    if (file.fail()) {
        std::cerr << "Failed to open " + fileName << std::endl;
    }

    std::string temp;
    std::getline(file, temp);
    while (std::getline(file, temp)) {
        m_levelData.push_back(temp);
    }
    file.close();

    SDL_Surface* surface = IMG_Load("assets/sprites/fondo.png");
    if (surface == nullptr) {
		throw std::runtime_error("Incorrect filepath");
	}
    background = SDL_CreateTextureFromSurface(scene->game->getRenderer(), surface);
    if (background == nullptr) {
		throw std::runtime_error("Background Texture was not created!");
	}

	SDL_FreeSurface(surface);
}

Level::~Level() {}

void Level::loadMap(const int& tileSizeX, const int& tileSizeY, const char* filename)
{
    SDL_Surface* surface = IMG_Load(filename);
    if (surface == nullptr) {
        throw std::runtime_error("Incorrect filepath");
    }

    mapTexture = SDL_CreateTextureFromSurface(scene->game->getRenderer(), surface);
    if (mapTexture == nullptr) {
        throw std::runtime_error("Map Texture was not created!");
    }

    SDL_FreeSurface(surface);
    /// Quering sprite sheet to get dimentions and pass how many tiles along x and y are there
    /// needed for sprite calculation later
    SpriteSheet::QuerySpriteSheet(tileSizeX, tileSizeY, mapTexture);

    /// <summary>
    /// GetUVTile function gets the individual texture from the spritesheet. Just pass the coordinates of the tile and
    /// it will return the sdl rectangle that is used to extract a tile from the spritesheet 
    /// </summary>
    /// <param name="indexX"> depending on the spritesheet pass in the coordinate of the tile along X</param>
    /// <param name="indexY"> depending on the spritesheet pass in the coordinate of the tile along Y</param>
    SDL_Rect pathRect = SpriteSheet::GetUVTile(1, 8);
    SDL_Rect grassRect = SpriteSheet::GetUVTile(1, 10);
    SDL_Rect leftGrassRect = SpriteSheet::GetUVTile(0, 8);
    SDL_Rect bottomLeftGrassCorner = SpriteSheet::GetUVTile(0, 7);
    SDL_Rect topLeftGrassCorner = SpriteSheet::GetUVTile(0, 9);
    SDL_Rect rightGrassRect = SpriteSheet::GetUVTile(2, 8);
    SDL_Rect bottomRightGrassRect = SpriteSheet::GetUVTile(2, 7);
    SDL_Rect topRightGrassCorner = SpriteSheet::GetUVTile(2, 9);
    SDL_Rect bottomRect = SpriteSheet::GetUVTile(1, 7);
    SDL_Rect topRect = SpriteSheet::GetUVTile(1, 9);

    SDL_Rect fenceRect = SpriteSheet::GetUVTile(8, 4);
    SDL_Rect fenceMRect = SpriteSheet::GetUVTile(9, 4);
    SDL_Rect fenceLRect = SpriteSheet::GetUVTile(10, 4);
    SDL_Rect fenceARect = SpriteSheet::GetUVTile(9, 7);



    SDL_Rect greenTreeRect = SpriteSheet::GetUVTile(4, 8);
    SDL_Rect orangeTreeRect = SpriteSheet::GetSizedUVTile(3, 9, 1, 2);
    SDL_Rect buildingRect = SpriteSheet::GetSizedUVTile(0, 4, 4, 3);
    SDL_Rect doorRect = SpriteSheet::GetUVTile(1, 3);
    SDL_Rect rockRect = SpriteSheet::GetUVTile(7, 7);
    SDL_Rect wellRect = SpriteSheet::GetSizedUVTile(8, 2, 1, 2);
    SDL_Rect stockRect = SpriteSheet::GetSizedUVTile(8, 5, 3, 3);
    SDL_Rect boardRect = SpriteSheet::GetUVTile(11, 4);
    SDL_Rect noTile = SpriteSheet::GetUVTile(-1, 0);

    static const SDL_Rect worldTileCoords{ ceil((float)scene->getWindowWidth() / (float)getWidth()),
                              ceil((float)scene->getWindowHeight() / (float)getHeight()),
                              ceil((float)scene->getWindowWidth() / (float)getWidth()),
                              ceil((float)scene->getWindowHeight() / (float)getHeight()) };

    /// Here I pupulate the m_tiles vector. When it detects a character it is going to scan through switch statement
    /// to determine the attributes for that tile that is going to be rendered at that grid location
    /// than it is pushed in the vector of Tile pointers after what i am going to sort the vector by its scale 
    /// so that sprites won't be overdrawn
    int label = 0;
    float positionX = 0;
    float positionY = 0;

    MATH::Vec3 position = {};

    for (size_t y = 0; y < m_levelData.size(); y++) {

        for (size_t x = 0; x < m_levelData[y].size(); x++) {

            char tile = m_levelData[y][x];

            SDL_Rect gridPosition = worldTileCoords;
            gridPosition.x *= x;
            gridPosition.y *= y;
            Tile* newTile = nullptr;
            Tile* childTile = nullptr;
            Node* newNode = nullptr;

            /// Node position is the centre of that tile that is within the orthographic dimensions
            positionX = static_cast<float>((gridPosition.x + gridPosition.w) * scene->getxAxis()) / scene->getWindowWidth();
            positionY = scene->getyAxis() - (static_cast<float>((gridPosition.y + 0.5 * gridPosition.h) * scene->getyAxis()) / scene->getWindowHeight());
            position = { positionX, positionY, 0.0f };

            newNode = new Node{ label, position };
            if (y == m_levelData.size() - 1&& x == m_levelData[y].size() - 1)
            {
                newNode = nullptr;
			}
				

            switch (tile) {
                
            case 'Y':
                gridPosition.y += 5;
                newTile = new Tile{ nullptr, mapTexture, greenTreeRect, gridPosition, scaleGenerator(randomEngine), true, false };
                break;
            case 'I':
                gridPosition.y += 5;
                newTile = new Tile{ nullptr, mapTexture, orangeTreeRect, gridPosition, scaleGenerator(randomEngine) * 1.5f, true, false };
                break;
            case 'G':
            case 'S':
                // Assuming 'G' and 'S' use the same tile setup
                newTile = new Tile{ nullptr, mapTexture, grassRect, gridPosition, 1.0, false, false, newNode };
                break;
            case 'N':
                newTile = new Tile{ nullptr, mapTexture, topRect, gridPosition, 1.0, false, true, newNode };
                break;
            case 'B':
                newTile = new Tile{ nullptr, mapTexture, bottomRect, gridPosition, 1.0, false, true, newNode };
                break;
            case 'P':
                // Assuming 'P', 'L', and 'R' have similar tile characteristics
                newTile = new Tile{ nullptr, mapTexture, pathRect, gridPosition, 1.0, false, true, newNode };
                break;
            case 'L':
                newTile = new Tile{ nullptr, mapTexture, leftGrassRect, gridPosition, 1.0, false, true, newNode };
                break;
            case 'R':
                newTile = new Tile{ nullptr, mapTexture, rightGrassRect, gridPosition, 1.0, false, true, newNode };
                break;
            case '1':
                newTile = new Tile{ nullptr, mapTexture, bottomLeftGrassCorner, gridPosition, 1.0, false, true, newNode };
                break;
            case '2':
                newTile = new Tile{ nullptr, mapTexture, bottomRightGrassRect, gridPosition, 1.0, false, true, newNode };
                break;
            case 'C':
                childTile = new Tile{ nullptr, mapTexture, buildingRect, gridPosition, 6.0, true, false };
                gridPosition.x += 96;
                newTile = new Tile{ childTile, mapTexture, doorRect, gridPosition, 2.0, true, false, newNode };
                break;
            case ' ':
                newTile = new Tile{ nullptr, mapTexture, noTile, gridPosition, 1.0, false, false, nullptr };
                break;
                
            default:
                printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
                break;
            }

            m_tiles.push_back(newTile);
            walkableTileNodes.push_back(newNode);
            label++;
        }
    }
}

void Level::sortTiles() {

    /// resizing tile so that they are not stretched to tile dimension(especially if scale is more than 1)
    for (auto& tile : m_tiles) {
        tile->resizeTile();
        if (tile->child) {
            tile->child->resizeTile();
        }
    }

    std::sort(m_tiles.begin(), m_tiles.end(), [](const Tile* tile1, const Tile* tile2) {
        return tile1->scale < tile2->scale;
        });
}

void Level::clear() {
    for (auto& tile : m_tiles) {
        if (tile->child) {
            delete tile->child;
        }
        delete tile;
    }
    m_tiles.clear();
}

void Level::drawTiles()
{
    SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 255, 255, 255);

    SDL_RenderCopyEx(scene->game->getRenderer(), background, nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE);

    for (size_t i = 0; i < m_tiles.size(); ++i) {
        SpriteSheet::draw(scene->game->getRenderer(), m_tiles[i]->tileTexture, m_tiles[i]->uvCoords, m_tiles[i]->destCoords, m_tiles[i]->scale, m_tiles[i]->needsResizing);
        if (m_tiles[i]->child) {
            SpriteSheet::draw(scene->game->getRenderer(), m_tiles[i]->child->tileTexture,
                m_tiles[i]->child->uvCoords, m_tiles[i]->child->destCoords, m_tiles[i]->child->scale,
                m_tiles[i]->child->needsResizing);
        }
    }
    //drawTopTileOutline();
}


bool Level::isMouseOverTile(const Tile* tile) {
    const Tile& tempTile = (tile->child) ? *(tile->child) : *tile;

    SDL_Rect adjustedRect = tempTile.destCoords;

    if (tempTile.needsResizing) {
        float aspectRatio = static_cast<float>(tempTile.uvCoords.w) / tempTile.uvCoords.h;
        adjustedRect.w = static_cast<int>(tempTile.destCoords.h * aspectRatio);
    }

    //check if the mouse position is within the bounds of the rectangle
    bool withinXBounds = mousePosX >= adjustedRect.x && mousePosX <= adjustedRect.x + adjustedRect.w;
    bool withinYBounds = mousePosY >= adjustedRect.y && mousePosY <= adjustedRect.y + adjustedRect.h;

    return withinXBounds && withinYBounds;
}

Node* Level::getTileNodeUnderMouse() {
    for (auto& tile : m_tiles) {
        if (isMouseOverTile(tile)) {
            return tile->tileNode;
        }
    }
    return nullptr;
}


void Level::levelHandleEvents(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_MOUSEMOTION:
        mousePosX = event.motion.x;
        mousePosY = event.motion.y;
        break;
    default:
        break;
    }
}

bool Level::canPlaceEntity() {
    // Can place a character if the topmost tile under the mouse is walkable
    return topTile != nullptr && topTile->isWalkable;
}

void Level::drawTopTileOutline() {
    Tile* drawTile = nullptr;

    for (auto it = m_tiles.rbegin(); it != m_tiles.rend(); ++it) {
        Tile* currentTile = *it;
        if (isMouseOverTile(currentTile)) {
            drawTile = currentTile->child ? currentTile->child : currentTile;
            topTile = drawTile;
            break;  // Break because no other tile can be above this one
        }
    }

    if (drawTile != nullptr) {
        SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 255, 255, 255); // white
        SDL_Rect outlineRect = drawTile->destCoords;  //draw the outline
        SDL_RenderDrawRect(scene->game->getRenderer(), &outlineRect);
    }

    if (drawTile != nullptr && drawTile->scale > 1.0f) {
        SDL_SetRenderDrawColor(scene->game->getRenderer(), 255, 0, 0, 255); // red
        SDL_Rect outlineRect = drawTile->destCoords;  //draw the outline
        SDL_RenderDrawRect(scene->game->getRenderer(), &outlineRect);
    }
}


void Tile::resizeTile()
{

    if (needsResizing)
    {
        int originalWidth = destCoords.w;
        int originalHeight = destCoords.h;

        float aspectRatio = static_cast<float>(uvCoords.w) / uvCoords.h;

        destCoords.w = static_cast<int>(originalHeight * aspectRatio * scale);
        destCoords.h = static_cast<int>(originalHeight * scale);

        // Adjust y-position to make the texture scale upwards
        destCoords.y -= (destCoords.h - originalHeight);
    }
    else {
        destCoords.w *= scale;
        destCoords.h *= scale;
    }
}
