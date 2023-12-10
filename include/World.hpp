#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <array>
#include <map>
#include <tuple>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>

#include "Witch.hpp"
#include "Enemy.hpp"
#include "MechaRockBoss.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "CommandQueue.hpp"
#include "ResourcesIdentifier.hpp"
#include "ResourcesHolder.hpp"
#include "BloomEffect.hpp"

namespace sf
{
    class RenderWindow;
}

class World : private sf::NonCopyable
{
    public:
        explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts);
        void update(sf::Time deltaTime);
        void draw();
        CommandQueue& getCommandQueue();
        bool hasAlivePlayer();
        bool isBossDefeated();
        void spawnEnemies();
        void handleCollisions();
        void setMousePosition(sf::Vector2i mousePosition);

    private:
        void loadTextures();


        void buildScene();
        void addBosses();
        void addBoss(float relX, float relY);
        void destroyEntitiesOutsideView();
        void guideMissiles();
        void adaptPlayerPosition();
        void adaptPlayerVelocity();
        void adaptGUI();


        sf::FloatRect getViewBounds() const;
        sf::FloatRect getBattlefieldBounds() const;

    private:
        enum Layer
        {
            Background,
            LowerGround,
            Ground,
            GUILayer,
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
        struct EnemySpawnPoint
        {
            EnemySpawnPoint(Enemy::Type type, float x, float y)
            : type(type)
            , x(x)
            , y(y)
            {
            }
            Enemy::Type type;
            float x;
            float y;
        };

    private:
        sf::RenderTarget& mTarget;
        sf::RenderTexture mSceneTexture;
        BloomEffect mBloomEffect;
        sf::View mWorldView;
        TextureHolder mTextures;
        FontHolder& mFonts;

        SceneNode mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        sf::Vector2f mMousePosition;
        float mScrollSpeed;

        Witch* mPlayerCharacter;
        bool invicible = false;
        CommandQueue mCommandQueue;

        //Enemy
        std::vector<Enemy*> mActiveEnemies;

        //Boss
        std::vector<SpawnPoint> mBossSpawnPoints;
        std::vector<MechaBoss*> mActiveBoss;
        MechaBoss* mBoss;

        bool isBoss = false;
        bool hasBossSpawn = false;

        sf::Time mIsInvicibleTime = sf::Time::Zero;
        sf::Time mSpawnTime = sf::Time::Zero;
        sf::Time mPlayedTime = sf::Time::Zero;
        int numEnemy = 0;

        //GUI
        SpriteNode* mExpBar;
        SpriteNode* mExpBarFrame;
        SpriteNode* mSkillEIcon;
        SpriteNode* mSkillEBlur;
        SpriteNode* mSkillQIcon;
        SpriteNode* mSkillQBlur;

        TextNode* mPlayedTimeText;
        TextNode* mLevelText;
};

#endif // WORLD_HPP