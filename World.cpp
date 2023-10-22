#include "World.hpp"

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
    mWorldView.move(-mScrollSpeed * deltaTime.asSeconds(), 0.f);

    mPlayerCharacter->setVelocity(0.f, 0.f);
    mSceneGraph.update(deltaTime);
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


    std::unique_ptr<Witch> player(new Witch(Witch::BlueWitch, mTextures));
    mPlayerCharacter = player.get();
    sf::FloatRect bounds = mPlayerCharacter->getLocalBounds();
    mPlayerCharacter->setPosition(mWorldView.getSize().x / 2 - bounds.width / 2, mWorldBounds.height - mWorldView.getSize().y / 2 - bounds.height / 2);
    mPlayerCharacter->setTextureRect(sf::IntRect(0, 0, 64, 96));
    mSceneLayers[Ground]->attachChild(std::move(player));
}