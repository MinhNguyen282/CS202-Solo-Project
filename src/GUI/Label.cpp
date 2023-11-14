#include "include/Label.hpp"
#include "include/Utility.hpp"

#include <SFML/Graphics.hpp>

namespace GUI
{
    Label::Label(const std::string& text, const FontHolder& fonts)
    : mText(text, fonts.get(Fonts::Main), 16)
    {
    }

    bool Label::isSelectable() const
    {
        return false;
    }

    void Label::setTextColor(const sf::Color& color)
    {
        mText.setFillColor(color);
    }

    void Label::setTextSize(int size)
    {
        mText.setCharacterSize(size);
    }

    void Label::setTextPosition(float x, float y)
    {
        mText.setPosition(x, y);
    }

    void Label::handleEvent(const sf::Event& event)
    {
    }

    void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(mText, states);
    }

    void Label::setText(const std::string& text)
    {
        mText.setString(text);
    }
}
