#include "Witch.hpp"
#include "ResourcesHolder.hpp"
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

Witch::Witch(Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(toTextureID(type)))
{
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
    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);
}

void Witch::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Witch::setTextureRect(sf::IntRect rect)
{
	mSprite.setTextureRect(rect);
}

sf::FloatRect Witch::getLocalBounds() const
{
	return mSprite.getLocalBounds();
}

unsigned int Witch::getCategory() const
{
	return Category::Player;
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

void Witch::updateCurrent(sf::Time deltaTime)
{
	if (mCurrentAnimation != Attack) move(getVelocity() * deltaTime.asSeconds());
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