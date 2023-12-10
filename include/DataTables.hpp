#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "ResourcesIdentifier.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <functional>

class Aircraft;

struct Direction
{
    Direction(float angle, float distance)
    : angle(angle)
    , distance(distance)
    {
    }
    float angle;
    float distance;
};

struct WitchData
{
    int hitpoints;
    float speed;
    Textures::ID texture;
    sf::Time fireInterval;
    sf::Time abilityInterval;
    sf::Time debuffInterval;
    std::vector<Direction> directions;
};

struct EnemyData
{
    int width, height;
    int hitpoints;
    int bodyDamage;
    int fireDamage;
    int expPoint;
    float speed;
    Textures::ID texture;
    sf::Time fireInterval;
    std::vector<std::tuple<int,int,int>> animations;
    std::tuple<int,int,int> projectileAnimation;
};

struct ProjectileData
{
    int damage;
    float speed;
    Textures::ID texture;
};

struct PickupData
{
    std::function<void(Aircraft&)> action;
    Textures::ID texture;
};

struct MechaBossData
{
    int hitpoints;
    float speed;
    Textures::ID texture;
    sf::Time fireInterval;
    std::vector<Direction> directions;
};

struct ParticleData
{
	sf::Color						color;
	sf::Time						lifetime;
};

std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<WitchData> initializeWitchData();
std::vector<EnemyData> initializeEnemyData();
MechaBossData initializeMechaBossData();
std::vector<ParticleData>	initializeParticleData();


#endif // DATATABLES_HPP    