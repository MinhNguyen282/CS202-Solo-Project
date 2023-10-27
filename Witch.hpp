#ifndef WITCH_HPP
#define WITCH_HPP

#include "Entity.hpp"
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

class Witch : public Entity
{
    public:
        enum Type
        {
            BlueWitch,
        };
        enum Animation
        {
            Idle,
            TakedDamage,
            Walk,
            Charge,
            Die,
            Attack,
            NumAnimation,
        };
    public:
        explicit Witch(Type type, const TextureHolder& textures);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void setTextureRect(sf::IntRect rect);
        sf::FloatRect getLocalBounds() const;
        unsigned int getCategory() const;
        void pushAnimation(Animation animation, int numRow, int width, int height);
        void setAnimation(Animation animation);
        bool isAttack();
        bool isRun();
        bool isCharge();

    private:
        void updateCurrent(sf::Time deltaTime);

    private:
        Type mType;
        sf::Sprite mSprite;
        std::map<Animation, std::tuple<int, int, int>> mAnimationMap;
        int numRow, curX;
        Animation mCurrentAnimation;
        sf::Time mAnimationTime;
};

#endif // WITCH_HPP