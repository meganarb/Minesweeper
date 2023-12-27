#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameWindow {

    struct Tile {
        bool isMine;
        bool hasFlag;
        bool revealed;
        int minesNearby;
        int rightEdge;
        int leftEdge;
        int topEdge;
        int botEdge;
        sf::Sprite tileSprite;
        sf::Sprite numSprite;
        sf::Sprite flagSprite;
        sf::Sprite mineSprite;
    };
    int width;
    int height;
    int numMines;
    int tilesRevealed;
    std::vector<int> mines;
    std::vector<sf::Sprite> mineSprites;
    std::vector<sf::Sprite> pauseTiles;
    std::map<std::string, sf::Texture> textures;
    std::vector<sf::Texture> digits;
    std::map<int, Tile> tiles;
    std::vector<sf::Sprite> numbers;
    std::vector<sf::Sprite> counter;
    bool pause;
    bool debugMode;
    bool lose;
    bool win;
    int mineCounter;

public:
    GameWindow(int width, int height, int numBombs);
    int LoadGameWindow();
    void LoadAllTiles();
    Tile LoadTile(int id);
    void LoadMines();
    void LoadTextures();
    void RevealTile(int id); // for left click
    void PlaceFlag(int id);
    int MinesNearby(int id);
    void ChangeMineCounter();
    void ResetBoard();
    void PauseTiles();
};


