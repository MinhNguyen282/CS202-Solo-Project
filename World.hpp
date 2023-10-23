#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <math.h>

#include "Witch.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "CommandQueue.hpp"
#include "ResourcesIdentifier.hpp"
#include "ResourcesHolder.hpp"

namespace sf
{
    class RenderWindow;
}

class World : private sf::NonCopyable
{
    public:
        explicit World(sf::RenderWindow &window);
        void update(sf::Time deltaTime);
        void draw();
        CommandQueue& getCommandQueue();
    private:
        void loadTextures();
        void buildScene();
    private:
        enum Layer
        {
            Background,
            Ground,
            LayerCount,
        };
    private:
        sf::RenderWindow& mWindow;
        sf::View mWorldView;
        TextureHolder mTextures;

        SceneNode mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        float mScrollSpeed;

        Witch* mPlayerCharacter;
        CommandQueue mCommandQueue;
};

#endif // WORLD_HPP