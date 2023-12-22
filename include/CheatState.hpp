#ifndef CHEATSTATE_HPP
#define CHEATSTATE_HPP

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Player.hpp"

#include <string>

class CheatState : public State
{
    public:
        CheatState(StateStack& stack, Context context);
        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time deltaTime);
        virtual void draw();
    private:
        sf::RectangleShape mBackground;
        sf::Text mInvicible;
        sf::Text mPlayedTime;
        sf::RectangleShape mInvicibleButton; sf::Text mInvicibleButtonText;
        sf::RectangleShape mPlayedTimedMinus30; sf::Text mPlayedTimedMinus30Text;
        sf::RectangleShape mPlayedTimedPlus30; sf::Text mPlayedTimedPlus30Text;
        sf::RectangleShape mPlayedTimedMinus15; sf::Text mPlayedTimedMinus15Text;
        sf::RectangleShape mPlayedTimedPlus15; sf::Text mPlayedTimedPlus15Text;
        sf::RectangleShape mPlayedTimedMinus1; sf::Text mPlayedTimedMinus1Text;
        sf::RectangleShape mPlayedTimedPlus1; sf::Text mPlayedTimedPlus1Text;

        Player& mPlayer;
};

#endif // CHEATSTATE_HPP