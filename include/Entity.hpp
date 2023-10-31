#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "SceneNode.hpp"
#include "CommandQueue.hpp"

class Entity : public SceneNode
{
    public:
        explicit Entity(int hitpoints);
        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
        sf::Vector2f getVelocity() const;
        void accelerate(sf::Vector2f velocity);
        void accelerate(float vx, float vy);

        void heal(int points);
        void damage(int points);
        void destroy();

        int getHitpoints() const;
        virtual bool isDestroyed() const;

    protected:
        virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
    
    private: 
        sf::Vector2f mVelocity;
        int mHitpoints;
};

#endif // ENTITY_HPP