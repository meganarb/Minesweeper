#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "WelcomeScreen.h"
#include "GameWindow.h"
#include <fstream>
using namespace std;

int main()
{
    // reading the info from the config file into vars for width, height, and number of mines
    ifstream file("files/board_config.cfg");
    string line;
    int width, height, numMines;
    getline(file, line);
    width = stoi(line);
    getline(file, line);
    height = stoi(line);
    getline(file, line);
    numMines = stoi(line);
    WelcomeScreen screen(width, height);
    string playerName = screen.LoadWelcomeScreen();

    if (playerName.empty()) {
        return 0;
    }

    cout << "Hello " << playerName << "!" << endl;

    GameWindow game(width, height, numMines);
    return game.LoadGameWindow();
}

