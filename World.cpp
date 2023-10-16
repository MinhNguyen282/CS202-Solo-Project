#include "World.hpp"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, 3000.f, 3000.f)
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
}

void World::buildScene()
{
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
    sf::Texture& texture = mTextures.get(Textures::BlueWitch);
    std::unique_ptr<Witch> player(new Witch(Witch::BlueWitch, mTextures));
    mPlayerCharacter = player.get();
    mPlayerCharacter->setPosition(mSpawnPosition);
    mSceneLayers[Ground]->attachChild(std::move(player));
}