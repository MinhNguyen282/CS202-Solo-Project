#ifndef WITCH_HPP
#define WITCH_HPP

#include "Entity.hpp"
#include "TextNode.hpp"
#include "DataTables.hpp"
#include "Projectile.hpp"

#include <map>
#include <math.h>
#include <SFML/Graphics.hpp>

class Witch : public Entity
{
    public:
        enum Type
        {
            BlueWitch,
            typeCount,
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
        explicit Witch(Type type, const TextureHolder& textures, const FontHolder& fonts, sf::RenderWindow& window);
        void setTextureRect(sf::IntRect rect);
        virtual sf::FloatRect getBoundingRect() const;
        virtual unsigned int getCategory() const;
        virtual void updateTexts();
        bool isAllied();

        float getMaxSpeed() const;

        void fire(const sf::Vector2f& target);
        void launchAbility();

        void createBullets(SceneNode& node, const TextureHolder& textures) const;
        void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

        void pushAnimation(Animation animation, int numRow, int width, int height);
        void setAnimation(Animation animation);
        bool isAttack();
        bool isRun();
        bool isCharge();

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
        void updateMovementPattern(sf::Time deltaTime);

        void checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands);
    private:
        Type mType;
        sf::Sprite mSprite;
        sf::RenderWindow& mWindow;

        //Animation
        std::map<Animation, std::tuple<int, int, int>> mAnimationMap;
        int numRow, curX;
        Animation mCurrentAnimation;
        sf::Time mAnimationTime;

        //Animation for projectiles
        std::map<Projectile::Type, std::tuple<int, int, int>> mProjectileAnimationMap;


        Command mFireCommand;
        Command mLaunchAbilityCommand;
        sf::Time mFireCountdown;
        sf::Vector2f mTargetDirection;

        bool mIsFiring;
        bool mIsLaunchingAbility;
        bool mIsMarkedForRemoval;

        int mFireRateLevel;
        int mSpreadLevel;
        int mMissileAmmo;

        Command mDropPickupCommand;
        std::size_t mDirectionIndex;
        float mTravelDistance;
        TextNode* mHealthDisplay;
};

#endif // WITCH_HPP