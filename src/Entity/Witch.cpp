#include "include/Witch.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/Utility.hpp"
#include "include/SoundNode.hpp"
#include "include/Projectile.hpp"
#include <iostream>

Textures::ID toTextureID(Witch::Type type)
{
	switch (type)
	{
		case Witch::BlueWitch:
			return Textures::BlueWitch;
	}
	return Textures::BlueWitch;
}

namespace
{
	std::vector<WitchData> pTable = initializeWitchData();
}

Witch::Witch(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(pTable[type].hitpoints)
, maxHitPoints(pTable[type].hitpoints)
, mType(type)
, mCoolDownE(pTable[type].mCoolDown)
, mSprite(textures.get(toTextureID(type)))
, mHealthDisplay(nullptr)
, mTravelDistance(0.f)
, mDirectionIndex(0)
, mFireCountdown(sf::Time::Zero)
, mFireRateLevel(1)
, mIsMarkedForRemoval(false)
, mIsLaunchingAbility(false)
, mIsLauchingDebuff(false)
, mIsLauchingUltimate(false)
, mIsFiring(false)
, level(1)
, curExpPoint(0)
, mLevelUpSpeed(0.f)
, mDamageUp(0)
, isLevelUp(false)
{
	if (mType == BlueWitch){
		mSprite.setTextureRect(sf::IntRect(0, 0, 64, 96));
		pushAnimation(Witch::Idle, 6, 64, 96);
		pushAnimation(Witch::TakedDamage, 3, 64, 96);
		pushAnimation(Witch::Walk, 8, 64, 96);
		pushAnimation(Witch::Charge, 5, 96, 96);
		pushAnimation(Witch::Die, 12, 64, 80);
		pushAnimation(Witch::Attack, 9, 217, 96);
		mCurrentAnimation = Idle;
		curX = 0;
		numRow = 0;
	}

	//set level system
	expCap[0] = 0;
	expCap[1] = 100;
	expCap[2] = 200;
	expCap[3] = 300;
	expCap[4] = 400;
	expCap[5] = 500;
	expCap[6] = 600;
	expCap[7] = 700;
	expCap[8] = 800;
	expCap[9] = 900;

	mProjectileAnimationMap[Projectile::AlliedBullet] = std::make_tuple(4, 32, 21);
	mProjectileAnimationMap[Projectile::AlliedSkillE] = std::make_tuple(4, 142, 96);
	mProjectileAnimationMap[Projectile::AlliedSkillQ] = std::make_tuple(9, 96, 96);
	mProjectileAnimationMap[Projectile::AlliedUltimate] = std::make_tuple(16, 165, 165);

	mFireCommand.category = Category::Scene;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node,textures);
	};

	mLaunchAbilityCommand.category = Category::Scene;
	mLaunchAbilityCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::AlliedSkillE, 0.0f, 0.0f, textures);
	};

	mLaunchDebuffCommand.category = Category::Scene;
	mLaunchDebuffCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::AlliedSkillQ, 0.0f, 0.0f, textures);
	};

	mLaunchUltimateCommand.category = Category::Scene;
	mLaunchUltimateCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::AlliedUltimate, 0.0f, 0.0f, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
	mHealthDisplay = healthDisplay.get();
	mHealthDisplay->setColor(sf::Color::White);
	attachChild(std::move(healthDisplay));

    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);
}

void Witch::rebuildTable()
{
	pTable = initializeWitchData();
	maxHitPoints = pTable[mType].hitpoints;
	Entity::heal(std::max(0, pTable[mType].hitpoints - getHitpoints()));
	mCoolDownE = pTable[mType].mCoolDown;
}

void Witch::setTable(WitchData table)
{
	pTable[mType] = table;
	maxHitPoints = pTable[mType].hitpoints;
}

void Witch::setLevel(int level)
{
	this->level = level;
}

void Witch::setExp(int exp)
{
	curExpPoint = exp;
}

int Witch::getExp() const
{
	return curExpPoint;
}

void Witch::receiveExp(int expPoint)
{
	if (level == 10) return;
	curExpPoint += expPoint;
	if (curExpPoint >= expCap[level]){
		level++;
		curExpPoint = 0;
		isLevelUp = true;
		std::cout << "Witch level up\n";
	}
}

void Witch::addHitpoints(){
	maxHitPoints += 10;
	Entity::heal(10);
}

void Witch::addDamage(){
	mDamageUp += 10;
}

void Witch::addMovementSpeed(){
	mLevelUpSpeed += 10;	
}

float Witch::getHealthRatio() const
{
	return static_cast<float>(getHitpoints()) / maxHitPoints;
}

float Witch::getExpRatio() const
{
	return static_cast<float>(curExpPoint) / expCap[level];
}

int Witch::getLevel() const
{
	return level;
}

void Witch::getTarget(sf::Vector2f target)
{
	mTargetDirection = target;
}

void Witch::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::AlliedBullet;
	createProjectile(node, type, 0.0f, 0.0f, textures);
}

bool Witch::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

void Witch::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures, std::get<0>(mProjectileAnimationMap.at(type)), std::get<1>(mProjectileAnimationMap.at(type)), std::get<2>(mProjectileAnimationMap.at(type))));
	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(projectile->getMaxSpeed(), projectile->getMaxSpeed());
	if (type == Projectile::AlliedSkillE) projectile->setScale(-1,1);
	float sign = 1.f;
	
	//get character position
	sf::Vector2f initPos = getWorldPosition();

	//get mouse position
	sf::Vector2f worldPos = mTargetDirection;

	//get angle
	float angle = std::atan2(worldPos.y - initPos.y, worldPos.x - initPos.x);
	velocity.x = projectile->getMaxSpeed() * std::cos(angle);
	velocity.y = projectile->getMaxSpeed() * std::sin(angle);

	//rotate projectile
	if (type != Projectile::AlliedSkillQ) projectile->setRotation(toDegree(angle));

	//get sign
	if (angle > 0 && angle < 3.14f) sign = -1.f;
	
	projectile->setPosition(getWorldPosition() + sf::Vector2f(5,5));
	projectile->setVelocity(velocity);
	projectile->addDamage(mDamageUp);
	node.attachChild(std::move(projectile));
}

void Witch::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);

}

void Witch::setTextureRect(sf::IntRect rect)
{
	mSprite.setTextureRect(rect);
}

sf::FloatRect Witch::getBoundingRect() const
{
	sf::FloatRect rect = getWorldTransform().transformRect(mSprite.getGlobalBounds());
	rect.width -= 30;
	rect.left += 15;
	rect.height -= 30;
	rect.top += 15;
	return rect;
}

unsigned int Witch::getCategory() const
{
	return Category::Player;
}

void Witch::updateTexts()
{
	mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());
}

float Witch::getMaxSpeed() const
{
	return pTable[mType].speed + mLevelUpSpeed;
}

void Witch::setAnimation(Animation animation)
{
	if (mCurrentAnimation == Die || (mCurrentAnimation==TakedDamage)) return;
	if (mCurrentAnimation == animation) return;
	mCurrentAnimation = animation;
	curX = 0;
	numRow = 0;
	for(size_t i=0; i<NumAnimation; i++){
		if (i == animation) break;
		curX += std::get<1>(mAnimationMap[(Animation)i]);
	}
	mAnimationTime = sf::Time::Zero;
}

bool Witch::isAttack()
{
	return mCurrentAnimation == Attack;
}

bool Witch::isRun()
{
	return mCurrentAnimation == Walk;
}

bool Witch::isCharge()
{
	return mCurrentAnimation == Charge;
}

void Witch::pushAnimation(Animation animation, int maxNumRow, int width, int height)
{
	mAnimationMap[animation] = std::make_tuple(maxNumRow, width, height);
}

void Witch::updateCurrent(sf::Time deltaTime, CommandQueue& mCommandQueue)
{
	if (isDestroyed()){
		mIsMarkedForRemoval = true;
		return;
	}

	mAbilityCountdown -= deltaTime;
	mDebuffCountdown -= deltaTime;
	mUltimateCountdown -= deltaTime;
	checkProjectileLaunch(deltaTime, mCommandQueue);
	Entity::updateCurrent(deltaTime, mCommandQueue);
	updateTexts();

	if (getVelocity().x < 0) mSprite.setScale(-1.f, 1.f);
	if (getVelocity().x > 0) mSprite.setScale(1.f, 1.f);
	if (getVelocity().x == 0 && getVelocity().y == 0) setAnimation(Idle);

	int maxNumRow = std::get<0>(mAnimationMap[mCurrentAnimation]);
	int width = std::get<1>(mAnimationMap[mCurrentAnimation]);
	int height = std::get<2>(mAnimationMap[mCurrentAnimation]);
	
	mAnimationTime += deltaTime;

	if (mAnimationTime >= sf::seconds(0.1f))
	{
		mAnimationTime = sf::Time::Zero;
		numRow++;
		if (numRow == maxNumRow)
		{
			numRow = 0;
			if (mCurrentAnimation == TakedDamage) {
				mCurrentAnimation = Attack;
				setAnimation(Idle);
			}
			if (mCurrentAnimation == Die) mIsMarkedForRemoval = true;
		}
		mSprite.setTextureRect(sf::IntRect(curX, height * numRow, width, height));
	}
}

bool Witch::isAllied()
{
	return true;
}

void Witch::fire()
{
	if (pTable[mType].fireInterval != sf::Time::Zero)
	{
		mIsFiring = true;
	}
}

void Witch::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		Entity::playLocalSound(commands, SoundEffect::AlliedFiring);
		mFireCountdown += pTable[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}

	if (mIsLaunchingAbility)
	{
		commands.push(mLaunchAbilityCommand);
		Entity::playLocalSound(commands, SoundEffect::AlliedAbility);
		mIsLaunchingAbility = false;
	}
	if (mIsLauchingDebuff)
	{
		commands.push(mLaunchDebuffCommand);
		Entity::playLocalSound(commands, SoundEffect::AlliedDebuff);
		mIsLauchingDebuff = false;
	}
	if (mIsLauchingUltimate)
	{
		commands.push(mLaunchUltimateCommand);
		Entity::playLocalSound(commands, SoundEffect::AlliedUltimate);
		mIsLauchingUltimate = false;
	}
}

void Witch::launchAbility()
{
	if (mAbilityCountdown <= sf::Time::Zero){
		mAbilityCountdown = sf::Time::Zero;
		mAbilityCountdown += pTable[mType].abilityInterval - sf::seconds(mCoolDownE * pTable[mType].abilityInterval.asSeconds());
		mIsLaunchingAbility = true;
	}
}

void Witch::laundDebuff()
{
	if (mDebuffCountdown <= sf::Time::Zero){
		mDebuffCountdown = sf::Time::Zero;
		mDebuffCountdown += pTable[mType].debuffInterval - sf::seconds(mCoolDownE * pTable[mType].debuffInterval.asSeconds());
		mIsLauchingDebuff = true;
		std::cout << "Debuff" << std::endl;
	}
}

void Witch::launchUltimate()
{
	if (mUltimateCountdown <= sf::Time::Zero){
		mUltimateCountdown = sf::Time::Zero;
		mUltimateCountdown += pTable[mType].ultimateInterval - sf::seconds(mCoolDownE * pTable[mType].ultimateInterval.asSeconds());
		mIsLauchingUltimate = true;
	}
}

float Witch::getCoolDownE() const
{
	return std::max(0.0f, mAbilityCountdown.asSeconds()) / (pTable[mType].abilityInterval.asSeconds() - mCoolDownE * pTable[mType].abilityInterval.asSeconds());
}

float Witch::getCoolDownQ() const
{
	return std::max(0.0f, mDebuffCountdown.asSeconds()) / (pTable[mType].debuffInterval.asSeconds() - mCoolDownE * pTable[mType].debuffInterval.asSeconds());
}

float Witch::getCoolDownUltimate() const
{
	return std::max(0.0f, mUltimateCountdown.asSeconds()) / (pTable[mType].ultimateInterval.asSeconds() - mCoolDownE * pTable[mType].ultimateInterval.asSeconds());
}