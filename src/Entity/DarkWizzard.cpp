#include "include/DarkWizzard.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/Utility.hpp"
#include "include/Projectile.hpp"

namespace
{
    DarkWizzardData  Table = initializeDarkWizzardData();
}

DarkWizzard::DarkWizzard(const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table.hitpoints)
, mSprite(textures.get(Textures::DarkWizzard))
, mTravelDistance(0.f)
, mDirectionIndex(0)
, mMovingTime(sf::Time::Zero)
, mAttackPattern(false)
, mIsAngry(false)
, mIsMarkedForRemoval(false)
, mIsExplosion(false)
, mIsThunderStrike(false)
, mIsChasing(false)
, attackLeft(0)
, mIsFiring(true)
{
    Table = initializeDarkWizzardData();
    Entity::heal(std::max(0, Table.hitpoints - getHitpoints()));
    mSprite.setTextureRect(sf::IntRect(0, 0, 381, 381));
    mAnimation[Attack1] = std::make_tuple(8, 381, 381);
    mAnimation[Attack2] = std::make_tuple(8, 381, 381);
    mAnimation[Death] = std::make_tuple(7, 381, 381);
    mAnimation[Idle] = std::make_tuple(8, 381, 381);
    mAnimation[Run] = std::make_tuple(8, 381, 381);
    mAnimation[TakedDamage] = std::make_tuple(3, 381, 381);
    setAnimation(Idle);

    mProjectileAnimationMap[Projectile::DarkAttack] = std::make_tuple(10, 165, 132);
    mProjectileAnimationMap[Projectile::ExplosionAttack] = std::make_tuple(18, 144, 144);
    mProjectileAnimationMap[Projectile::ThunderStrike] = std::make_tuple(13, 128, 128);

    mDarkAttackCommand.category = Category::Scene;
    mDarkAttackCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::DarkAttack, 0.f, 0.f, textures);
        createProjectile(node, Projectile::DarkAttack, 1.f, 0.f, textures);
        createProjectile(node, Projectile::DarkAttack, -1.f, 0.f, textures);
    };

    mThunderStrikeCommand.category = Category::Scene;
    mThunderStrikeCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createThunderStrike(node, Projectile::ThunderStrike, 0.f, 0.f, textures);
        createThunderStrike(node, Projectile::ThunderStrike, 200.f, 200.f, textures);
        createThunderStrike(node, Projectile::ThunderStrike, -200.f, -200.f, textures);
        createThunderStrike(node, Projectile::ThunderStrike, 200.f, -200.f, textures);
        createThunderStrike(node, Projectile::ThunderStrike, -200.f, 200.f, textures);
    };

    mExplosionCommand.category = Category::Scene;
    mExplosionCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createExplosion(node, 0.f, 0.f, textures);
        createExplosion(node, 200.f, 0.f, textures);
        createExplosion(node, -200.f, 0.f, textures);
        createExplosion(node, 0.f, 200.f, textures);
        createExplosion(node, 0.f, -200.f, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
	mHealthDisplay = healthDisplay.get();
    mHealthDisplay->setColor(sf::Color::Black);
	attachChild(std::move(healthDisplay));

    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);
    mSprite.setScale(-mSprite.getScale().x, mSprite.getScale().y);
}

void DarkWizzard::createExplosion(SceneNode& node, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(Projectile::ExplosionAttack, textures, std::get<0>(mProjectileAnimationMap.at(Projectile::ExplosionAttack)), std::get<1>(mProjectileAnimationMap.at(Projectile::ExplosionAttack)), std::get<2>(mProjectileAnimationMap.at(Projectile::ExplosionAttack))));

    projectile->setPosition(mTargetDirection.x + xOffset, mTargetDirection.y + yOffset);

    node.attachChild(std::move(projectile));
}

void DarkWizzard::createThunderStrike(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures, std::get<0>(mProjectileAnimationMap.at(type)), std::get<1>(mProjectileAnimationMap.at(type)), std::get<2>(mProjectileAnimationMap.at(type))));

    projectile->setPosition(mTargetDirection.x + xOffset, mTargetDirection.y + yOffset);

    node.attachChild(std::move(projectile));
}

void DarkWizzard::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures, std::get<0>(mProjectileAnimationMap.at(type)), std::get<1>(mProjectileAnimationMap.at(type)), std::get<2>(mProjectileAnimationMap.at(type))));
    sf::Vector2f offset(xOffset * mSprite.getScale().x, yOffset * mSprite.getScale().y);
    sf::Vector2f velocity(0.f, 0.f);
    float sign = (mSprite.getScale().x > 0) ? 1.f : -1.f;
    sf::Vector2f direction(mTargetDirection.x - getWorldPosition().x, mTargetDirection.y - getWorldPosition().y);
    float angle = std::atan2(direction.y, direction.x);
    angle += xOffset * 45.f * 3.14f / 180.f;

    velocity = projectile->getMaxSpeed() * sf::Vector2f(std::cos(angle), std::sin(angle));
    projectile->setPosition(getWorldPosition() + offset);
    projectile->setVelocity(velocity);
    projectile->setRotation(angle * 180.f / 3.14f);
    node.attachChild(std::move(projectile));
}

int DarkWizzard::numAnimation()
{
    return numRow;
}

unsigned int DarkWizzard::getCategory() const
{
    return Category::DarkWizzard;
}

void DarkWizzard::setTargetDirection(sf::Vector2f direction)
{
    mTargetDirection = direction;
}

void DarkWizzard::setAnimation(Animation animation)
{
    if (mCurrentAnimation == Death) return;
    if (mCurrentAnimation == animation) return;
    mCurrentAnimation = animation;
    curX = 0;
    for(size_t i=0; i<AnimationCount; i++)
    {
        if (i == animation) break;
        curX += std::get<1>(mAnimation[(Animation)i]);
    }
    numRow = 0;
    mAnimationTime = sf::Time::Zero;
}

void DarkWizzard::setTextureRect(sf::IntRect rect)
{
    mSprite.setTextureRect(rect);
}

sf::FloatRect DarkWizzard::getBoundingRect() const
{
    sf::FloatRect rect = getWorldTransform().transformRect(mSprite.getGlobalBounds());
    rect.width -= 300;
    rect.left += 150;
    rect.height -= 270;
    rect.top += 150;
    return rect;
}

void DarkWizzard::rebuildTable()
{

}

float DarkWizzard::getMaxSpeed() const
{
    return Table.speed;
}

void DarkWizzard::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void DarkWizzard::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
    if (isDestroyed())
    {
        setAnimation(Death);
        return;
    }
    if (getVelocity().x < 0) setScale(1.f, 1.f), mHealthDisplay->setScale(1.f, 1.f);
    if (getVelocity().x > 0) setScale(-1.f, 1.f), mHealthDisplay->setScale(-1.f, 1.f);
    mMovingTime += deltaTime;
    if (mCurrentAnimation!=TakedDamage) Entity::updateCurrent(deltaTime, commands);
    setVelocity(0.f, 0.f);
    checkProjectileLaunch(deltaTime, commands);
    fireAttack(commands);

    //Update health display
    updateTexts();
    doAnimation(deltaTime,commands);
}

void DarkWizzard::doAnimation(sf::Time deltaTime, CommandQueue& commands)
{
    int widthSprite = std::get<1>(mAnimation.at(mCurrentAnimation));
    int heightSprite = std::get<2>(mAnimation.at(mCurrentAnimation));
    int maxCol = std::get<0>(mAnimation.at(mCurrentAnimation));

    mAnimationTime += deltaTime;

    if (mAnimationTime >= sf::seconds(0.1f))
    {
        mAnimationTime = sf::Time::Zero;
        numRow++;
        if (mCurrentAnimation == Run){
            Entity::playLocalSound(commands, SoundEffect::Chasing);
        }

        if (numRow == 4 && mCurrentAnimation == Attack1){
            if (mIsFiring){
                commands.push(mDarkAttackCommand);
                Entity::playLocalSound(commands, SoundEffect::DarkAttack);
                attackLeft--;
            }
        }
        if (numRow == 4 && mCurrentAnimation == Attack2){
            if (mIsExplosion){
                commands.push(mExplosionCommand);
                Entity::playLocalSound(commands, SoundEffect::ExplosionAttack);
                attackLeft--;
            }
            if (mIsThunderStrike){
                commands.push(mThunderStrikeCommand);
                Entity::playLocalSound(commands, SoundEffect::ThunderStrike);
                attackLeft--;
            }
        }
        if (numRow == maxCol)
        {
            if (mCurrentAnimation == Death){
                mIsMarkedForRemoval = true;
                return;
            }
            else if (mCurrentAnimation == Run){
                attackLeft--;
            }
            numRow = 0;
        }
        mSprite.setTextureRect(sf::IntRect(curX, numRow * heightSprite, widthSprite, heightSprite));
    }
}

void DarkWizzard::fireAttack(CommandQueue& commands)
{
    if (mIsChasing && attackLeft){
        setAnimation(Run);
        sf::Vector2f direction(mTargetDirection.x - getWorldPosition().x, mTargetDirection.y - getWorldPosition().y);
        float angle = std::atan2(direction.y, direction.x);
        setVelocity(getMaxSpeed() * sf::Vector2f(std::cos(angle), std::sin(angle)));
    }
    else if (mIsFiring && attackLeft){
        setAnimation(Attack1);
    }
    else if (mIsExplosion && attackLeft){
        setAnimation(Attack2);
    }
    else if (mIsThunderStrike && attackLeft){
        setAnimation(Attack2);
    }
}

void DarkWizzard::checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands)
{
    if (mIsChasing && !attackLeft){
        mIsChasing = false;
        mIsThunderStrike = true;
        Entity::playLocalSound(commands, SoundEffect::ThunderCasting);
        attackLeft = 5;
    }
    else if (mIsThunderStrike && !attackLeft){
        mIsThunderStrike = false;
        mIsExplosion = true;
        Entity::playLocalSound(commands, SoundEffect::ExplosionCasting);
        attackLeft = 5;
    }
    else if (mIsExplosion && !attackLeft){
        mIsExplosion = false;
        mIsFiring = true;
        Entity::playLocalSound(commands, SoundEffect::DarkAttackCasting);
        attackLeft = 5;
    }
    else if (mIsFiring && !attackLeft){
        mIsFiring = false;
        mIsChasing = true;
        attackLeft = 5;
    }
}

void DarkWizzard::updateTexts()
{
    if (mCurrentAnimation!=Death) mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
    else mHealthDisplay->setString("0 HP");
	mHealthDisplay->setPosition(0.f, 100.f);
	mHealthDisplay->setRotation(-getRotation());
}