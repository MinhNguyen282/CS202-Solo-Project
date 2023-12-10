#include "include/DataTables.hpp"
#include "include/Witch.hpp"
#include "include/Enemy.hpp"
#include "include/Particle.hpp"

using namespace std::placeholders;

std::vector<WitchData> initializeWitchData()
{
    std::vector<WitchData> data(Witch::typeCount);

    data[Witch::BlueWitch].hitpoints = 100;
    data[Witch::BlueWitch].speed = 300.f;
    data[Witch::BlueWitch].texture = Textures::BlueWitch;
    data[Witch::BlueWitch].fireInterval = sf::seconds(1);
    data[Witch::BlueWitch].abilityInterval = sf::seconds(5);
    data[Witch::BlueWitch].debuffInterval = sf::seconds(5);

    return data;
}

std::vector<EnemyData> initializeEnemyData()
{
    std::vector<EnemyData> data(Enemy::typeCount);
    std::ifstream ifs("Media/Textures/FlyingEye/info.txt");
    ifs >> data[Enemy::FlyingEye].width >> data[Enemy::FlyingEye].height;
    int x, y, z;
    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].animations.push_back(std::make_tuple(x,y,z));

    ifs >> data[Enemy::FlyingEye].expPoint;

    data[Enemy::FlyingEye].hitpoints = 100;
    data[Enemy::FlyingEye].speed = 150.f;
    data[Enemy::FlyingEye].texture = Textures::FlyingEye;
    data[Enemy::FlyingEye].fireInterval = sf::seconds(1);
    data[Enemy::FlyingEye].bodyDamage = 10;
    data[Enemy::FlyingEye].fireDamage = 10;

    ifs.close();

    
    ifs.open("Media/Textures/FlyingEye/projectileinfo.txt");

    ifs >> x >> y >> z;
    data[Enemy::FlyingEye].projectileAnimation = std::make_tuple(z,x,y);

    ifs.close();

    

    ifs.open("Media/Textures/Goblin/info.txt");
    ifs >> data[Enemy::Goblin].width >> data[Enemy::Goblin].height;
    ifs >> x >> y >> z;
    data[Enemy::Goblin].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Goblin].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Goblin].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Goblin].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Goblin].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Goblin].animations.push_back(std::make_tuple(x,y,z));

    ifs >> data[Enemy::Goblin].expPoint;

    data[Enemy::Goblin].hitpoints = 200;
    data[Enemy::Goblin].speed = 150.f;
    data[Enemy::Goblin].texture = Textures::Goblin;
    data[Enemy::Goblin].fireInterval = sf::seconds(1);
    data[Enemy::Goblin].bodyDamage = 20;
    data[Enemy::Goblin].fireDamage = 20;

    ifs.close();

    ifs.open("Media/Textures/Goblin/projectileinfo.txt");
    ifs >> x >> y >> z;
    data[Enemy::Goblin].projectileAnimation = std::make_tuple(z,x,y);
    ifs.close();

    ifs.open("Media/Textures/Mushroom/info.txt");
    ifs >> data[Enemy::Mushroom].width >> data[Enemy::Mushroom].height;
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].animations.push_back(std::make_tuple(x,y,z));

    ifs >> data[Enemy::Mushroom].expPoint;

    data[Enemy::Mushroom].hitpoints = 300;
    data[Enemy::Mushroom].speed = 250.f;
    data[Enemy::Mushroom].texture = Textures::Mushroom;
    data[Enemy::Mushroom].fireInterval = sf::seconds(1);
    data[Enemy::Mushroom].bodyDamage = 40;
    data[Enemy::Mushroom].fireDamage = 40;

    ifs.close();

    ifs.open("Media/Textures/Mushroom/projectileinfo.txt");
    ifs >> x >> y >> z;
    data[Enemy::Mushroom].projectileAnimation = std::make_tuple(z,x,y);
    ifs.close();

    ifs.open("Media/Textures/Skeleton/info.txt");
    ifs >> data[Enemy::Skeleton].width >> data[Enemy::Skeleton].height;
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].animations.push_back(std::make_tuple(x,y,z));
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].animations.push_back(std::make_tuple(x,y,z));

    ifs >> data[Enemy::Skeleton].expPoint;

    data[Enemy::Skeleton].hitpoints = 400;
    data[Enemy::Skeleton].speed = 200.f;
    data[Enemy::Skeleton].texture = Textures::Skeleton;
    data[Enemy::Skeleton].fireInterval = sf::seconds(1);
    data[Enemy::Skeleton].bodyDamage = 50;
    data[Enemy::Skeleton].fireDamage = 50;

    ifs.close();

    ifs.open("Media/Textures/Skeleton/projectileinfo.txt");
    ifs >> x >> y >> z;
    data[Enemy::Skeleton].projectileAnimation = std::make_tuple(z,x,y);
    ifs.close();

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::typeCount);

    data[Projectile::AlliedBullet].damage = 100;
    data[Projectile::AlliedBullet].speed = 600.f;
    data[Projectile::AlliedBullet].texture = Textures::AlliedBullet;

    data[Projectile::AlliedSkillE].damage = 25;
    data[Projectile::AlliedSkillE].speed = 500.f;
    data[Projectile::AlliedSkillE].texture = Textures::AlliedSkillE;

    data[Projectile::AlliedSkillQ].damage = 100;
    data[Projectile::AlliedSkillQ].speed = 0.f;
    data[Projectile::AlliedSkillQ].texture = Textures::AlliedSkillQ;

    data[Projectile::MechaBossRangedAttack].damage = 40;
    data[Projectile::MechaBossRangedAttack].speed = 600.f;
    data[Projectile::MechaBossRangedAttack].texture = Textures::MechaBossRangedAttack;

    data[Projectile::FlyingEyeBullet].damage = 10;
    data[Projectile::FlyingEyeBullet].speed = 200.f;
    data[Projectile::FlyingEyeBullet].texture = Textures::FlyingEyeBullet;

    data[Projectile::GoblinBullet].damage = 20;
    data[Projectile::GoblinBullet].speed = 300.f;
    data[Projectile::GoblinBullet].texture = Textures::GoblinBullet;

    data[Projectile::MushroomBullet].damage = 40;
    data[Projectile::MushroomBullet].speed = 400.f;
    data[Projectile::MushroomBullet].texture = Textures::MushroomBullet;

    data[Projectile::SkeletonBullet].damage = 50;
    data[Projectile::SkeletonBullet].speed = 400.f;
    data[Projectile::SkeletonBullet].texture = Textures::SkeletonBullet;

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

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(Particle::ParticleCount);

	data[Particle::Propellant].color = sf::Color(255, 255, 50);
	data[Particle::Propellant].lifetime = sf::seconds(0.6f);

	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].lifetime = sf::seconds(4.f);

	return data;
}