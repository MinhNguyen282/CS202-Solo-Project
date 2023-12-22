#include "include/World.hpp"
#include "include/ParticleNode.hpp"
#include "include/SoundNode.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)
: mTarget(outputTarget)
, mWorldView(outputTarget.getDefaultView())
, mTextures()
, mBoss(nullptr)
, mFonts(fonts)
, mSounds(sounds)
, mSceneTexture()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 2800.f, 2800.f)
, mSpawnPosition(100, mWorldBounds.height / 4 - 200)
, mScrollSpeed(-100.f)
, mPlayerCharacter(nullptr)
, numEnemy(5)
, isBoss(false)
, hasBossSpawn(false)
, mScore(0)
, invicible(false)
{
    srand(time(NULL));
    mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

    Command rebuildTable;
    rebuildTable.category = Category::Entity | Category::Projectile;
    rebuildTable.action = derivedAction<Entity>([this](Entity& e, sf::Time)
    {
        e.rebuildTable();
    });
    mCommandQueue.push(rebuildTable);

    loadTextures();
    buildScene();

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

    if (mBoss && mBoss->getHitpoints() <= 0){
        isBoss = false;
    }

    if (mPlayedTime >= sf::seconds(60.0 * 15)){
        numEnemy = 0;
        if (!hasBossSpawn) addBosses();
    }
    else if (mPlayedTime >= sf::seconds(60.0 * 10)){
        numEnemy = 20;
    }
    else if (mPlayedTime >= sf::seconds(60.0 * 5)){
        numEnemy = 15;
    }
    else if (mPlayedTime >= sf::seconds(60.0 * 1)){
        numEnemy = 10;
    }

    destroyEntitiesOutsideView();
    guideMissiles();
    guideTarget();
    spawnEnemies();

    mPlayerCharacter->getTarget(mMousePosition);
    
    while (mCommandQueue.isEmpty() == false)
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), deltaTime);
    }

    adaptPlayerVelocity();

    handleCollisions();
    mSceneGraph.removeWrecks();

    mSceneGraph.update(deltaTime,mCommandQueue);
    adaptPlayerPosition();
    adaptGUI();

    updateSounds();
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
    float percent = mPlayerCharacter->getHealthRatio();
    mHealthBarFrame->setScale(percent,1);

    mHealthBar->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 10, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 50);
    mHealthBarFrame->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 28, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 55);
    mExpBar->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 10, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 15);
    mExpBarFrame->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 10, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 15);
    mSkillEIcon->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 15, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillEBlur->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 15, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillEBlur->setScale(mPlayerCharacter->getCoolDownE(),1);
    mSkillQIcon->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 50, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillQBlur->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 50, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mSkillQBlur->setScale(mPlayerCharacter->getCoolDownQ(),1);
    mUltimateIcon->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 85, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mUltimateBlur->setPosition(mWorldView.getCenter().x - mWorldView.getSize().x / 2.f + 85, mWorldView.getCenter().y + mWorldView.getSize().y / 2.f - 50);
    mUltimateBlur->setScale(mPlayerCharacter->getCoolDownUltimate(),1);

    mPlayedTimeText->setString("Time: " + std::to_string((int)(mPlayedTime.asSeconds()/60)) + " : " + std::to_string((int)mPlayedTime.asSeconds()%60));
    mPlayedTimeText->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 150, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 10);
    mLevelText->setString("Level: " + std::to_string(mPlayerCharacter->getLevel()));
    mLevelText->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 150, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 40);
    mScoreText->setString("Score: " + std::to_string(mScore));
    mScoreText->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 150, mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 70);
}

void World::updateSounds()
{
    mSounds.setListenerPosition(mPlayerCharacter->getWorldPosition());
    mSounds.removeStoppedSounds();
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
    mTextures.load(Textures::AlliedUltimate, "Media/Textures/AlliedUltimate.png");


    mTextures.load(Textures::DarkWizzard, "Media/Textures/bosssheet.png");
    mTextures.load(Textures::DarkAttack, "Media/Textures/DarkAttack.png");
    mTextures.load(Textures::ExplosionAttack, "Media/Textures/ExplosionAttack.png");
    mTextures.load(Textures::ThunderStrike, "Media/Textures/ThunderStrike.png");

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
    mTextures.load(Textures::HealthBar, "Media/Textures/HealthBar.png");
    mTextures.load(Textures::HealthBarFrame, "Media/Textures/HealthBarFrame.png");
    mTextures.load(Textures::ExpBar, "Media/Textures/ExpBar.png");
    mTextures.load(Textures::ExpBarFrame, "Media/Textures/ExpBarFrame.png");
    mTextures.load(Textures::SkillEIcon, "Media/Textures/SkillEIcon.png");
    mTextures.load(Textures::SkillQIcon, "Media/Textures/SkillQIcon.png");
    mTextures.load(Textures::UltimateIcon, "Media/Textures/UltimateIcon.png");
    mTextures.load(Textures::SkillBorder, "Media/Textures/SkillBorder.png");
    mTextures.load(Textures::IconBlur, "Media/Textures/IconBlur.png");
    //mFonts.load(Fonts::Main, "Media/buddychampion.ttf");
}

void World::spawnEnemies()
{
    Command enemyCollector;
    enemyCollector.category = Category::Enemy;
    enemyCollector.action = derivedAction<Enemy>([this](Enemy& enemy, sf::Time)
    {
        if (!enemy.isDestroyed()) mActiveEnemies.push_back(&enemy);
    });
    mSceneGraph.onCommand(enemyCollector, sf::Time::Zero);
    if (mActiveEnemies.size() < numEnemy && mSpawnTime >= sf::seconds(0.5)){
        mSpawnTime = sf::Time::Zero;
        int randX = (rand() % 2 == 0 ? -1 : 1);
        int randY = (rand() % 2 == 0 ? -1 : 1);
        int randomX = mPlayerCharacter->getWorldPosition().x + randX * (rand() %300 + 600);
        int randomY = mPlayerCharacter->getWorldPosition().y + randY * (rand() %100 + 400);
        int randomEnemy = rand() % 4;
        std::unique_ptr<Enemy> enemy(new Enemy((Enemy::Type)(randomEnemy), mTextures, mFonts));
        enemy->setPosition(randomX, randomY);
        mSceneLayers[Ground]->attachChild(std::move(enemy));
    }
    mActiveEnemies.clear();
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = Category::Projectile;
    command.action = derivedAction<Projectile>([this] (Projectile& e, sf::Time)
    {
        if (e.isGuided()) return;
        if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
        {
            e.destroy();
        }
    });

    mCommandQueue.push(command);
}

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::Enemy;
    enemyCollector.action = derivedAction<Enemy>([this](Enemy& enemy, sf::Time)
    {
        if (!enemy.isDestroyed()) mActiveEnemies.push_back(&enemy);
    });

    Command ultimateGuide;
    ultimateGuide.category = Category::AlliedProjectile;
    ultimateGuide.action = derivedAction<Projectile>([this](Projectile &proj, sf::Time){
        if (!proj.isGuided()) return;
        float minDistance = std::numeric_limits<float>::max();
        DarkWizzard* closetBoss = mBoss;
        if (mBoss) minDistance = distance(proj, *closetBoss);
        Enemy* closestEnemy = nullptr;
        for (Enemy* enemy : mActiveEnemies)
        {
            float enemyDistance = distance(proj, *enemy);
            if (enemyDistance < minDistance)
            {
                closestEnemy = enemy;
                minDistance = enemyDistance;
            }
        }
        if (closestEnemy)
        {
            sf::Vector2f velocity = unitVector(closestEnemy->getWorldPosition() - proj.getWorldPosition());
            proj.setVelocity(velocity * proj.getMaxSpeed());
        }
        else{
            sf::Vector2f velocity = unitVector(closetBoss->getWorldPosition() - proj.getWorldPosition());
            proj.setVelocity(velocity * proj.getMaxSpeed());
        }
    });
    mCommandQueue.push(enemyCollector);
    mCommandQueue.push(ultimateGuide);
    mActiveEnemies.clear();
}

void World::guideTarget()
{
    Command enemyCollector;
    enemyCollector.category = Category::Enemy;
    enemyCollector.action = derivedAction<Enemy>([this](Enemy& enemy, sf::Time)
    {
        if (!enemy.isDestroyed()) mActiveEnemies.push_back(&enemy);
    });

    Command guideTarget;
    guideTarget.category = Category::Enemy;
    guideTarget.action = derivedAction<Enemy>([this](Enemy& enemy, sf::Time)
    {
        if (!enemy.isDestroyed()) enemy.setTargetDirection(mPlayerCharacter->getWorldPosition());
    });
    mCommandQueue.push(enemyCollector);
    mCommandQueue.push(guideTarget);
    mActiveEnemies.clear();

    if (mBoss) mBoss->setTargetDirection(mPlayerCharacter->getWorldPosition());
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

    std::unique_ptr<SpriteNode> healthBar(new SpriteNode(mTextures.get(Textures::HealthBar)));
    mHealthBar = healthBar.get();
    mSceneLayers[GUILayer]->attachChild(std::move(healthBar));

    std::unique_ptr<SpriteNode> healthBarFrame(new SpriteNode(mTextures.get(Textures::HealthBarFrame)));
    mHealthBarFrame = healthBarFrame.get();
    mHealthBarFrame->setScale(0,1);
    mSceneLayers[GUILayer]->attachChild(std::move(healthBarFrame));

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

    std::unique_ptr<SpriteNode> ultimateIcon(new SpriteNode(mTextures.get(Textures::UltimateIcon)));
    mUltimateIcon = ultimateIcon.get();
    mSceneLayers[GUILayer]->attachChild(std::move(ultimateIcon));

    std::unique_ptr<SpriteNode> ultimateBlur(new SpriteNode(mTextures.get(Textures::IconBlur)));
    mUltimateBlur = ultimateBlur.get();
    mUltimateBlur->setScale(0,1);
    mSceneLayers[GUILayer]->attachChild(std::move(ultimateBlur));

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

    std::unique_ptr<TextNode> scoreDisplay(new TextNode(mFonts,""));
    mScoreText = scoreDisplay.get();
    mScoreText->setColor(sf::Color::White);
    mScoreText->setCharacterSize(25);
    mSceneLayers[GUILayer]->attachChild(std::move(scoreDisplay));

    std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
    mSceneGraph.attachChild(std::move(soundNode));
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
        if (matchesCategories(pair, Category::DarkWizzard, Category::AlliedProjectile))
        {
            if (pair.second && pair.first){
                std::cout << "Boss hit by projectile" << std::endl;
                auto& enemy = static_cast<DarkWizzard&>(*pair.first);
                auto& projectile = static_cast<Projectile&>(*pair.second);
                std::cout << "After 1 cast" << std::endl;
                if (!projectile.isDestroyed() && !enemy.isDestroyed()){
                    //enemy.setAnimation(DarkWizzard::TakedDamage);
                    enemy.damage(projectile.getDamage());
                    if (enemy.getHitpoints() <= 0){ 
                        mScore += 100;
                    }
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
                if (!projectile.isDestroyed() && !player.isDestroyed() && projectile.getType() != Projectile::ThunderStrike && projectile.getType() != Projectile::ExplosionAttack){
                    player.setAnimation(Witch::TakedDamage);
                    player.damage(projectile.getDamage());
                    projectile.destroy();
                }
                else if (!projectile.isDestroyed() && !player.isDestroyed() && projectile.getType() == Projectile::ThunderStrike && projectile.getCurrentAnimation() == 5){
                    player.setAnimation(Witch::TakedDamage);
                    player.damage(projectile.getDamage());
                    projectile.destroy();
                }
                else if (!projectile.isDestroyed() && !player.isDestroyed() && projectile.getType() == Projectile::ExplosionAttack && projectile.getCurrentAnimation() >= 8 && projectile.getCurrentAnimation() <= 11){
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
                if (!player.isDestroyed() && !enemy.isDestroyed() && ((enemy.getCurrentAnimation()==Enemy::Attack1 && enemy.getNumRow()==6) || (enemy.getCurrentAnimation()==Enemy::Attack2 && enemy.getNumRow()==6))){
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
                if (!projectile.isDestroyed() && !enemy.isDestroyed()){
	                sf::Vector2f initPos = enemy.getWorldPosition();
	                sf::Vector2f worldPos = projectile.getWorldPosition();
                    float angle = std::atan2(initPos.y - worldPos.y, initPos.x - worldPos.x);
                    sf::Vector2f velocity;
                    velocity.x = std::cos(angle);
                    velocity.y = std::sin(angle);
                    std::cout << projectile.getDamage() << std::endl;
                    enemy.knockback(velocity, 20);
                    enemy.damage(projectile.getDamage());
                    if (enemy.getHitpoints() <= 0){ 
                        mPlayerCharacter->receiveExp(enemy.getExpPoint());
                        float percent = mPlayerCharacter->getExpRatio();
                        mExpBarFrame->setScale(percent,1);
                        mScore += enemy.getExpPoint();
                    }
                    if (projectile.getType() == Projectile::AlliedBullet) projectile.destroy();
                }
            }
        }
    }
}

void World::addBosses()
{
    std::unique_ptr<DarkWizzard> boss(new DarkWizzard(mTextures, mFonts));
    mBoss = boss.get();
    mBoss->setPosition(mPlayerCharacter->getWorldPosition().x+600, mPlayerCharacter->getWorldPosition().y);
    mSceneLayers[Ground]->attachChild(std::move(boss));
    hasBossSpawn = true;
    isBoss = true;
}

int World::getScore()
{
    return mScore;
}

sf::Time World::getPlayedTime()
{
    return mPlayedTime;
}

bool World::getInvicible()
{
    return invicible;
}

void World::setInvicible(bool invicible)
{
    this->invicible = invicible;
}

void World::setPlayedTime(sf::Time playedTime)
{
    mPlayedTime = playedTime;
}