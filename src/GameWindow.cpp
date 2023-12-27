#include "GameWindow.h"
#include "random.h"

GameWindow::GameWindow(int width, int height, int numMines) {
    this->width = width;
    this->height = height;
    this->numMines = numMines;
    tilesRevealed = 0;
    mineCounter = numMines;
    pause = false;
    debugMode = false;
    lose = false;
    win = false;
}

int GameWindow::LoadGameWindow() {
    sf::RenderWindow window(sf::VideoMode(width * 32.0f, height * 32.0f + 100), "Minesweeper", sf::Style::Close);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f (width * 32.0f, 100));
    rect.setPosition(0, height * 32.0f);
    rect.setFillColor(sf::Color::White);

    LoadTextures();

    sf::Sprite happyFace;
    happyFace.setTexture(textures.at("happy"));
    happyFace.setPosition(width / 2.0f * 32 - 32, 32 * (height + 0.5f));

    sf::Sprite debug;
    debug.setTexture(textures.at("debug"));
    debug.setPosition(width * 32.0f - 304, 32 * (height + 0.5f));

    sf::Sprite play;
    play.setTexture(textures.at("pause"));
    play.setPosition(width * 32.0f - 240, 32 * (height + 0.5f));

    sf::Sprite leaderboard;
    leaderboard.setTexture(textures.at("leaderboard"));
    leaderboard.setPosition(width * 32.0f - 176, 32 * (height + 0.5f));

    sf::Sprite counter1;
    sf::Sprite counter2;
    sf::Sprite counter3;
    sf::Sprite counter4;
    counter1.setPosition(33, 32 * ((height) + 0.5f) + 16);
    counter2.setPosition(54, 32 * ((height) + 0.5f) + 16);
    counter3.setPosition(75, 32 * ((height) + 0.5f) + 16);
    counter4.setPosition(12, 32 * ((height) + 0.5f) + 16);
    counter4.setTexture(digits.at(10));
    counter.push_back(counter1);
    counter.push_back(counter2);
    counter.push_back(counter3);
    counter.push_back(counter4);

    ChangeMineCounter();

    LoadAllTiles();
    PauseTiles();

    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            else if (!win && !lose && !pause && sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
            sf::Mouse::getPosition(window).y <= height * 32) {
                for (int i = 1; i < width * height + 1; i++) {
                    if (sf::Mouse::getPosition(window).x < tiles.at(i).rightEdge &&
                    sf::Mouse::getPosition(window).x > tiles.at(i).leftEdge &&
                    sf::Mouse::getPosition(window).y > tiles.at(i).topEdge &&
                    sf::Mouse::getPosition(window).y < tiles.at(i).botEdge &&
                        !tiles.at(i).revealed) {
                        if (tiles.at(i).isMine) {
                            lose = true;
                            happyFace.setTexture(textures.at("lose"));
                        }
                        RevealTile(i);
                        std::cout << tilesRevealed << " " << width * height - numMines << std::endl;
                        if (!lose && tilesRevealed == width * height - numMines) {
                            win = true;
                        }
                    }
                }
            }
            else if (!win && !lose && !pause && sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
            sf::Mouse::getPosition(window).y <= height * 32) {
                for (int i = 1; i < width * height + 1; i++) {
                    if (sf::Mouse::getPosition(window).x < tiles.at(i).rightEdge &&
                    sf::Mouse::getPosition(window).x > tiles.at(i).leftEdge &&
                        sf::Mouse::getPosition(window).y > tiles.at(i).topEdge &&
                        sf::Mouse::getPosition(window).y < tiles.at(i).botEdge &&
                        !tiles.at(i).revealed) {

                        PlaceFlag(i);
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::getPosition(window).y > 32 *(height + 0.5f)
            && sf::Mouse::getPosition(window).y < 32 * (height + 0.5f) + 64) {
                if (sf::Mouse::getPosition(window).x > width / 2.0f * 32 - 32 &&
                sf::Mouse::getPosition(window).x < width / 2.0f * 32 + 32) {
                    window.clear();
                    ResetBoard();
                    happyFace.setTexture(textures.at("happy"));
                }
                else if (!win && !lose && !pause && sf::Mouse::getPosition(window).x > width * 32.0f - 304 &&
                sf::Mouse::getPosition(window).x < width * 32.0f - 304 + 64) {
                    if (debugMode) {
                        debugMode = false;
                    }
                    else {
                        debugMode = true;
                    }
                }
                else if (!win && !lose && sf::Mouse::getPosition(window).x > width * 32.0f - 240 &&
                sf::Mouse::getPosition(window).x < width * 32.0f - 240 + 64) {
                    if (pause) {
                        pause = false;
                        play.setTexture(textures.at("pause"));
                    }
                    else {
                        pause = true;
                        play.setTexture(textures.at("play"));
                    }
                }
                else if (sf::Mouse::getPosition(window).x > width * 32.0f - 176 &&
                sf::Mouse::getPosition(window).x < width * 32.0f - 176 + 64) {
                    std::cout << "leaderboard" << std::endl;
                }
            }
        }
        window.clear();
        window.draw(rect);
        window.draw(happyFace);
        window.draw(debug);
        window.draw(play);
        window.draw(leaderboard);
        for (int i = 0; i < 3; i++) {
            window.draw(counter.at(i));
        }
        if (mineCounter < 0) {
            window.draw(counter.at(3));
        }
        for (int i = 1; i < (width * height) + 1; i++) {
            window.draw(tiles.at(i).tileSprite);
        }
        for (const auto & number : numbers) {
            window.draw(number);
        }
        for (int i = 1; i < width * height + 1; i++) {
            if (tiles.at(i).hasFlag) {
                window.draw(tiles.at(i).flagSprite);
            }
        }
        if (debugMode) {
            for (int i = 0; i < mineSprites.size(); i++) {
                window.draw(mineSprites.at(i));
            }
        }
        if (pause) {
            for (int i = 0; i < pauseTiles.size(); i++) {
                window.draw(pauseTiles.at(i));
            }
        }

        if (lose) {
            for (int i = 0; i < mineSprites.size(); i++) {
                window.draw(mineSprites.at(i));
            }
        }
        if (win) {
            happyFace.setTexture(textures.at("win"));
        }
        window.display();
    }
}

void GameWindow::LoadTextures() {
    sf::Texture temp;
    temp.loadFromFile("files/images/debug.png");
    textures.emplace("debug", temp);
    temp.loadFromFile("files/images/face_happy.png");
    textures.emplace("happy", temp);
    temp.loadFromFile("files/images/face_lose.png");
    textures.emplace("lose", temp);
    temp.loadFromFile("files/images/face_win.png");
    textures.emplace("win", temp);
    temp.loadFromFile("files/images/flag.png");
    textures.emplace("flag", temp);
    temp.loadFromFile("files/images/leaderboard.png");
    textures.emplace("leaderboard", temp);
    temp.loadFromFile("files/images/mine.png");
    textures.emplace("mine", temp);
    temp.loadFromFile("files/images/number_1.png");
    textures.emplace("1", temp);
    temp.loadFromFile("files/images/number_2.png");
    textures.emplace("2", temp);
    temp.loadFromFile("files/images/number_3.png");
    textures.emplace("3", temp);
    temp.loadFromFile("files/images/number_4.png");
    textures.emplace("4", temp);
    temp.loadFromFile("files/images/number_5.png");
    textures.emplace("5", temp);
    temp.loadFromFile("files/images/number_6.png");
    textures.emplace("6", temp);
    temp.loadFromFile("files/images/number_7.png");
    textures.emplace("7", temp);
    temp.loadFromFile("files/images/number_8.png");
    textures.emplace("8", temp);
    temp.loadFromFile("files/images/pause.png");
    textures.emplace("pause", temp);
    temp.loadFromFile("files/images/play.png");
    textures.emplace("play", temp);
    temp.loadFromFile("files/images/tile_hidden.png");
    textures.emplace("hidden", temp);
    temp.loadFromFile("files/images/tile_revealed.png");
    textures.emplace("revealed", temp);

    for (int i = 0; i < 10; i++) {
        temp.loadFromFile("files/images/digits.png", sf::IntRect(21 * i, 0, 21, 32));
        digits.push_back(temp);
    }
    temp.loadFromFile("files/images/digits.png", sf::IntRect(210, 0, 21, 32));
    digits.push_back(temp);
}

void GameWindow::LoadAllTiles() {
    LoadMines();
    for (int i = 1; i < (width * height) + 1; i++) {
        tiles.emplace(i, LoadTile(i));
    }
}

GameWindow::Tile GameWindow::LoadTile(int id) {
    Tile tile;
    tile.rightEdge = ((id - 1) % width + 1) * 32;
    tile.leftEdge = ((id - 1) % width) * 32;
    tile.topEdge = ((id - 1) / width * 32);
    tile.botEdge = ((id - 1) / width + 1) * 32;
    tile.hasFlag = false;
    tile.revealed = false;
    tile.tileSprite.setTexture(textures.at("hidden"));
    tile.tileSprite.setPosition(tile.leftEdge, tile.topEdge);
    tile.numSprite.setPosition(tile.leftEdge, tile.topEdge);
    tile.flagSprite.setPosition(tile.leftEdge, tile.topEdge);
    tile.flagSprite.setTexture(textures.at("flag"));
    if (std::find(mines.begin(), mines.end(), id) != mines.end()) {
        tile.isMine = true;
        tile.mineSprite.setPosition(tile.leftEdge, tile.topEdge);
        tile.mineSprite.setTexture(textures.at("mine"));
        mineSprites.push_back(tile.mineSprite);
    }
    else {
        tile.isMine = false;
    }
    tile.minesNearby = MinesNearby(id);
    return tile;
}

void GameWindow::LoadMines() {
    int minesRemaining = numMines;
    int tilesRemaining = width * height;
    for (int i = 1; i < width * height + 1; i++) {
        if (minesRemaining == tilesRemaining || Random::Int(1, (int)ceil(1.0 / (minesRemaining * j0(1) / tilesRemaining))) == 1) {
            mines.push_back(i);
            minesRemaining -= 1;
        }
        tilesRemaining -= 1;
    }
}

int GameWindow::MinesNearby(int id) {
    int amount = 0;
    if (std::find(mines.begin(), mines.end(), id - width - 1) != mines.end() && id % width != 1) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id - width) != mines.end()) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id - width + 1) != mines.end() && id % width != 0) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id - 1) != mines.end() && id % width != 1) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id + 1) != mines.end() && id % width != 0) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id + width - 1) != mines.end() && id % width != 1) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id + width) != mines.end()) {
        amount += 1;
    }
    if (std::find(mines.begin(), mines.end(), id + width + 1) != mines.end() && id % width != 0) {
        amount += 1;
    }
    return amount;
}

void GameWindow::RevealTile(int id) {
    if (!tiles.at(id).revealed && !tiles.at(id).hasFlag && !lose) {
        tiles.at(id).revealed = true;
        tilesRevealed += 1;
        if (tiles.at(id).minesNearby != 0) {
            tiles.at(id).numSprite.setTexture(textures.at(std::to_string(tiles.at(id).minesNearby)));
            numbers.push_back(tiles.at(id).numSprite);
        } else {
            if (id % width != 0) { RevealTile(id + 1); }
            if (id % width != 1) { RevealTile(id - 1); }
            if (id > width) { RevealTile(id - width); }
            if (id > width - 1 && id % width != 0) { RevealTile(id - width + 1); }
            if (id > width + 1 && id % width != 1) { RevealTile(id - width - 1); }
            if (id <= width * height - width) { RevealTile(id + width); }
            if (id <= width * height - width + 1 && id % width != 1) { RevealTile(id + width - 1); }
            if (id <= width * height - width - 1 && id % width != 0) { RevealTile(id + width + 1); }
        }
        tiles.at(id).tileSprite.setTexture(textures.at("revealed"));
    }
    else if (lose) {
        tiles.at(id).tileSprite.setTexture(textures.at("revealed"));
    }
}

void GameWindow::PlaceFlag(int id) {
    if (tiles.at(id).hasFlag) {
        tiles.at(id).hasFlag = false;
        mineCounter += 1;
    }
    else {
        tiles.at(id).hasFlag = true;
        mineCounter -= 1;
    }
    ChangeMineCounter();
}

void GameWindow::ChangeMineCounter() {
    int num;
    if (mineCounter >= -999 && mineCounter <= 999) {
        if (mineCounter >= 0) {
            num = mineCounter % 10;
            counter.at(2).setTexture(digits.at(num));

            num = mineCounter / 10 % 10;
            counter.at(1).setTexture(digits.at(num));

            num = mineCounter / 100 % 10;
            counter.at(0).setTexture(digits.at(num));
        }
        else {
            num = mineCounter * -1 % 10;
            counter.at(2).setTexture(digits.at(num));

            num = mineCounter / -10 % 10;
            counter.at(1).setTexture(digits.at(num));

            num = mineCounter / -100 % 10;
            counter.at(0).setTexture(digits.at(num));
        }
    }
}

void GameWindow::ResetBoard() {
    mines.clear();
    numbers.clear();
    pause = false;
    lose = false;
    win = false;
    debugMode = false;
    tilesRevealed = 0;
    mineCounter = numMines;
    ChangeMineCounter();
    tiles.clear();
    mineSprites.clear();
    LoadAllTiles();
}

void GameWindow::PauseTiles() {
    for (int i = 1; i < width * height + 1; i++) {
        sf::Sprite tile;
        tile.setTexture(textures.at("revealed"));
        tile.setPosition(((i - 1) % width) * 32, (i - 1) / width * 32);
        pauseTiles.push_back(tile);
    }
}