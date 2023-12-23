#ifndef CONTINUESTATE_HPP
#define CONTINUESTATE_HPP

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Player.hpp"

class ContinueState : public State
{
    public:
        ContinueState(StateStack& stack, Context context);
        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time deltaTime);
        virtual void draw();
    private:
        sf::RectangleShape mBackgroundSprite;
        sf::Text mText;
        std::vector<Button> mButtons;
        Player& mPlayer;
};

#endif // CONTINUESTATE_HPP