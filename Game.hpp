#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>

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
};

#endif // GAME_HPP