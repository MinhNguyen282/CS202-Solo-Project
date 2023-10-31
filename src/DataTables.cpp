#include "include/DataTables.hpp"
#include "include/Witch.hpp"

using namespace std::placeholders;

std::vector<WitchData> initializeWitchData()
{
    std::vector<WitchData> data(Witch::typeCount);

    data[Witch::BlueWitch].hitpoints = 100;
    data[Witch::BlueWitch].speed = 200.f;
    data[Witch::BlueWitch].texture = Textures::BlueWitch;
    data[Witch::BlueWitch].fireInterval = sf::seconds(1);

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::typeCount);

    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 400.f;
    data[Projectile::AlliedBullet].texture = Textures::AlliedBullet;

    data[Projectile::MechaBossRangedAttack].damage = 40;
    data[Projectile::MechaBossRangedAttack].speed = 400.f;
    data[Projectile::MechaBossRangedAttack].texture = Textures::MechaBossRangedAttack;

    return data;
}

MechaBossData initializeMechaBossData()
{
    MechaBossData data;

    data.hitpoints = 1000;
    data.speed = 200.f;
    data.texture = Textures::MechaBoss;
    data.fireInterval = sf::seconds(1);

    Direction direction01(0.f, 50.f);
    Direction direction02(45.f, 50.f);
    Direction direction03(90.f, 50.f);
    Direction direction04(135.f, 50.f);
    Direction direction05(180.f, 50.f);
    Direction direction06(225.f, 50.f);
    Direction direction07(270.f, 50.f);
    Direction direction08(315.f, 50.f);

    data.directions.push_back(direction01);
    data.directions.push_back(direction02);
    data.directions.push_back(direction03);
    data.directions.push_back(direction04);
    data.directions.push_back(direction05);
    data.directions.push_back(direction06);
    data.directions.push_back(direction07);
    data.directions.push_back(direction08);

    return data;
}