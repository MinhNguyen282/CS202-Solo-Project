#include "include/Projectile.hpp"
#include "include/DataTables.hpp"
#include "include/Utility.hpp"
#include "include/ResourcesHolder.hpp"

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

unsigned int Projectile::getCategory() const
{
    if (mType == EnemyBullet)
        return Category::EnemyProjectile;
    else
        return Category::AlliedProjectile;
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
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

int Projectile::getDamage() const
{
    return Table[mType].damage;
}