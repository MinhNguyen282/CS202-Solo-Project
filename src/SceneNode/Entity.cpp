#include "include/Entity.hpp"

Entity::Entity(int hitpoints)
: mVelocity()
, mHitpoints(hitpoints)
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::heal(int points)
{
    mHitpoints += points;
}

void Entity::damage(int points)
{
    mHitpoints -= points;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::setHitpoints(int points)
{
    mHitpoints = points;
}

bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}

void Entity::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
    move(mVelocity * deltaTime.asSeconds());
}

void Entity::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

void Entity::rebuildTable()
{
    //do nothing
}

void Entity::playLocalSound(CommandQueue& commands, SoundEffect::ID effect, float volume)
{
    sf::Vector2f worldPosition = getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(
        [effect, worldPosition, volume] (SoundNode& node, sf::Time)
        {
            node.playSound(effect, worldPosition);
            node.setVolume(volume);
        });
    commands.push(command);
}