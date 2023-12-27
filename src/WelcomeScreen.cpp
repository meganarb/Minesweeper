#include "WelcomeScreen.h"

WelcomeScreen::WelcomeScreen(int width, int height) {
    this->width = 32.0f * width;
    this->height = 32.0f * height + 100;
}

string WelcomeScreen::LoadWelcomeScreen() {

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f (width,height));
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Blue);

    sf::Font myFont;
    myFont.loadFromFile("files/font.ttf");

    sf::Text title("WELCOME TO MINESWEEPER!", myFont, 24);
    sf::FloatRect textRect = title.getLocalBounds();

    title.setOrigin(textRect.width/2, textRect.height/2);
    title.setPosition(width / 2.0f, height / 2.0f - 150);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::Text namePrompt("Enter your name:", myFont, 20);
    textRect = namePrompt.getLocalBounds();

    namePrompt.setOrigin(textRect.width/2, textRect.height/2);
    namePrompt.setPosition(width / 2.0f, height / 2.0f - 75);
    namePrompt.setStyle(sf::Text::Bold);

    sf::Text input("|", myFont, 18);
    textRect = input.getLocalBounds();

    input.setOrigin(textRect.width/2, textRect.height/2);
    input.setPosition(width / 2.0f, height / 2.0f - 45);
    input.setStyle(sf::Text::Bold);
    input.setFillColor(sf::Color::Yellow);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }

            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (input.getString().getSize() != 1) {
                    return input.getString().substring(0, input.getString().find("|"));
                    window.close();
                }
            }

            else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                input.setString(Backspace(input));
            }

            else if (event.type == sf::Event::TextEntered && ((event.text.unicode >= 0x61 && event.text.unicode <= 0x7A)
            || (event.text.unicode >= 0x41 && event.text.unicode <= 0x5A))) {
                input.setString(TypeCharacter(event.text.unicode, input));
            }

            textRect = input.getLocalBounds();
            input.setOrigin(textRect.width/2, textRect.height/2);
        }
        window.clear();
        window.draw(rect);
        window.draw(title);
        window.draw(namePrompt);
        window.draw(input);
        window.display();
    }

}

string WelcomeScreen::TypeCharacter(char character, sf::Text text) {
    if (text.getString().getSize() <= 10) {
        string output;
        if (text.getString().getSize() == 1) {
            output = toupper(character);
            output += text.getString();
            return output;
        }
        output = tolower(character);
        return text.getString().substring(0, text.getString().find('|')) + output + "|";
    }
    return text.getString();
}

string WelcomeScreen::Backspace(sf::Text text) {
    if (text.getString().getSize() != 1) {
        return text.getString().substring(0, text.getString().find('|') - 1) + "|";
    }
}
