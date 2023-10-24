#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Player.hpp"
#include "State.hpp"

class GameState : public State
{
    public:
        GameState(StateStack& stack, Context context);
        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time deltaTime);
        virtual void draw();
        
    private:
        World mWorld;
        bool mIsPaused = false;
        Player mPlayer;
};  

#endif // GAME_HPP