#include "include/DataTables.hpp"
#include "include/Witch.hpp"
#include "include/Enemy.hpp"
#include "include/Particle.hpp"

using namespace std::placeholders;

std::vector<WitchData> initializeWitchData()
{
    std::vector<WitchData> data(Witch::typeCount);
    data[Witch::BlueWitch].texture = Textures::BlueWitch;

    std::cout << "Data called" << std::endl;

    std::ifstream ifs("Data/Character/BlueWitch/BasePower.dat");

    ifs >> data[Witch::BlueWitch].hitpoints;
    ifs >> data[Witch::BlueWitch].speed;
    int x;
    ifs >> x;
    data[Witch::BlueWitch].fireInterval = sf::seconds(x);
    ifs >> x;
    data[Witch::BlueWitch].abilityInterval = sf::seconds(x);
    ifs >> x;
    data[Witch::BlueWitch].debuffInterval = sf::seconds(x);
    ifs >> x;
    data[Witch::BlueWitch].ultimateInterval = sf::seconds(x);
    ifs >> data[Witch::BlueWitch].mCoolDown;
    data[Witch::BlueWitch].mCoolDown /= 100.f;
    ifs.close();

    ifs.open("Data/Upgrade/BlueWitch/UpgradePower.dat");
    int y,z; ifs >> x >> y >> z;
    data[Witch::BlueWitch].hitpoints += x;
    ifs >> x >> y >> z;
    data[Witch::BlueWitch].speed += x;
    ifs >> x >> y >> z;
    data[Witch::BlueWitch].mCoolDown += (float)x/100.f;
    ifs.close();

    return data;
}

std::vector<EnemyData> initializeEnemyData()
{
    std::cout << "Enemy Data Called" << std::endl;

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
    std::cout << "Projectile Data Called" << std::endl;

    std::vector<ProjectileData> data(Projectile::typeCount);

    std::ifstream ifs("Data/Character/BlueWitch/ProjectilePower.dat");

    ifs >> data[Projectile::AlliedBullet].damage;
    ifs >> data[Projectile::AlliedBullet].speed;
    data[Projectile::AlliedBullet].texture = Textures::AlliedBullet;

    ifs >> data[Projectile::AlliedSkillE].damage;
    ifs >> data[Projectile::AlliedSkillE].speed;
    data[Projectile::AlliedSkillE].texture = Textures::AlliedSkillE;

    ifs >> data[Projectile::AlliedSkillQ].damage;
    ifs >> data[Projectile::AlliedSkillQ].speed;
    data[Projectile::AlliedSkillQ].texture = Textures::AlliedSkillQ;

    ifs >> data[Projectile::AlliedUltimate].damage;
    ifs >> data[Projectile::AlliedUltimate].speed;
    int x; ifs >> x;
    data[Projectile::AlliedUltimate].lifetime = sf::seconds(x);
    data[Projectile::AlliedUltimate].texture = Textures::AlliedUltimate;
    ifs.close();

    ifs.open("Data/Upgrade/BlueWitch/ProjectilePower.dat");
    int y, z;
    ifs >> x >> y >> z;
    data[Projectile::AlliedBullet].damage += x;
    ifs >> x >> y >> z;
    data[Projectile::AlliedSkillE].damage += x;
    ifs >> x >> y >> z;
    data[Projectile::AlliedSkillQ].damage += x;
    ifs >> x >> y >> z;
    data[Projectile::AlliedUltimate].damage += x;
    ifs.close();

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

    data[Projectile::DarkAttack].damage = 100;
    data[Projectile::DarkAttack].speed = 300.f;
    data[Projectile::DarkAttack].texture = Textures::DarkAttack;

    data[Projectile::ExplosionAttack].damage = 50;
    data[Projectile::ExplosionAttack].speed = 300.f;
    data[Projectile::ExplosionAttack].texture = Textures::ExplosionAttack;

    data[Projectile::ThunderStrike].damage = 50;
    data[Projectile::ThunderStrike].speed = 300.f;
    data[Projectile::ThunderStrike].texture = Textures::ThunderStrike;

    return data;
}

DarkWizzardData initializeDarkWizzardData()
{
    std::cout << "Dark Wizzard Data Called" << std::endl;

    DarkWizzardData data;

    data.hitpoints = 10000;
    data.speed = 300.f;
    data.texture = Textures::DarkWizzard;
    data.fireInterval = sf::seconds(1);

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