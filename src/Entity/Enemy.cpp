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
    const std::vector<EnemyData> Table = initializeEnemyData();
}

//First 5 Min: FlyingEye and Goblin
//After 5 Min: FlyingEye, Goblin and Mushroom
//After 10 Min: FlyingEye, Goblin, Mushroom and Skeleton
//15 Min: FlyingEye, Goblin, Mushroom, Skeleton and Boss

Enemy::Enemy(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(toTextureIDEnemy(type)))
, mIsMarkedForRemoval(false)
, mTargetDirection()
, mHealthDisplay(nullptr)
, mAnimationTime(sf::Time::Zero)
, mFireCountdown(sf::Time::Zero)
, curX(0)
, numRow(0)
, mIsNearPlayer(false)
, mIsAttack(false)
{
    readEnemyData(type);
    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);

    mAttack3.category = Category::Scene;
    mAttack3.action = [this, &textures](SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::EnemyBullet, 0.f, 0.f, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
    mHealthDisplay = healthDisplay.get();
    mHealthDisplay->setColor(sf::Color::Black);
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


    projectile->setPosition(getWorldPosition() + sf::Vector2f(-100, -45));
    projectile->setVelocity(velocity);
    projectile->setRotation(toDegree(angle));
    node.attachChild(std::move(projectile));
}

void Enemy::readEnemyData(Type type)
{
    std::ifstream ifs("Media/Textures/"+dirEnemy(type)+"/info.txt");
    int width, height;
    ifs >> width >> height;
    mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
    int x, y, z;
    ifs >> x >> y >> z;
    std::cout << x << ' ' << y << ' ' << z << '\n';
    mAnimation[Attack1] = std::make_tuple(x,y,z);
    ifs >> x >> y >> z;
    mAnimation[Death] = std::make_tuple(x,y,z);
    ifs >> x >> y >> z;
    mAnimation[Move] = std::make_tuple(x,y,z);
    ifs >> x >> y >> z;
    mAnimation[TakedDamage] = std::make_tuple(x,y,z);
    ifs >> x >> y >> z;
    mAnimation[Attack2] = std::make_tuple(x,y,z);
    ifs >> x >> y >> z;
    mAnimation[Attack3] = std::make_tuple(x,y,z);
    curX = 0;
    numRow = 0;
    setAnimation(Move);
    ifs.close();
    ifs.open("Media/Textures/"+dirEnemy(type)+"/projectileinfo.txt");
    ifs >> width >> height >> x;
    mProjectileAnimationMap[Projectile::EnemyBullet] = std::make_tuple(x, width, height);
}

void Enemy::setAnimation(Animation animation)
{
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

sf::FloatRect Enemy::getBoundingRect() const
{
    sf::FloatRect rect = getWorldTransform().transformRect(mSprite.getGlobalBounds());
    rect.width -= 300;
    rect.left += 150;
    rect.height -= 300;
    rect.top += 150;
    return rect;
}

bool Enemy::isMarkedForRemoval() const
{
    return mIsMarkedForRemoval;
}

float Enemy::getMaxSpeed() const
{
    return Table[mType].speed;
}

void Enemy::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Enemy::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
    if (isDestroyed())
    {
        mIsMarkedForRemoval = true;
        return;
    }
    mFireCountdown += deltaTime;
    if (mCurrentAnimation==Move) Entity::updateCurrent(deltaTime, commands);
    doAnimation(deltaTime);
    updateMovementPattern(deltaTime);
    fireAttack(commands);
}

void Enemy::doAnimation(sf::Time deltaTime)
{
    int width = std::get<1>(mAnimation.at(mCurrentAnimation));
    int height = std::get<2>(mAnimation.at(mCurrentAnimation));
    int maxCol = std::get<0>(mAnimation.at(mCurrentAnimation));
    mAnimationTime += deltaTime;

    if (mAnimationTime >= sf::seconds(0.1f))
    {
        mAnimationTime = sf::Time::Zero;
        numRow++;
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
    if (mFireCountdown >= sf::seconds(5.0))
    {
        mFireCountdown = sf::Time::Zero;
        commands.push(mAttack3);
    }
}