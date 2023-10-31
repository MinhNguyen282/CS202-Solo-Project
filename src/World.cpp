#include "include/World.hpp"
#include "include/Witch.hpp"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 3000.f, mWorldView.getSize().y)
, mSpawnPosition(mWorldView.getSize().x / 2, mWorldBounds.height - mWorldView.getSize().y / 2)
, mScrollSpeed(-50.f)
, mPlayerCharacter(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time deltaTime)
{
    //mWorldView.move(-mScrollSpeed * deltaTime.asSeconds(), 0.f);

    destroyEntitiesOutsideView();
    guideMissiles();
    spawnEnemies();

    while (mCommandQueue.isEmpty() == false)
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), deltaTime);
    }

    sf::Vector2f velocity = mPlayerCharacter->getVelocity();
    if (velocity.x != 0.f && velocity.y != 0.f)
    {
        mPlayerCharacter->setVelocity(velocity / std::sqrt(2.f));
    }

    //mPlayerCharacter->accelerate(-mScrollSpeed, 0.f);

    mSceneGraph.update(deltaTime,mCommandQueue);
    mPlayerCharacter->setVelocity(0.f, 0.f);

    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 10.f;

    sf::Vector2f position = mPlayerCharacter->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance - mPlayerCharacter->getBoundingRect().width);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance - mPlayerCharacter->getBoundingRect().height);
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
    mFonts.load(Fonts::Main, "Media/buddychampion.ttf");
}

void World::spawnEnemies()
{
    if (mEnemySpawnPoints.empty()) return;
    std::cout << "Boss spam" << '\n';
    SpawnPoint spawn = mEnemySpawnPoints.back();
    std::unique_ptr<MechaBoss> enemy(new MechaBoss( mTextures, mFonts, mWindow));
    enemy->setPosition(spawn.x, spawn.y);
    mSceneLayers[Ground]->attachChild(std::move(enemy));
    mEnemySpawnPoints.pop_back();
}

void World::addEnemy(float relX, float relY)
{
    SpawnPoint spawn(mSpawnPosition.x + relX, mSpawnPosition.y - relY);
    mEnemySpawnPoints.push_back(spawn);
}

void World::addEnemies()
{
    addEnemy(-50.f, -50.f);

    std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
    {
        return lhs.y < rhs.y;
    });
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
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    sf::Texture& texture = mTextures.get(Textures::Desert);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));


    std::unique_ptr<Witch> player(new Witch(Witch::BlueWitch, mTextures, mFonts, mWindow));
    mPlayerCharacter = player.get();
    sf::FloatRect bounds = mPlayerCharacter->getBoundingRect();
    mPlayerCharacter->setPosition(mSpawnPosition);
    mSceneLayers[Ground]->attachChild(std::move(player));

    std::unique_ptr<MechaBoss> enemy(new MechaBoss( mTextures, mFonts, mWindow));
    enemy->setPosition(mSpawnPosition.x + 50, mSpawnPosition.y);
    mSceneLayers[Ground]->attachChild(std::move(enemy));
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