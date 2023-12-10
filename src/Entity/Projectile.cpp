#include "include/Projectile.hpp"
#include "include/DataTables.hpp"
#include "include/Utility.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/Enemy.hpp"

#include <cmath>
#include <cassert>

namespace 
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures, int mCol, int wS, int hS)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTargetDirection()
, curCol(0)
, mDamgeUp(0)
, mAnimationTime(sf::Time::Zero)
{
    maxCol = mCol;
    widthSprite = wS;
    heightSprite = hS;
    mSprite.setTextureRect(sf::IntRect(curCol * wS, 0, wS, hS));
    centerOrigin(mSprite);
}

void Projectile::guideToward(sf::Vector2f position)
{
    assert(isGuided());
    mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
    return mType == Missile;
}

Projectile::Type Projectile::getType() const
{
    return mType;
}

unsigned int Projectile::getCategory() const
{
    if (mType == AlliedBullet || mType == AlliedSkillE)
        return Category::AlliedProjectile;
    else
        return Category::EnemyProjectile;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (isGuided())
    {
        const float approachRate = 200.f;

        sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
        newVelocity *= getMaxSpeed();
        float angle = std::atan2(newVelocity.y, newVelocity.x);

        setRotation(toDegree(angle) + 90.f);
        setVelocity(newVelocity);
    }

    mAnimationTime += dt;
    if (mAnimationTime >= sf::seconds(0.1f))
    {
        mAnimationTime = sf::Time::Zero;
        curCol++;
        if (curCol == maxCol)
        {
            if (mType==AlliedSkillQ){
                destroy();
                Command debuffCommand;
                debuffCommand.category = Category::Enemy;
                debuffCommand.action = derivedAction<Enemy>([this](Enemy& enemy, sf::Time)
                {
                    enemy.debuff(getDamage(), sf::seconds(2.f));
                    enemy.setAnimation(Enemy::TakedDamage);
                    enemy.damage(getDamage()/5);
                });
                commands.push(debuffCommand);
            }
            curCol = 0;
        }
        mSprite.setTextureRect(sf::IntRect(curCol * widthSprite, 0, widthSprite, heightSprite));
    }

    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

sf::FloatRect Projectile::getBoundingRect() const
{
    sf::FloatRect rect = getWorldTransform().transformRect(mSprite.getGlobalBounds());
    if (mType == AlliedBullet){
        float height = 25.0;
        float diffHei = rect.height - height;
        rect.height -= diffHei;
        rect.top += diffHei / 2;
        float width = 30;
        float diffWid = rect.width - width;
        rect.width -= diffWid;
        rect.left += diffWid / 2;
    }
    if (mType == AlliedSkillE){
        float height = 96.0;
        float diffHei = rect.height - height;
        rect.height -= diffHei;
        rect.top += diffHei / 2;
        float width = 100;
        float diffWid = rect.width - width;
        rect.width -= diffWid;
        rect.left += diffWid / 2;
    }
    if (mType == FlyingEyeBullet){
        float height = 35.0;
        float diffHei = rect.height - height;
        rect.height -= diffHei;
        rect.top += diffHei / 2;
        float width = 35.0;
        float diffWid = rect.width - width;
        rect.width -= diffWid;
        rect.left += diffWid / 2;
    }
    if (mType == GoblinBullet)
    {
        float height = 35.0;
        float diffHei = rect.height - height;
        rect.height -= diffHei;
        rect.top += diffHei / 2;
        float width = 35.0;
        float diffWid = rect.width - width;
        rect.width -= diffWid;
        rect.left += diffWid / 2;
    }
    if (mType == MushroomBullet)
    {
        float height = 35.0;
        float diffHei = rect.height - height;
        rect.height -= diffHei;
        rect.top += diffHei / 2;
        float width = 35.0;
        float diffWid = rect.width - width;
        rect.width -= diffWid;
        rect.left += diffWid / 2;
    }
    if (mType == SkeletonBullet)
    {
        float height = 35.0;
        float diffHei = rect.height - height;
        rect.height -= diffHei;
        rect.top += diffHei / 2;
        float width = 35.0;
        float diffWid = rect.width - width;
        rect.width -= diffWid;
        rect.left += diffWid / 2;
    }
    return rect;   
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

int Projectile::getDamage() const
{
    return Table[mType].damage + mDamgeUp;
}

void Projectile::addDamage(int damage)
{
    mDamgeUp += damage;
}

sf::Vector2f Projectile::getTargetDirection() const
{
    return mTargetDirection;
}