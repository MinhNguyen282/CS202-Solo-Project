#include "include/Button.hpp"
#include "include/Utility.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace GUI
{
    Button::Button(const FontHolder& fonts, const TextureHolder& textures)
    : mCallback()
    , mNormalTexture(textures.get(Textures::ButtonNormal))
    , mSelectedTexture(textures.get(Textures::ButtonSelected))
    , mPressedTexture(textures.get(Textures::ButtonPressed))
    , mSprite()
    , mText("", fonts.get(Fonts::Main), 16)
    , mIsToggle(false)
    {
        mSprite.setTexture(mNormalTexture);

        sf::FloatRect bounds = mSprite.getLocalBounds();
        mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    Button::Button(const FontHolder& fonts, const TextureHolder& textures, Textures::ID normalTexture, Textures::ID selectedTexture, Textures::ID pressedTexture)
    : mCallback()
    , mNormalTexture(textures.get(normalTexture))
    , mSelectedTexture(textures.get(selectedTexture))
    , mPressedTexture(textures.get(pressedTexture))
    , mSprite()
    , mText("", fonts.get(Fonts::Main), 16)
    , mIsToggle(false)
    {
        mSprite.setTexture(mNormalTexture);

        sf::FloatRect bounds = mSprite.getLocalBounds();
        mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::setCallback(Callback callback)
    {
        mCallback = std::move(callback);
    }

    void Button::setText(const std::string& text)
    {
        mText.setString(text);
    }

    void Button::setToggle(bool flag)
    {
        mIsToggle = flag;
    }

    void Button::setTextColor(const sf::Color& color)
    {
        mText.setFillColor(color);
    }

    void Button::setTextSize(int size)
    {
        mText.setCharacterSize(size);
    }

    void Button::setTextPosition(float x, float y)
    {
        mText.setPosition(x, y);
    }

    sf::FloatRect Button::getBoundingRect() const
    {
        return mSprite.getGlobalBounds();
    }

    sf::FloatRect Button::getTextBounding() const
    {
        return mText.getLocalBounds();
    }

    bool Button::isSelectable() const
    {
        return true;
    }

    void Button::select()
    {
        Component::select();

        mSprite.setTexture(mSelectedTexture);
    }

    void Button::deselect()
    {
        Component::deselect();

        mSprite.setTexture(mNormalTexture);
    }

    void Button::activate()
    {
        Component::activate();

        // If we are toggle then we should show that the button is pressed and thus "toggled".
        if (mIsToggle)
            mSprite.setTexture(mPressedTexture);

        if (mCallback)
            mCallback();

        // If we are not a toggle then deactivate the button since we are just momentarily activated.
        if (!mIsToggle)
            deactivate();
    }

    void Button::deactivate()
    {
        Component::deactivate();

        if (mIsToggle)
        {
            if (isSelected())
                mSprite.setTexture(mSelectedTexture);
            else
                mSprite.setTexture(mNormalTexture);
        }
    }

    void Button::handleEvent(const sf::Event& event)
    {

    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(mSprite, states);
        target.draw(mText, states);
    }
}