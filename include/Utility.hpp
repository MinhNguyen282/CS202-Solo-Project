#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <sstream>

namespace sf
{
    class Sprite;
    class Text;
}

template<typename T>
std::string toString(const T&  value);

std::string toString(sf::Keyboard::Key key);

sf::Keyboard::Key toKeyboardKey(std::string key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

float toDegree(float radian);
float toRadian(float degree);

int randomInt(int exclusiveMax);

float length(sf::Vector2f vector);
sf::Vector2f unitVector(sf::Vector2f vector);

#include "Utility.inl"
#endif // UTILITY_HPP