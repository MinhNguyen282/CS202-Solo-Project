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

    std::string temp;

    ifs >> temp >> data[Witch::BlueWitch].hitpoints;
    ifs >> temp >> data[Witch::BlueWitch].speed;
    int x;
    ifs >> temp >> x;
    data[Witch::BlueWitch].fireInterval = sf::seconds(x);
    ifs >> temp >> x;
    data[Witch::BlueWitch].abilityInterval = sf::seconds(x);
    ifs >> temp >> x;
    data[Witch::BlueWitch].debuffInterval = sf::seconds(x);
    ifs >> temp >> x;
    data[Witch::BlueWitch].ultimateInterval = sf::seconds(x);
    ifs >> temp >> data[Witch::BlueWitch].mCoolDown;
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
    data[Enemy::FlyingEye].texture = Textures::FlyingEye;
    ifs.close();

    ifs.open("Data/Enemies/FlyingEye.dat");
    std::string temp;
    ifs >> temp >> data[Enemy::FlyingEye].hitpoints;
    ifs >> temp >> data[Enemy::FlyingEye].speed;
    ifs >> temp >> x;
    data[Enemy::FlyingEye].fireInterval = sf::seconds(x);
    ifs >> temp >> data[Enemy::FlyingEye].bodyDamage;
    ifs >> temp >> data[Enemy::FlyingEye].fireDamage;
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
    ifs.close();
    data[Enemy::Goblin].texture = Textures::Goblin;

    ifs.open("Data/Enemies/Goblin.dat");

    ifs >> temp >> data[Enemy::Goblin].hitpoints;
    ifs >> temp >> data[Enemy::Goblin].speed;
    ifs >> temp >> x;
    data[Enemy::Goblin].fireInterval = sf::seconds(x);
    ifs >> temp >> data[Enemy::Goblin].bodyDamage;
    ifs >> temp >> data[Enemy::Goblin].fireDamage;

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
    data[Enemy::Mushroom].texture = Textures::Mushroom;
    ifs.close();

    ifs.open("Data/Enemies/Mushroom.dat");

    ifs >> temp >> data[Enemy::Mushroom].hitpoints;
    ifs >> temp >> data[Enemy::Mushroom].speed;
    ifs >> temp >> x;
    data[Enemy::Mushroom].fireInterval = sf::seconds(x);
    ifs >> temp >> data[Enemy::Mushroom].bodyDamage;
    ifs >> temp >> data[Enemy::Mushroom].fireDamage;

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
    data[Enemy::Skeleton].texture = Textures::Skeleton;
    ifs.close();

    ifs.open("Data/Enemies/Skeleton.dat");

    ifs >> temp >> data[Enemy::Skeleton].hitpoints;
    ifs >> temp >> data[Enemy::Skeleton].speed;
    ifs >> temp >> x;
    data[Enemy::Skeleton].fireInterval = sf::seconds(x);
    ifs >> temp >> data[Enemy::Skeleton].bodyDamage;
    ifs >> temp >> data[Enemy::Skeleton].fireDamage;

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

    std::string temp;

    std::ifstream ifs("Data/Character/BlueWitch/ProjectilePower.dat");

    ifs >> temp >> data[Projectile::AlliedBullet].damage;
    ifs >> temp >> data[Projectile::AlliedBullet].speed;
    data[Projectile::AlliedBullet].texture = Textures::AlliedBullet;

    ifs >> temp >> data[Projectile::AlliedSkillE].damage;
    ifs >> temp >> data[Projectile::AlliedSkillE].speed;
    data[Projectile::AlliedSkillE].texture = Textures::AlliedSkillE;

    ifs >> temp >> data[Projectile::AlliedSkillQ].damage;
    ifs >> temp >> data[Projectile::AlliedSkillQ].speed;
    data[Projectile::AlliedSkillQ].texture = Textures::AlliedSkillQ;

    ifs >> temp >> data[Projectile::AlliedUltimate].damage;
    ifs >> temp >> data[Projectile::AlliedUltimate].speed;
    int x; ifs >> temp >> x;
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

    ifs.open("Data/Projectiles/FlyingEye.dat");
    ifs >> temp >> data[Projectile::FlyingEyeBullet].damage;
    ifs >> temp >> data[Projectile::FlyingEyeBullet].speed;
    data[Projectile::FlyingEyeBullet].texture = Textures::FlyingEyeBullet;
    ifs.close();

    ifs.open("Data/Projectiles/Goblin.dat");
    ifs >> temp >> data[Projectile::GoblinBullet].damage;
    ifs >> temp >> data[Projectile::GoblinBullet].speed;
    data[Projectile::GoblinBullet].texture = Textures::GoblinBullet;
    ifs.close();

    ifs.open("Data/Projectiles/Mushroom.dat");
    ifs >> temp >> data[Projectile::MushroomBullet].damage;
    ifs >> temp >> data[Projectile::MushroomBullet].speed;
    data[Projectile::MushroomBullet].texture = Textures::MushroomBullet;
    ifs.close();

    ifs.open("Data/Projectiles/Skeleton.dat");
    ifs >> temp >> data[Projectile::SkeletonBullet].damage;
    ifs >> temp >> data[Projectile::SkeletonBullet].speed;
    data[Projectile::SkeletonBullet].texture = Textures::SkeletonBullet;
    ifs.close();

    ifs.open("Data/Projectiles/DarkAttack.dat");
    ifs >> temp >> data[Projectile::DarkAttack].damage;
    ifs >> temp >> data[Projectile::DarkAttack].speed;
    data[Projectile::DarkAttack].texture = Textures::DarkAttack;
    ifs.close();

    ifs.open("Data/Projectiles/ExplosionAttack.dat");
    ifs >> temp >> data[Projectile::ExplosionAttack].damage;
    ifs >> temp >> data[Projectile::ExplosionAttack].speed;
    data[Projectile::ExplosionAttack].texture = Textures::ExplosionAttack;
    ifs.close();

    ifs.open("Data/Projectiles/ThunderStrike.dat");
    ifs >> temp >> data[Projectile::ThunderStrike].damage;
    ifs >> temp >> data[Projectile::ThunderStrike].speed;
    data[Projectile::ThunderStrike].texture = Textures::ThunderStrike;
    ifs.close();

    return data;
}

DarkWizzardData initializeDarkWizzardData()
{
    std::cout << "Dark Wizzard Data Called" << std::endl;

    DarkWizzardData data;
    data.texture = Textures::DarkWizzard;

    std::ifstream ifs("Data/Enemies/DarkWizzard.dat");
    std::string temp;
    ifs >> temp >> data.hitpoints;
    ifs >> temp >> data.speed;
    int x;
    ifs >> temp >> x;
    data.fireInterval = sf::seconds(x);
    ifs.close();

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