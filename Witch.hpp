#ifndef WITCH_HPP
#define WITCH_HPP

#include "Entity.hpp"
#include <SFML/Graphics.hpp>

class Witch : public Entity
{
    public:
        enum Type
        {
            BlueWitch,
        };
    public:
        explicit Witch(Type type, const TextureHolder& textures);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        Type mType;
        sf::Sprite mSprite;
};

#endif // WITCH_HPP