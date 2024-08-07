#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
    sf::RenderWindow &window;
    sf::FloatRect buttonRect;
    sf::RectangleShape buttonShape;
    sf::Text text;
    sf::Font font;
    std::string buttonString;

    bool hovered;
    bool clicked;

    Button(int x, int y, int width, int height, sf::RenderWindow &window,
           const char *str, const char *fontPath, int xOffset, int yOffset, int fontSize);
    ~Button();

    bool update();
    void draw();
};
