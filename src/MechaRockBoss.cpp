#include "include/MechaRockBoss.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/Utility.hpp"
#include "include/Projectile.hpp"

namespace
{
    const MechaBossData  Table = initializeMechaBossData();
}

MechaBoss::MechaBoss(const TextureHolder& textures, const FontHolder& fonts)
: Entity(1000)
, mSprite(textures.get(Textures::MechaBoss))
, mTravelDistance(0.f)
, mDirectionIndex(0)
, mFireCountdown(sf::Time::Zero)
, mIsMarkedForRemoval(false)
, mIsSkill(false)
, mIsFiring(false)
{
    mSprite.setTextureRect(sf::IntRect(0, 0, 346, 346));
    mAnimation[Idle] = std::make_tuple(4, 346, 346);
    mAnimation[TakedDamage] = std::make_tuple(8, 346, 346);
    mAnimation[RangedAttack] = std::make_tuple(9, 346, 346);
    mAnimation[SkillAttack] = std::make_tuple(7, 346, 346);
    mAnimation[Shield] = std::make_tuple(7, 346, 346);
    mAnimation[Die] = std::make_tuple(14, 346, 346);
    mCurrentAnimation = Idle;
    curX = 0;
    numRow = 0;

    mProjectileAnimationMap[Projectile::MechaBossRangedAttack] = std::make_tuple(6, 60, 25);

    mFireCommand.category = Category::Scene;
    mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
    {
        createBullets(node,textures);
    };

    mSkillCommand.category = Category::Scene;
    mSkillCommand.action = [this, &textures](SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::MechaBossSkillAttack, 0.f, 0.f, textures);
    };

    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts,""));
	mHealthDisplay = healthDisplay.get();
    mHealthDisplay->setColor(sf::Color::Black);
	attachChild(std::move(healthDisplay));

    mSprite.setOrigin(mSprite.getLocalBounds().width / 2.f, mSprite.getLocalBounds().height / 2.f);
    mSprite.setScale(-mSprite.getScale().x, mSprite.getScale().y);
}

int MechaBoss::numAnimation()
{
    return numRow;
}

unsigned int MechaBoss::getCategory() const
{
    return Category::MechaBoss;
}

void MechaBoss::setTargetDirection(sf::Vector2f direction)
{
    mTargetDirection = direction;
}

void MechaBoss::createBullets(SceneNode& node, const TextureHolder& textures) const
{
    Projectile::Type type = Projectile::MechaBossRangedAttack;
    createProjectile(node, type, 0.f, 0.f, textures);
}

void MechaBoss::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
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

void MechaBoss::setAnimation(Animation animation)
{
    if (animation!=Die && (mCurrentAnimation == RangedAttack || mCurrentAnimation == SkillAttack || mCurrentAnimation == Shield || mCurrentAnimation == TakedDamage || mCurrentAnimation == Die)) return;
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

void MechaBoss::setTextureRect(sf::IntRect rect)
{
    mSprite.setTextureRect(rect);
}

sf::FloatRect MechaBoss::getBoundingRect() const
{
    sf::FloatRect rect = getWorldTransform().transformRect(mSprite.getGlobalBounds());
    rect.width -= 260;
    rect.left += 130;
    rect.height -= 180;
    rect.top += 80;
    return rect;
}


float MechaBoss::getMaxSpeed() const
{
    return Table.speed;
}

void MechaBoss::skillAttack(const sf::Vector2f& target)
{
    mIsSkill = true;
}

void MechaBoss::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void MechaBoss::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
    if (isDestroyed())
    {
        mIsMarkedForRemoval = true;
        return;
    }
    checkProjectileLaunch(deltaTime, commands);
    updateMovementPattern(deltaTime);
    Entity::updateCurrent(deltaTime, commands);

    //Update health display
    updateTexts();


    int widthSprite = std::get<1>(mAnimation.at(mCurrentAnimation));
    int heightSprite = std::get<2>(mAnimation.at(mCurrentAnimation));
    int maxCol = std::get<0>(mAnimation.at(mCurrentAnimation));

    mAnimationTime += deltaTime;

    if (mAnimationTime >= sf::seconds(0.1f))
    {
        mAnimationTime = sf::Time::Zero;
        numRow++;
        if (numRow == maxCol)
        {
            numRow = 0;
            if (mCurrentAnimation == RangedAttack || mCurrentAnimation == SkillAttack || mCurrentAnimation == Shield || mCurrentAnimation == TakedDamage)
            {
                mCurrentAnimation = Idle;
                curX = 0;
                numRow = 0;
            }
            if (mCurrentAnimation == Die)
            {
                damage(100);
            }
        }
        mSprite.setTextureRect(sf::IntRect(curX, numRow * heightSprite, widthSprite, heightSprite));
    }

}

void MechaBoss::updateMovementPattern(sf::Time deltaTime)
{
    const std::vector<Direction>& directions = Table.directions;

    if (!directions.empty())
    {
        float distanceToTravel = directions[mDirectionIndex].distance;
        if (mTravelDistance > distanceToTravel)
        {
            mDirectionIndex = (mDirectionIndex + 1) % directions.size();
            mTravelDistance = 0.f;
        }

        float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
        float vx = getMaxSpeed() * std::cos(radians);
        float vy = getMaxSpeed() * std::sin(radians);

        setVelocity(vx, vy);
        mTravelDistance += getMaxSpeed() * deltaTime.asSeconds();
    }
}

void MechaBoss::fireAttack()
{
    mIsFiring = true;
    if (mFireCountdown <= sf::Time::Zero) setAnimation(RangedAttack);
}

void MechaBoss::checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands)
{
    if (mCurrentAnimation != Die) fireAttack();
    else mIsFiring = false;
    if (mIsFiring && mFireCountdown <= sf::Time::Zero && numRow == 7)
    {
        commands.push(mFireCommand);
        mFireCountdown += Table.fireInterval;
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero)
    {
        mFireCountdown -= deltaTime;
        mIsFiring = false;
    }
}

void MechaBoss::updateTexts()
{
    if (mCurrentAnimation!=Die) mHealthDisplay->setString(std::to_string(getHitpoints()) + " HP");
    else mHealthDisplay->setString("0 HP");
	mHealthDisplay->setPosition(0.f, 100.f);
	mHealthDisplay->setRotation(-getRotation());
}