#ifndef TEXTNODE_HPP
#define TEXTNODE_HPP

#include "SceneNode.hpp"
#include "ResourcesHolder.hpp"
#include "ResourcesIdentifier.hpp"

#include <SFML/Graphics.hpp>    

class TextNode : public SceneNode
{
    public:
        explicit TextNode(const FontHolder& fonts, const std::string& text);
        void setString(const std::string& text);
        void setColor(const sf::Color& color);
        void setCharacterSize(int size);
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        sf::Text mText;
};

#endif // TEXTNODE_HPP