#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Window/Keyboard.hpp>

#include <sstream>

namespace sf
{
    class Sprite;
    class Text;
}

template<typename T>
std::string toString(const T&  value);

std::string toString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include "Utility.inl"
#endif // UTILITY_HPP