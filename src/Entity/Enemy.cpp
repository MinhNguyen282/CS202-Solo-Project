#include "include/Enemy.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/Utility.hpp"
#include "include/Projectile.hpp"

Textures::ID toTextureIDEnemy(Enemy::Type type)
{
    switch (type)
    {
        case Enemy::FlyingEye:
            return Textures::FlyingEye;
        case Enemy::Goblin:
            return Textures::Goblin;
        case Enemy::Mushroom:
            return Textures::Mushroom;
        case Enemy::Skeleton:
            return Textures::Skeleton;
    }
    return Textures::FlyingEye;
}

Projectile::Type toProjectileType(Enemy::Type type)
{
    switch (type)
    {
    case Enemy::FlyingEye:
        return Projectile::FlyingEyeBullet;
        break;
    case Enemy::Goblin:
        return Projectile::GoblinBullet;
        break;
    case Enemy::Mushroom:
        return Projectile::MushroomBullet;
        break;
    case Enemy::Skeleton:
        return Projectile::SkeletonBullet;
        break;
    }
    return Projectile::FlyingEyeBullet;
}

std::string dirEnemy(Enemy::Type type)
{
    switch (type)
    {
    case Enemy::FlyingEye:
        return "FlyingEye";
        break;
    case Enemy::Goblin:
        return "Goblin";
        break;  
    case Enemy::Mushroom:
        return "Mushroom";
        break;
    case Enemy::Skeleton:
        return "Skeleton";
        break;
    }
    return "FlyingEye";
}

namespace
{
    std::vector<EnemyData> Table = initializeEnemyData();
}

//First 5 Min: FlyingEye and Goblin
//After 5 Min: FlyingEye, Goblin and Mushroom
//After 10 Min: FlyingEye, Goblin, Mushroom and Skeleton
//15 Min: FlyingEye, Goblin, Mushroom, Skeleton and Boss

Enemy::Enemy(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSpeedDiff(0.f)
, mSprite(textures.get(toTextureIDEnemy(type)))
, mTargetDirection()
, mHealthDisplay(nullptr)
, mAnimationTime(sf::Time::Zero)
, mFireCountdown(sf::Time::Zero)
, invicibleTime(sf::Time::Zero)
, curX(0)
, numRow(0)
, mIsKnockback(false)
, mIsNearPlayer(false)
, mIsAttack(false)
, mIsMarkedForRemoval(false)
, mCurrentAnimation(Attack1)
{
    readEnemyData(type);
    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);

    mAttack3.category = Category::Scene;
    mAttack3.action = [this, &textures](SceneNode& node, sf::Time)
    {
        createProjectile(node, toProjectileType(mType), 0.f, 0.f, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
    mHealthDisplay = healthDisplay.get();
    mHealthDisplay->setColor(sf::Color::White);
    attachChild(std::move(healthDisplay));
}

void Enemy::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures, std::get<0>(mProjectileAnimationMap.at(type)), std::get<1>(mProjectileAnimationMap.at(type)), std::get<2>(mProjectileAnimationMap.at(type))));
    sf::Vector2f offset(xOffset, yOffset);
    sf::Vector2f velocity(-projectile->getMaxSpeed(),0);

    sf::Vector2f initPos = getWorldPosition();

    float angle = std::atan2(mTargetDirection.y - initPos.y, mTargetDirection.x - initPos.x);

    velocity.x = projectile->getMaxSpeed() * std::cos(angle);
    velocity.y = projectile->getMaxSpeed() * std::sin(angle);


    projectile->setPosition(getWorldPosition() + sf::Vector2f(10, 10));
    projectile->setVelocity(velocity);
    projectile->setRotation(toDegree(angle));
    node.attachChild(std::move(projectile));
}

void Enemy::readEnemyData(Type type)
{
    int width = Table[type].width;
    int height = Table[type].height;
    mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
    mAnimation[Attack1] = Table[type].animations[0];
    mAnimation[Death] = Table[type].animations[1];
    mAnimation[Move] = Table[type].animations[2];
    mAnimation[TakedDamage] = Table[type].animations[3];
    mAnimation[Attack2] = Table[type].animations[4];
    mAnimation[Attack3] = Table[type].animations[5];
    expPoint = Table[type].expPoint;
    curX = 0;
    numRow = 0;
    setAnimation(Move);
    mProjectileAnimationMap[toProjectileType(type)] = Table[type].projectileAnimation;
}

void Enemy::rebuildTable()
{
    Table = initializeEnemyData();
    Entity::heal(std::max(0, Table[mType].hitpoints - getHitpoints()));
}

int Enemy::getExpPoint() const
{
    return expPoint;
}

void Enemy::setAnimation(Animation animation)
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

void Enemy::setTargetDirection(sf::Vector2f direction)
{
    mTargetDirection = direction;
}

int Enemy::getBodyDamage() const
{
    return Table[mType].bodyDamage;
}

int Enemy::getFireDamage() const
{
    return Table[mType].fireDamage;
}

unsigned int Enemy::getCategory() const
{
    return Category::Enemy;
}

int Enemy::getNumRow() const
{
    return numRow;
}

sf::FloatRect Enemy::getBoundingRect() const
{
    sf::FloatRect rect = getWorldTransform().transformRect(mSprite.getGlobalBounds());
    if (mType == FlyingEye)
    {
        rect.width -= 300;
        rect.left += 150;
        rect.height -= 300;
        rect.top += 150;
    }
    if (mType == Goblin)
    {
        rect.width -= 300;
        rect.left += 150;
        rect.height -= 275;
        rect.top += 150;
    }
    if (mType == Mushroom)
    {
        rect.width -= 300;
        rect.left += 150;
        rect.height -= 275;
        rect.top += 150;
    }
    if (mType == Skeleton)
    {
        rect.width -= 300;
        rect.left += 150;
        rect.height -= 250;
        rect.top += 125;
    }
    return rect;
}

bool Enemy::isMarkedForRemoval() const
{
    return isDestroyed();
}

bool Enemy::isDestroyed() const
{
    return getHitpoints() <= 0;
}

Enemy::Animation Enemy::getCurrentAnimation() const
{
    return mCurrentAnimation;
}

float Enemy::getMaxSpeed() const
{
    float speed = Table[mType].speed;
    if (mDebuffDuration > sf::Time::Zero) speed -= mSpeedDiff;
    return speed;
}

void Enemy::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Enemy::knockback(sf::Vector2f direction, float distance)
{
    setAnimation(TakedDamage);
    move(direction * distance);
}

void Enemy::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
    if (getVelocity().x < 0) setScale(-1.f, 1.f), mHealthDisplay->setScale(-1.f, 1.f);
    if (getVelocity().x > 0) setScale(1.f, 1.f), mHealthDisplay->setScale(1.f, 1.f);

    if (mDebuffDuration > sf::Time::Zero){
        mSprite.setColor(sf::Color::Blue);
    }
    else mSprite.setColor(sf::Color::White);

    invicibleTime += deltaTime;
    mFireCountdown += deltaTime;
    mDebuffDuration -= deltaTime;

    if (mCurrentAnimation==Move || mCurrentAnimation==TakedDamage) Entity::updateCurrent(deltaTime, commands);
    updateTexts();
    doAnimation(deltaTime,commands);
    updateMovementPattern(deltaTime);
    fireAttack(commands);
}

void Enemy::doAnimation(sf::Time deltaTime, CommandQueue& commands)
{
    int width = std::get<1>(mAnimation.at(mCurrentAnimation));
    int height = std::get<2>(mAnimation.at(mCurrentAnimation));
    int maxCol = std::get<0>(mAnimation.at(mCurrentAnimation));
    mAnimationTime += deltaTime;

    if (mAnimationTime >= sf::seconds(0.1f))
    {
        mAnimationTime = sf::Time::Zero;
        numRow++;
        if (numRow==4 && mCurrentAnimation==Attack3 && dirEnemy(mType)=="FlyingEye")
        {
            commands.push(mAttack3);
            float distance = std::sqrt((getWorldPosition().x - mTargetDirection.x)*(getWorldPosition().x - mTargetDirection.x) + (getWorldPosition().y - mTargetDirection.y)*(getWorldPosition().y - mTargetDirection.y));
            if (distance <= 600) Entity::playLocalSound(commands, SoundEffect::FlyingEyeFiring);
        }
        if (numRow==10 && mCurrentAnimation==Attack3 && dirEnemy(mType)=="Goblin")
        {
            commands.push(mAttack3);
            float distance = std::sqrt((getWorldPosition().x - mTargetDirection.x)*(getWorldPosition().x - mTargetDirection.x) + (getWorldPosition().y - mTargetDirection.y)*(getWorldPosition().y - mTargetDirection.y));
            if (distance <= 600) Entity::playLocalSound(commands, SoundEffect::GoblinFiring);
        }
        if (numRow==8 && mCurrentAnimation==Attack3 && dirEnemy(mType)=="Mushroom")
        {
            commands.push(mAttack3);
            float distance = std::sqrt((getWorldPosition().x - mTargetDirection.x)*(getWorldPosition().x - mTargetDirection.x) + (getWorldPosition().y - mTargetDirection.y)*(getWorldPosition().y - mTargetDirection.y));
            if (distance <= 600) Entity::playLocalSound(commands, SoundEffect::MushroomFiring);
        }
        if (numRow==4 && mCurrentAnimation==Attack3 && dirEnemy(mType)=="Skeleton")
        {
            commands.push(mAttack3);
            float distance = std::sqrt((getWorldPosition().x - mTargetDirection.x)*(getWorldPosition().x - mTargetDirection.x) + (getWorldPosition().y - mTargetDirection.y)*(getWorldPosition().y - mTargetDirection.y));
            if (distance <= 600) Entity::playLocalSound(commands, SoundEffect::SkeletonFiring);
        }
        if (numRow == maxCol)
        {
            if (mCurrentAnimation==Attack1 || mCurrentAnimation==Attack2 || mCurrentAnimation==Attack3)
            {
                setAnimation(Move);
            }
            if (mCurrentAnimation == Death)
            {
                mIsMarkedForRemoval = true;
                return;
            }
            if (mCurrentAnimation == TakedDamage)
            {
                setAnimation(Move);
            }
            numRow = 0;
        }
        mSprite.setTextureRect(sf::IntRect(curX, numRow * height, width, height));
    }
}

void Enemy::updateMovementPattern(sf::Time deltaTime)
{
    sf::Vector2f currentDir = getWorldPosition();
    float angle = std::atan2(mTargetDirection.y - currentDir.y, mTargetDirection.x - currentDir.x);
    float maxSpeed = getMaxSpeed();
    setVelocity(maxSpeed * std::cos(angle), maxSpeed * std::sin(angle));
}

void Enemy::fireAttack(CommandQueue& commands)
{
    float timeFire = (rand() % 300)/100.0 + 5;
    if (mFireCountdown >= sf::seconds(timeFire))
    {
        mFireCountdown = sf::Time::Zero;
        setAnimation(Attack3);
    }
    sf::Vector2f currentDir = getWorldPosition();
    sf::Vector2f playerDir = mTargetDirection;
    float distance = std::sqrt((currentDir.x - playerDir.x)*(currentDir.x - playerDir.x) + (currentDir.y - playerDir.y)*(currentDir.y - playerDir.y));
    if (distance <= 30){
        srand(time(NULL));
        int rant = rand() % 2;
        if (rant == 0) setAnimation(Attack1);
        else setAnimation(Attack2);
    }
}

void Enemy::updateTexts()
{
    mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
    mHealthDisplay->setPosition(0.f, 50.f);
    mHealthDisplay->setRotation(-getRotation());
}

void Enemy::damage(int points)
{
    if (invicibleTime >= sf::seconds(0.1f)){
        invicibleTime = sf::Time::Zero;
        Entity::damage(points);
    }
}

void Enemy::debuff(float speedDiff, sf::Time duration)
{
    mSpeedDiff = speedDiff;
    mDebuffDuration = duration;
}