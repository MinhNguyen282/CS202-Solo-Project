#ifndef WITCH_HPP
#define WITCH_HPP

#include "Entity.hpp"
#include "TextNode.hpp"
#include "DataTables.hpp"
#include "Projectile.hpp"

#include <map>
#include <math.h>
#include <vector>
#include <fstream>
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
        explicit Witch(Type type, const TextureHolder& textures, const FontHolder& fonts);
        void setTextureRect(sf::IntRect rect);
        virtual sf::FloatRect getBoundingRect() const;
        virtual unsigned int getCategory() const;
        virtual void rebuildTable();
        bool isAllied();
        bool isMarkedForRemoval() const;
        void receiveExp(int expPoint);
        float getHealthRatio() const;
        float getExpRatio() const;
        float getCoolDownE() const;
        float getCoolDownQ() const;
        float getCoolDownUltimate() const;
        int getLevel() const;

        void getTarget(sf::Vector2f target);

        float getMaxSpeed() const;

        void fire();
        void launchAbility();
        void laundDebuff();
        void launchUltimate();

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

        void checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands);
        void updateTexts();
    private:
        Type mType;
        sf::Sprite mSprite;

        //Animation
        std::map<Animation, std::tuple<int, int, int>> mAnimationMap;
        int numRow, curX;
        Animation mCurrentAnimation;
        sf::Time mAnimationTime;

        //Animation for projectiles
        std::map<Projectile::Type, std::tuple<int, int, int>> mProjectileAnimationMap;


        Command mFireCommand;
        Command mLaunchAbilityCommand;
        Command mLaunchDebuffCommand;
        Command mLaunchUltimateCommand;

        sf::Time mFireCountdown;
        sf::Time mAbilityCountdown;
        sf::Time mDebuffCountdown;
        sf::Time mUltimateCountdown;

        sf::Vector2f mTargetDirection;

        bool mIsFiring;
        bool mIsLaunchingAbility;
        bool mIsLauchingDebuff;
        bool mIsLauchingUltimate;

        bool mIsMarkedForRemoval;

        int mFireRateLevel;
        int mSpreadLevel;
        int mMissileAmmo;

        //level system
        int level;
        int curExpPoint;
        int expCap[10];
        float mLevelUpSpeed;

        //hit points system
        int maxHitPoints;
        int mDamageUp;

        //ability cooldown system
        float mCoolDownE;

        Command mDropPickupCommand;
        std::size_t mDirectionIndex;
        float mTravelDistance;
        TextNode* mHealthDisplay;
};

#endif // WITCH_HPP