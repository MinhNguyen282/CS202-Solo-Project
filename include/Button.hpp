#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ResourcesHolder.hpp"
#include "ResourcesIdentifier.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>
#include <functional>
#include <vector>

class Button : public sf::Drawable, public sf::Transformable
{
    public:
        Button(TextureHolder& textures, FontHolder& fonts, sf::Vector2f position, std::string text, std::function<void()> callback);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update(sf::Time dt, sf::Vector2i mousePos);
        void handleEvent(const sf::Event& event, sf::Vector2i mousePos);
        void setCallback(std::function<void()> callback);
        void setSprite(Textures::ID normal, Textures::ID pressed);
    private:
        sf::Sprite mSprite;
        sf::Text mText;
        Textures::ID mNormal, mPressed;
        std::function<void()> mCallback;
        TextureHolder* mTextures;
        FontHolder* mFonts;
};

#endif // BUTTON_HPP