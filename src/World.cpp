#include "include/World.hpp"
#include "include/ParticleNode.hpp"

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts)
: mTarget(outputTarget)
, mWorldView(outputTarget.getDefaultView())
, mTextures()
, mFonts(fonts)
, mSceneTexture()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 2800.f, 2800.f)
, mSpawnPosition(100, mWorldBounds.height / 4 - 200)
, mScrollSpeed(-100.f)
, mPlayerCharacter(nullptr)
, numEnemy(10)
{
    srand(time(NULL));
    mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

    loadTextures();
    buildScene();
    invicible = true;

    sf::Vector2f viewSize = mWorldView.getSize();

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

void World::setMousePosition(sf::Vector2i mousePosition)
{
    mMousePosition = mTarget.mapPixelToCoords(mousePosition,mWorldView);
}

void World::update(sf::Time deltaTime)
{
    mPlayerCharacter->setVelocity(0.f, 0.f);
    mIsInvicibleTime += deltaTime;
    mSpawnTime += deltaTime;
    mPlayedTime += deltaTime;
    if (mPlayedTime >= sf::seconds(60.0 * 5)){
        numEnemy = 15;
    }
    if (mPlayedTime >= sf::seconds(60.0 * 10)){
        numEnemy = 20;
    }
    if (mPlayedTime >= sf::seconds(60.0 * 15)){
        numEnemy = 25;
    }

    destroyEntitiesOutsideView();
    if (isBoss && mBoss->getHitpoints() <= 0)
    {
        isBoss = false;
    }
    //guideMissiles();

    sf::Vector2f Target = sf::Vector2f(mPlayerCharacter->getWorldPosition().x + 16, mPlayerCharacter->getWorldPosition().y + 24);
    
    mPlayerCharacter->getTarget(mMousePosition);
    

    if (isBoss) mBoss->setTargetDirection(Target);
    for(int i=0; i<mActiveEnemies.size(); i++)
    {
        if (mActiveEnemies[i] == nullptr) continue;
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
    adaptGUI();
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

void World::adaptGUI()
{
    mExpBar->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 10, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 15);
    mExpBarFrame->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 10, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 15);
    mSkillEIcon->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 15, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillEBlur->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 15, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillEBlur->setScale(mPlayerCharacter->getCoolDownE(),1);
    mSkillQIcon->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 50, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillQBlur->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 50, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillQBlur->setScale(mPlayerCharacter->getCoolDownQ(),1);

    mPlayedTimeText->setString("Time: " + std::to_string((int)(mPlayedTime.asSeconds()/60)) + " : " + std::to_string((int)mPlayedTime.asSeconds()%60));
    mPlayedTimeText->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 150, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 10);
    mLevelText->setString("Level: " + std::to_string(mPlayerCharacter->getLevel()));
    mLevelText->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 150, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 40);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::draw()
{
	if (PostEffect::isSupported())
	{
		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);
	}
	else
	{
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
	}
}

void World::loadTextures()
{
    mTextures.load(Textures::BlueWitch, "Media/Textures/bluewitchsheet.png");
    mTextures.load(Textures::Desert, "Media/Textures/Desert.png");
    mTextures.load(Textures::Particle, "Media/Textures/Particle.png");
    
    mTextures.load(Textures::AlliedBullet, "Media/Textures/AlliedBullet.png");
    mTextures.load(Textures::AlliedSkillE, "Media/Textures/AlliedSkillE.png");
    mTextures.load(Textures::AlliedSkillQ, "Media/Textures/AlliedSkillQ.png");


    mTextures.load(Textures::MechaBoss, "Media/Textures/bosssheet.png");
    mTextures.load(Textures::MechaBossRangedAttack, "Media/Textures/bossprojectilesheet.png");

    //Enemy texture
    mTextures.load(Textures::FlyingEye, "Media/Textures/FlyingEye/enemysheet.png");
    mTextures.load(Textures::FlyingEyeBullet, "Media/Textures/FlyingEye/projectilesheet.png");
    mTextures.load(Textures::Goblin, "Media/Textures/Goblin/enemysheet.png");
    mTextures.load(Textures::GoblinBullet, "Media/Textures/Goblin/projectilesheet.png");
    mTextures.load(Textures::Mushroom, "Media/Textures/Mushroom/enemysheet.png");
    mTextures.load(Textures::MushroomBullet, "Media/Textures/Mushroom/projectilesheet.png");
    mTextures.load(Textures::Skeleton, "Media/Textures/Skeleton/enemysheet.png");
    mTextures.load(Textures::SkeletonBullet, "Media/Textures/Skeleton/projectilesheet.png");

    //GUI
    mTextures.load(Textures::ExpBar, "Media/Textures/ExpBar.png");
    mTextures.load(Textures::ExpBarFrame, "Media/Textures/ExpBarFrame.png");
    mTextures.load(Textures::SkillEIcon, "Media/Textures/SkillEIcon.png");
    mTextures.load(Textures::SkillQIcon, "Media/Textures/SkillQIcon.png");
    mTextures.load(Textures::SkillBorder, "Media/Textures/SkillBorder.png");
    mTextures.load(Textures::IconBlur, "Media/Textures/IconBlur.png");
    //mFonts.load(Fonts::Main, "Media/buddychampion.ttf");
}

void World::spawnEnemies()
{
    if (mActiveEnemies.size()){
        auto wreckfieldBegin = std::remove_if(mActiveEnemies.begin(), mActiveEnemies.end(), std::mem_fn(&Enemy::isMarkedForRemoval));
        mActiveEnemies.erase(wreckfieldBegin, mActiveEnemies.end());
    }

    if (mActiveEnemies.size() < numEnemy && mSpawnTime >= sf::seconds(0.5)){
        mSpawnTime = sf::Time::Zero;
        int randX = (rand() % 2 == 0 ? -1 : 1);
        int randY = (rand() % 2 == 0 ? -1 : 1);
        int randomX = mPlayerCharacter->getWorldPosition().x + randX * (rand() %300 + 600);
        int randomY = mPlayerCharacter->getWorldPosition().y + randY * (rand() %100 + 400);
        int randomEnemy = rand() % 4;
        std::unique_ptr<Enemy> enemy(new Enemy((Enemy::Type)(randomEnemy), mTextures, mFonts));
        enemy->setPosition(randomX, randomY);
        mActiveEnemies.push_back(enemy.get());
        if (enemy->isMarkedForRemoval() == false) std::cout << "False\n";
        else std::cout << "True\n";
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

    std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant,mTextures));
    mSceneLayers[LowerGround]->attachChild(std::move(propellantNode));

    std::unique_ptr<Witch> player(new Witch(Witch::BlueWitch, mTextures, mFonts));
    mPlayerCharacter = player.get();
    sf::FloatRect bounds = mPlayerCharacter->getBoundingRect();
    mPlayerCharacter->setPosition(mSpawnPosition);
    mSceneLayers[Ground]->attachChild(std::move(player));

    std::unique_ptr<SpriteNode> expBar(new SpriteNode(mTextures.get(Textures::ExpBar)));
    mExpBar = expBar.get();
    mSceneLayers[GUILayer]->attachChild(std::move(expBar));

    std::unique_ptr<SpriteNode> expBarFrame(new SpriteNode(mTextures.get(Textures::ExpBarFrame)));
    mExpBarFrame = expBarFrame.get();
    mExpBarFrame->setScale(0,1);
    mSceneLayers[GUILayer]->attachChild(std::move(expBarFrame));

    std::unique_ptr<SpriteNode> skillEIcon(new SpriteNode(mTextures.get(Textures::SkillEIcon)));
    mSkillEIcon = skillEIcon.get();
    mSceneLayers[GUILayer]->attachChild(std::move(skillEIcon));

    std::unique_ptr<SpriteNode> skillEBlur(new SpriteNode(mTextures.get(Textures::IconBlur)));
    mSkillEBlur = skillEBlur.get();
    mSkillEBlur->setScale(0,1);
    mSceneLayers[GUILayer]->attachChild(std::move(skillEBlur));

    std::unique_ptr<SpriteNode> skillQIcon(new SpriteNode(mTextures.get(Textures::SkillQIcon)));
    mSkillQIcon = skillQIcon.get();
    mSceneLayers[GUILayer]->attachChild(std::move(skillQIcon));

    std::unique_ptr<SpriteNode> skillQBlur(new SpriteNode(mTextures.get(Textures::IconBlur)));
    mSkillQBlur = skillQBlur.get();
    mSkillQBlur->setScale(0,1);
    mSceneLayers[GUILayer]->attachChild(std::move(skillQBlur));

    std::unique_ptr<TextNode> playedTimeText(new TextNode(mFonts,""));
    mPlayedTimeText = playedTimeText.get();
    mPlayedTimeText->setColor(sf::Color::White);
    mPlayedTimeText->setCharacterSize(25);
    mSceneLayers[GUILayer]->attachChild(std::move(playedTimeText));

    std::unique_ptr<TextNode> levelDisplay(new TextNode(mFonts,""));
    mLevelText = levelDisplay.get();
    mLevelText->setColor(sf::Color::White);
    mLevelText->setCharacterSize(25);
    mSceneLayers[GUILayer]->attachChild(std::move(levelDisplay));
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
            if (pair.second && pair.first){
                std::cout << "Boss hit by projectile" << std::endl;
                auto& enemy = static_cast<MechaBoss&>(*pair.first);
                auto& projectile = static_cast<Projectile&>(*pair.second);
                std::cout << "After 1 cast" << std::endl;
                if (!projectile.isDestroyed() && !enemy.isDestroyed()){
                    enemy.setAnimation(MechaBoss::TakedDamage);
                    enemy.damage(projectile.getDamage());
                    projectile.destroy();
                }
            }
        }
        else if (matchesCategories(pair, Category::Player, Category::EnemiesProjectile))
        {
            if (pair.second && pair.first && !invicible){
                std::cout << "Player hit by projectile" << std::endl;
                auto& player = static_cast<Witch&>(*pair.first);
                auto& projectile = static_cast<Projectile&>(*pair.second);
                std::cout << "After 2 cast" << std::endl;
                if (!projectile.isDestroyed() && !player.isDestroyed()){
                    player.setAnimation(Witch::TakedDamage);
                    player.damage(projectile.getDamage());
                    projectile.destroy();
                }
            }
        }
        else if (matchesCategories(pair, Category::Player,Category::Enemy) && mIsInvicibleTime >= sf::seconds(2.0))
        {
            if (pair.second && pair.first && !invicible) {
                std::cout << "Player hit by enemy" << std::endl;
                auto& player = static_cast<Witch&>(*pair.first);
                auto& enemy = static_cast<Enemy&>(*pair.second);
                std::cout << "After 3 cast" << std::endl;
                if (!player.isDestroyed() && !enemy.isDestroyed() && enemy.getCurrentAnimation()==Enemy::Attack1 && enemy.getNumRow()==6){
                    player.damage(enemy.getBodyDamage());
                    player.setAnimation(Witch::TakedDamage);
                    mIsInvicibleTime = sf::Time::Zero;
                }
            }
        }
        else if (matchesCategories(pair, Category::Enemy, Category::AlliedProjectile))
        {
            if (pair.second && pair.first){
                std::cout << "Enemy hit by projectile" << std::endl;
                auto& enemy = static_cast<Enemy&>(*pair.first);
                auto& projectile = static_cast<Projectile&>(*pair.second);
                std::cout << "After 4 cast" << std::endl;
                if (!projectile.isDestroyed() && !enemy.isDestroyed()){
	                sf::Vector2f initPos = enemy.getWorldPosition();
	                sf::Vector2f worldPos = projectile.getWorldPosition();
                    float angle = std::atan2(initPos.y - worldPos.y, initPos.x - worldPos.x);
                    sf::Vector2f velocity;
                    velocity.x = std::cos(angle);
                    velocity.y = std::sin(angle);
                    enemy.knockback(velocity, 20);
                    enemy.damage(projectile.getDamage());
                    if (enemy.getHitpoints() <= 0){ 
                        mPlayerCharacter->receiveExp(enemy.getExpPoint());
                        float percent = mPlayerCharacter->getExpRatio();
                        mExpBarFrame->setScale(percent,1);
                    }
                    if (projectile.getType() == Projectile::AlliedBullet) projectile.destroy();
                }
                std::cout << "After 4-4 cast" << std::endl;
            }
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