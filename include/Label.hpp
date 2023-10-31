#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"
#include "ResourcesHolder.hpp"
#include "ResourcesIdentifier.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace GUI
{
    class Label : public Component 
    {
        public:
            typedef std::shared_ptr<Label> Ptr;
        public:
            Label(const std::string& text, const FontHolder& fonts);
            virtual bool isSelectable() const;
            void setText(const std::string& text);
            void setTextColor(const sf::Color& color);
            void setTextSize(int size);
            void setTextPosition(float x, float y);
            virtual void handleEvent(const sf::Event& event);
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        private:
            sf::Text mText;
    };
}

#endif // LABEL_HPP