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

class MechaBoss : public Entity
{
    public:
        enum Animation
        {
            Idle,
            TakedDamage,
            RangedAttack,
            SkillAttack,
            Shield,
            Die,
            AnimationCount,
        };
    public:
        explicit MechaBoss(const TextureHolder& textures, const FontHolder& fonts);
        void setTextureRect(sf::IntRect rect);
        virtual sf::FloatRect getBoundingRect() const;

        float getMaxSpeed() const;

        void fireAttack();
        void skillAttack(const sf::Vector2f& target);

        void createBullets(SceneNode& node, const TextureHolder& textures) const;
        void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

        void setAnimation(Animation animation);
        void setTargetDirection(sf::Vector2f direction);

        int numAnimation();
    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
        virtual unsigned int getCategory() const;

        void updateMovementPattern(sf::Time deltaTime);

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

        Command mFireCommand;
        Command mSkillCommand;
        sf::Time mFireCountdown;
        
        bool mIsFiring;
        bool mIsSkill;
        bool mIsMarkedForRemoval;

        std::size_t mDirectionIndex;
        float mTravelDistance;
};

#endif // MECHABOSS_HPP