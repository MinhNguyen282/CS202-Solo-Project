#ifndef MECHABOSS_HPP
#define MECHABOSS_HPP

#include "Entity.hpp"
#include "TextNode.hpp"
#include "DataTables.hpp"
#include "Projectile.hpp"
#include <map>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class DarkWizzard : public Entity
{
    public:
        enum Animation
        {
            Attack1,
            Attack2,
            Death,
            Idle,
            Run,
            TakedDamage,
            AnimationCount,
        };
    public:
        explicit DarkWizzard(const TextureHolder& textures, const FontHolder& fonts);
        void setTextureRect(sf::IntRect rect);
        virtual sf::FloatRect getBoundingRect() const;

        float getMaxSpeed() const;

        void fireAttack(CommandQueue& commands);

        void setAnimation(Animation animation);
        void setTargetDirection(sf::Vector2f direction);

        int numAnimation();
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
        void doAnimation(sf::Time deltaTime, CommandQueue& commands);
        void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
        void createThunderStrike(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
        void createExplosion(SceneNode& node, float xOffset, float yOffset, const TextureHolder& textures) const;
        virtual unsigned int getCategory() const;
        virtual void rebuildTable();

        void checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands);
        void updateTexts();
    private:
        sf::Sprite mSprite;
        TextNode* mHealthDisplay;

        //Target locked at player
        sf::Vector2f mTargetDirection;

        std::map<Animation, std::tuple<int,int,int>> mAnimation;
        Animation mCurrentAnimation;
        sf::Time mAnimationTime;
        int numRow, curX;

        std::map<Projectile::Type, std::tuple<int,int,int>> mProjectileAnimationMap;

        sf::Time mMovingTime;
        bool mAttackPattern;

        Command mDarkAttackCommand;
        Command mExplosionCommand;
        Command mThunderStrikeCommand;

        int attackLeft;

        bool mIsAngry;
        bool mIsFiring;
        bool mIsExplosion;
        bool mIsThunderStrike;
        bool mIsChasing;
        bool mIsMarkedForRemoval;

        std::size_t mDirectionIndex;
        float mTravelDistance;
};

#endif // MECHABOSS_HPP