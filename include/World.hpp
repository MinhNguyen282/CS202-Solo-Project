#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <math.h>
#include <iostream>

#include "Witch.hpp"
#include "MechaRockBoss.hpp"
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
        bool hasAlivePlayer();
        bool isBossDefeated();

    private:
        void loadTextures();


        void buildScene();
        void addEnemies();
        void addEnemy(float relX, float relY);
        void addBosses();
        void addBoss(float relX, float relY);
        void spawnEnemies();
        void destroyEntitiesOutsideView();
        void guideMissiles();
        void handleCollisions();
        void adaptPlayerPosition();
        void adaptPlayerVelocity();


        sf::FloatRect getViewBounds() const;
        sf::FloatRect getBattlefieldBounds() const;

    private:
        enum Layer
        {
            Background,
            Ground,
            LayerCount,
        };

        struct SpawnPoint
        {
            SpawnPoint(float x, float y)
            : x(x)
            , y(y)
            {
            }
            float x;
            float y;
        };

    private:
        sf::RenderWindow& mWindow;
        sf::View mWorldView;
        TextureHolder mTextures;
        FontHolder mFonts;

        SceneNode mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        float mScrollSpeed;

        Witch* mPlayerCharacter;
        CommandQueue mCommandQueue;

        std::vector<SpawnPoint> mEnemySpawnPoints;
        std::vector<MechaBoss*> mActiveEnemies;
        //Boss
        std::vector<SpawnPoint> mBossSpawnPoints;
        std::vector<MechaBoss*> mActiveBoss;
        MechaBoss* mBoss;
        bool isBoss = false;
        bool hasBossSpawn = false;
};

#endif // WORLD_HPP