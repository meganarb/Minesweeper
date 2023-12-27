#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;

class WelcomeScreen {
    float width;
    float height;
public:
    WelcomeScreen(int width, int height);
    string LoadWelcomeScreen(); // loads the welcome window and does the event loop; returns the entered name
    string TypeCharacter(char character, sf::Text text); // for each character typed in the name spot
    static string Backspace(sf::Text text);
};
