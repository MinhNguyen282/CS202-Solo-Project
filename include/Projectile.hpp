#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"
#include "ResourcesIdentifier.hpp"
#include "CommandQueue.hpp"

#include <SFML/Graphics.hpp>
#include <array>

class Projectile : public Entity
{
    public:
        enum Type
        {
            AlliedBullet,
            AlliedSkillE,
            AlliedSkillQ,
            FlyingEyeBullet,
            GoblinBullet,
            MushroomBullet,
            SkeletonBullet,
            Missile,
            MechaBossRangedAttack,
            MechaBossSkillAttack,
            typeCount,
        };
    public:
        Projectile(Type type, const TextureHolder& textures, int mCol, int wS, int hS);
        void guideToward(sf::Vector2f position);
        bool isGuided() const;
        void pushAnimation(int mCol, int wS, int hS);
        Type getType() const;

        virtual unsigned int getCategory() const;
        virtual sf::FloatRect getBoundingRect() const;
        float getMaxSpeed() const;
        int getDamage() const;
        void addDamage(int damage);
        sf::Vector2f getTargetDirection() const;
    private:
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        Type mType;
        sf::Sprite mSprite;
        sf::Vector2f mTargetDirection;
        int curCol;
        int maxCol;
        int widthSprite;
        int heightSprite;
        int mDamgeUp;
        sf::Time mAnimationTime;
};

#endif // PROJECTILE_HPP