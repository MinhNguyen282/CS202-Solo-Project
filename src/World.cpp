#include "include/World.hpp"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 2800.f, 2800.f)
, mSpawnPosition(100, mWorldBounds.height / 4 - 200)
, mScrollSpeed(-100.f)
, mPlayerCharacter(nullptr)
{
    srand(time(NULL));
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

bool World::hasAlivePlayer()
{
    return !mPlayerCharacter->isMarkedForRemoval();
}

bool World::isBossDefeated()
{
    return !isBoss && hasBossSpawn;
}

void World::update(sf::Time deltaTime)
{
    mPlayerCharacter->setVelocity(0.f, 0.f);
    mIsInvicibleTime += deltaTime;

    destroyEntitiesOutsideView();
    if (isBoss && mBoss->getHitpoints() <= 0)
    {
        isBoss = false;
    }
    //guideMissiles();

    sf::Vector2f Target = sf::Vector2f(mPlayerCharacter->getWorldPosition().x + 16, mPlayerCharacter->getWorldPosition().y + 24);
    
    sf::Vector2i mousePosition = sf::Mouse::getPosition(mWindow);
    sf::Vector2f worldMousePosition = mWindow.mapPixelToCoords(mousePosition,mWorldView);
    mPlayerCharacter->getTarget(worldMousePosition);
    

    if (isBoss) mBoss->setTargetDirection(Target);
    for(int i=0; i<mActiveEnemies.size(); i++)
    {
        mActiveEnemies[i]->setTargetDirection(Target);
    }
    
    while (mCommandQueue.isEmpty() == false)
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), deltaTime);
    }

    adaptPlayerVelocity();

    handleCollisions();

    spawnEnemies();
    mSceneGraph.removeWrecks();

    mSceneGraph.update(deltaTime,mCommandQueue);
    adaptPlayerPosition();
}

void World::adaptPlayerVelocity()
{
    sf::Vector2f velocity = mPlayerCharacter->getVelocity();
    if (velocity.x != 0.f && velocity.y != 0.f)
    {
        mPlayerCharacter->setVelocity(velocity / std::sqrt(2.f));
    }
}

void World::adaptPlayerPosition()
{
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 5.f;

    sf::Vector2f position = mPlayerCharacter->getPosition();

    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance - mPlayerCharacter->getBoundingRect().width);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance - mPlayerCharacter->getBoundingRect().height);

    if (position.x > viewBounds.left + viewBounds.width / 2.f - borderDistance)
    {
        mWorldView.move(position.x - viewBounds.left - viewBounds.width / 2.f + borderDistance, 0.f);
    }
    else if (position.x < viewBounds.left + viewBounds.width / 2.f - borderDistance)
    {
        mWorldView.move(position.x - viewBounds.left - viewBounds.width / 2.f + borderDistance, 0.f);
    }
    if (position.y > viewBounds.top + viewBounds.height / 2.f - borderDistance)
    {
        mWorldView.move(0.f, position.y - viewBounds.top - viewBounds.height / 2.f + borderDistance);
    }
    else if (position.y < viewBounds.top + viewBounds.height / 2.f - borderDistance)
    {
        mWorldView.move(0.f, position.y - viewBounds.top - viewBounds.height / 2.f + borderDistance);
    }
    
    if ((mWorldView.getCenter() - mWorldView.getSize() / 2.f).x < 0) 
    {
        mWorldView.setCenter(mWorldView.getSize().x / 2.f, mWorldView.getCenter().y);
    }

    if ((mWorldView.getCenter() + mWorldView.getSize() / 2.f).y < 0) 
    {
        mWorldView.setCenter(mWorldView.getCenter().x, -mWorldView.getSize().y / 2.f);
    }

    if ((mWorldView.getCenter() + mWorldView.getSize() / 2.f).x > mWorldBounds.width) 
    {
        mWorldView.setCenter(mWorldBounds.width - mWorldView.getSize().x / 2.f, mWorldView.getCenter().y);
    }

    if ((mWorldView.getCenter() + mWorldView.getSize() / 2.f).y > mWorldBounds.height) 
    {
        mWorldView.setCenter(mWorldView.getCenter().x, mWorldBounds.height - mWorldView.getSize().y / 2.f);
    }

    mPlayerCharacter->setPosition(position);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
    mTextures.load(Textures::BlueWitch, "Media/Textures/bluewitchsheet.png");
    mTextures.load(Textures::Desert, "Media/Textures/Desert.png");
    mTextures.load(Textures::AlliedBullet, "Media/Textures/AlliedBullet.png");
    mTextures.load(Textures::MechaBoss, "Media/Textures/bosssheet.png");
    mTextures.load(Textures::MechaBossRangedAttack, "Media/Textures/bossprojectilesheet.png");
    mTextures.load(Textures::FlyingEye, "Media/Textures/FlyingEye/enemysheet.png");
    mFonts.load(Fonts::Main, "Media/buddychampion.ttf");
}

void World::spawnEnemies()
{
    while (!mBossSpawnPoints.empty() && mBossSpawnPoints.back().x < getBattlefieldBounds().left + getBattlefieldBounds().width - 300)
    {
        SpawnPoint spawn = mBossSpawnPoints.back();
        std::unique_ptr<MechaBoss> enemy(new MechaBoss(mTextures,mFonts));
        mBoss = enemy.get();
        mBoss->setPosition(spawn.x,spawn.y);
        hasBossSpawn = true;
        isBoss = true;
        mSceneLayers[Ground]->attachChild(std::move(enemy));
        mBossSpawnPoints.pop_back();
    }

    for(auto &it : mActiveEnemies)
    {
        if (it->isMarkedForRemoval())
        {
            it->destroy();
            mActiveEnemies.erase(std::remove(mActiveEnemies.begin(), mActiveEnemies.end(), it), mActiveEnemies.end());
        }
    }

    while (mActiveEnemies.size() < 5){
        int randX = (rand() % 2 == 0 ? -1 : 1);
        int randY = (rand() % 2 == 0 ? -1 : 1);
        int randomX = mPlayerCharacter->getWorldPosition().x + rand() % 600 + randX * 1000;
        int randomY = mPlayerCharacter->getWorldPosition().y + rand() % 320 + randY * 500;
        int randomEnemy = rand() % 1;
        std::unique_ptr<Enemy> enemy(new Enemy(Enemy::FlyingEye, mTextures, mFonts));
        enemy->setPosition(randomX, randomY);
        mActiveEnemies.push_back(enemy.get());
        mSceneLayers[Ground]->attachChild(std::move(enemy));
    }
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = Category::Projectile;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
    {
        if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
        {
            e.destroy();
        }
    });

    mCommandQueue.push(command);
}

void World::guideMissiles()
{

}

void World::buildScene()
{
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        Category::Type category = (i == Ground) ? Category::Scene : Category::None;
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& texture = mTextures.get(Textures::Desert);
    texture.setRepeated(true);

    float viewHeight = mWorldView.getSize().y;
    sf::IntRect textureRect(mWorldBounds);
    textureRect.height += static_cast<int>(viewHeight);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));


    std::unique_ptr<Witch> player(new Witch(Witch::BlueWitch, mTextures, mFonts));
    mPlayerCharacter = player.get();
    sf::FloatRect bounds = mPlayerCharacter->getBoundingRect();
    mPlayerCharacter->setPosition(mSpawnPosition);
    mSceneLayers[Ground]->attachChild(std::move(player));

    //addBosses();
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getViewBounds();
    bounds.top -= 100;
    bounds.height += 100;
    return bounds;
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();
    if (type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
    for (SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::MechaBoss, Category::AlliedProjectile))
        {
            auto& enemy = static_cast<MechaBoss&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);
            if (enemy.getHitpoints() > 1) 
            {
                enemy.damage(projectile.getDamage());
                if (enemy.getHitpoints() <= 0){
                    enemy.heal(1 - enemy.getHitpoints());
                    enemy.setAnimation(MechaBoss::Die);
                }
            }
            projectile.destroy();
        }
        else if (matchesCategories(pair, Category::Player, Category::EnemiesProjectile))
        {
            auto& player = static_cast<Witch&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);
            player.damage(projectile.getDamage());
            projectile.destroy();
        }
        else if (matchesCategories(pair, Category::Player,Category::Enemy) && mIsInvicibleTime >= sf::seconds(1.0))
        {
            auto& player = static_cast<Witch&>(*pair.first);
            auto& enemy = static_cast<Enemy&>(*pair.second);
            player.damage(enemy.getBodyDamage());
            player.setAnimation(Witch::TakedDamage);
            enemy.setAnimation(Enemy::Attack1);
            mIsInvicibleTime = sf::Time::Zero;
        }
        else if (matchesCategories(pair, Category::Enemy, Category::AlliedProjectile))
        {
            auto& enemy = static_cast<Enemy&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);
            if (enemy.getHitpoints() > 1) 
            {
                enemy.damage(projectile.getDamage());
                if (enemy.getHitpoints() <= 0){
                    enemy.heal(1 - enemy.getHitpoints());
                    enemy.setAnimation(Enemy::Death);
                }
            }
            projectile.destroy();
        }
    }
}

void World::addBosses()
{
    addBoss(400,0);
    
    std::sort(mBossSpawnPoints.begin(), mBossSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
    {
        return lhs.x < rhs.x;
    });
}

void World::addBoss(float relX, float relY)
{
    SpawnPoint spawn(mSpawnPosition.x + relX, mSpawnPosition.y - relY);
    mBossSpawnPoints.push_back(spawn);
}