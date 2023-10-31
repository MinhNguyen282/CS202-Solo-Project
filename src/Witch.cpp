#include "include/Witch.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/Utility.hpp"
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
	const std::vector<WitchData> Table = initializeWitchData();
}

Witch::Witch(Type type, const TextureHolder& textures, const FontHolder& fonts, sf::RenderWindow& window)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(toTextureID(type)))
, mHealthDisplay(nullptr)
, mTravelDistance(0.f)
, mDirectionIndex(0)
, mFireCountdown(sf::Time::Zero)
, mFireRateLevel(1)
, mIsMarkedForRemoval(false)
, mIsLaunchingAbility(false)
, mIsFiring(false)
, mWindow(window)
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

	mProjectileAnimationMap[Projectile::AlliedBullet] = std::make_tuple(4, 32, 21);

	mFireCommand.category = Category::Scene;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node,textures);
	};

    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
}

void Witch::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::AlliedBullet;
	createProjectile(node, type, 0.5f, 0.0f, textures);
}

void Witch::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures, std::get<0>(mProjectileAnimationMap.at(type)), std::get<1>(mProjectileAnimationMap.at(type)), std::get<2>(mProjectileAnimationMap.at(type))));
	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(projectile->getMaxSpeed(), projectile->getMaxSpeed());
	float sign = 1.f;
	
	//get character position
	sf::Vector2f initPos = getWorldPosition();

	//get mouse position
	sf::Vector2i pos = sf::Mouse::getPosition(mWindow);
	sf::Vector2f worldPos = mWindow.mapPixelToCoords(pos);

	//get angle
	float angle = std::atan2(worldPos.y - initPos.y, worldPos.x - initPos.x);
	velocity.x = projectile->getMaxSpeed() * std::cos(angle);
	velocity.y = projectile->getMaxSpeed() * std::sin(angle);

	//rotate projectile
	projectile->setRotation(toDegree(angle));

	//get sign
	if (angle > 0 && angle < 3.14f) sign = -1.f;
	
	projectile->setPosition(getWorldPosition() + sf::Vector2f(5,5));
	projectile->setVelocity(velocity);
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
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
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
	return Table[mType].speed;
}

void Witch::setAnimation(Animation animation)
{
	if (mCurrentAnimation == animation) return;
	if (mCurrentAnimation == Attack && animation != Idle) return;
	mCurrentAnimation = animation;
	curX = 0;
	numRow = 0;
	for(size_t i=0; i<NumAnimation; i++){
		if (i == animation) break;
		curX += std::get<1>(mAnimationMap[(Animation)i]);
	}
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

	checkProjectileLaunch(deltaTime, mCommandQueue);
	Entity::updateCurrent(deltaTime, mCommandQueue);
	updateMovementPattern(deltaTime);

	move(getVelocity() * getMaxSpeed() * deltaTime.asSeconds());
	if (getVelocity().x == 0 && getVelocity().y==0 && mCurrentAnimation == Walk) setAnimation(Witch::Idle);
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
			if (isAttack()) setAnimation(Witch::Idle), width = std::get<1>(mAnimationMap[mCurrentAnimation]), height = std::get<2>(mAnimationMap[mCurrentAnimation]);
		}
		mSprite.setTextureRect(sf::IntRect(curX, height * numRow, width, height));
	}
}

bool Witch::isAllied()
{
	return true;
}

void Witch::fire(const sf::Vector2f& target)
{
	if (Table[mType].fireInterval != sf::Time::Zero)
	{
		mIsFiring = true;
		mTargetDirection = target - getWorldPosition();
	}
}

void Witch::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
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
		mIsLaunchingAbility = false;
	}
}

void Witch::updateMovementPattern(sf::Time deltaTime)
{
	const std::vector<Direction>& direction = Table[mType].directions;
	if (direction.empty()) return;
	if (mTravelDistance > (direction[mDirectionIndex].distance))
	{
		mDirectionIndex = (mDirectionIndex + 1) % direction.size();
		mTravelDistance = 0.f;
	}
	float radians = toRadian(direction[mDirectionIndex].angle + 90.f);
	float vx = getMaxSpeed() * std::cos(radians);
	float vy = getMaxSpeed() * std::sin(radians);

	setVelocity(vx, vy);

	mTravelDistance += getMaxSpeed() * deltaTime.asSeconds();
}