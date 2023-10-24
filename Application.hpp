#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ResourcesHolder.hpp"
#include "ResourcesIdentifier.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "GameState.hpp"

class Application
{
    public:
        Application();
        void run();
    private:
        void processInput();
        void update(sf::Time dt);
        void render();
        void updateStatistics(sf::Time dt);
        void registerStates();
    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow mWindow;
        TextureHolder mTextures;
        FontHolder mFonts;
        Player mPlayer;

        StateStack mStateStack;

        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;
        std::size_t mStatisticsNumFrames;
};

#endif // APPLICATION_HPP