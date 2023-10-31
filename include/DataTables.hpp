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
    std::vector<Direction> directions;
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


std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<WitchData> initializeWitchData();
MechaBossData initializeMechaBossData();

#endif // DATATABLES_HPP    