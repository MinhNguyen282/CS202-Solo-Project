#include "include/Button.hpp"
#include "include/Utility.hpp"

Button::Button(TextureHolder& textures, FontHolder& fonts, sf::Vector2f position, std::string text, std::function<void()> callback)
: mCallback(callback)
, mTextures(&textures)
, mFonts(&fonts)
{
    mSprite.setTexture(textures.get(Textures::ButtonNormal));
    mSprite.setPosition(position);
    mText.setFont(fonts.get(Fonts::Main));
    mText.setString(text);
    mText.setCharacterSize(20);
    mText.setPosition(position.x + (mSprite.getGlobalBounds().width - mText.getGlobalBounds().width)/2, position.y + (mSprite.getGlobalBounds().height - mText.getGlobalBounds().height)/4);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}   

void Button::update(sf::Time dt, sf::Vector2i mousePos)
{
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    if (mSprite.getGlobalBounds().contains(mousePosF))
    {
        mSprite.setTexture(mTextures->get(Textures::ButtonPressed));
    }
    else
    {
        mSprite.setTexture(mTextures->get(Textures::ButtonNormal));
    }
}

void Button::handleEvent(const sf::Event& event, sf::Vector2i mousePos)
{
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    if (mSprite.getGlobalBounds().contains(mousePosF))
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            mSprite.setTexture(mTextures->get(Textures::ButtonPressed));
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            mCallback();
        }
    }
}

void Button::setCallback(std::function<void()> callback)
{
    mCallback = callback;
}