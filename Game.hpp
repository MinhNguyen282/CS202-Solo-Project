#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Player.hpp"

class Game
{
    public:
        Game();
        void run();
    private:
        void processEvents();
        void update(sf::Time deltaTime);
        void render();
    private:
        sf::RenderWindow mWindow;
        World mWorld;
        bool mIsPaused = false;
        Player mPlayer;
};  

#endif // GAME_HPP