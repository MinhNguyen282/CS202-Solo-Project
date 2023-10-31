#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "ResourcesHolder.hpp"
#include "ResourcesIdentifier.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace GUI
{
    class Button : public Component
    {
        public:
            typedef std::shared_ptr<Button> Ptr;
            typedef std::function<void()> Callback;

        public:
            Button(const FontHolder& fonts, const TextureHolder& textures);
            Button(const FontHolder& fonts, const TextureHolder& textures, Textures::ID normalTexture, Textures::ID selectedTexture, Textures::ID pressedTexture);
            void setCallback(Callback callback);
            void setText(const std::string& text);
            void setToggle(bool flag);
            void setTextColor(const sf::Color& color);
            void setTextSize(int size);
            void setTextPosition(float x, float y);
            sf::FloatRect getBoundingRect() const;
            sf::FloatRect getTextBounding() const;

            virtual bool isSelectable() const;
            virtual void select();
            virtual void deselect();
            
            virtual void activate();
            virtual void deactivate();

            virtual void handleEvent(const sf::Event& event);

        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            Callback mCallback;
            const sf::Texture& mNormalTexture;
            const sf::Texture& mSelectedTexture;
            const sf::Texture& mPressedTexture;
            sf::Sprite mSprite;
            sf::Text mText;
            bool mIsToggle;
    };
}

#endif // BUTTON_HPP