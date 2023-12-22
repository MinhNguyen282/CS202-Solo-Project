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
#include "DarkWizzard.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "CommandQueue.hpp"
#include "ResourcesIdentifier.hpp"
#include "ResourcesHolder.hpp"
#include "BloomEffect.hpp"
#include "SoundPlayer.hpp"

namespace sf
{
    class RenderWindow;
}

class World : private sf::NonCopyable
{
    public:
        explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
        void update(sf::Time deltaTime);
        void draw();
        CommandQueue& getCommandQueue();
        bool hasAlivePlayer();
        bool isBossDefeated();
        void spawnEnemies();
        void handleCollisions();
        void setMousePosition(sf::Vector2i mousePosition);
        int getScore();
        bool getInvicible();
        sf::Time getPlayedTime();
        void setPlayedTime(sf::Time playedTime);
        void setInvicible(bool invicible);

    private:
        void loadTextures();


        void buildScene();
        void addBosses();
        void destroyEntitiesOutsideView();
        void guideTarget();
        void guideMissiles();
        void adaptPlayerPosition();
        void adaptPlayerVelocity();
        void adaptGUI();
        void updateSounds();


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

    private:
        sf::RenderTarget& mTarget;
        sf::RenderTexture mSceneTexture;
        BloomEffect mBloomEffect;
        sf::View mWorldView;
        TextureHolder mTextures;
        FontHolder& mFonts;
        SoundPlayer& mSounds;

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
        DarkWizzard* mBoss;

        bool isBoss = false;
        bool hasBossSpawn = false;

        sf::Time mIsInvicibleTime = sf::Time::Zero;
        sf::Time mSpawnTime = sf::Time::Zero;
        sf::Time mPlayedTime = sf::Time::Zero;
        int numEnemy = 0;
        int mScore;

        //GUI
        SpriteNode* mHealthBar;
        SpriteNode* mHealthBarFrame;
        SpriteNode* mExpBar;
        SpriteNode* mExpBarFrame;
        SpriteNode* mSkillEIcon;
        SpriteNode* mSkillEBlur;
        SpriteNode* mSkillQIcon;
        SpriteNode* mSkillQBlur;
        SpriteNode* mUltimateIcon;
        SpriteNode* mUltimateBlur;

        TextNode* mPlayedTimeText;
        TextNode* mLevelText;
        TextNode* mScoreText;
};

#endif // WORLD_HPP