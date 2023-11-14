#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "TextNode.hpp"
#include "DataTables.hpp"
#include "Projectile.hpp"

#include <map>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

class Enemy : public Entity
{
    public:
        enum Type
        {
            FlyingEye,
            Goblin,
            Mushroom,
            Skeleton,
            typeCount,
        };
        enum Animation
        {
            Attack1,
            Death,
            Move,
            TakedDamage,
            Attack2,
            Attack3,
            AnimationCount,
        };
    public:
        explicit Enemy(Type type, const TextureHolder& textures, const FontHolder& fonts);
        virtual sf::FloatRect getBoundingRect() const;
        bool isMarkedForRemoval() const;

        float getMaxSpeed() const;

        void fireAttack(CommandQueue& commands);
        void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
        void setAnimation(Animation animation);
        void doAnimation(sf::Time deltaTime);
        void setTargetDirection(sf::Vector2f direction);
        void updateMovementPattern(sf::Time deltaTime);
        int getBodyDamage() const;
        int getFireDamage() const;

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
        virtual unsigned int getCategory() const;
        void readEnemyData(Type type);
    private:
        Type mType;
        sf::Sprite mSprite;
        TextNode* mHealthDisplay;
        sf::Time mFireCountdown;

        sf::Vector2f mTargetDirection;

        std::map<Animation, std::tuple<int,int,int>> mAnimation;
        Animation mCurrentAnimation;
        sf::Time mAnimationTime;
        int numRow, curX;

        std::map<Projectile::Type, std::tuple<int,int,int>> mProjectileAnimationMap;

        Command mAttack3;

        bool mIsNearPlayer;
        bool mIsAttack;
        bool mIsMarkedForRemoval;
};

#endif // ENEMY_HPP