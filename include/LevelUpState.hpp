#ifndef LEVELUPSTATE_HPP
#define LEVELUPSTATE_HPP

#include "State.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>

class LevelUpState : public State
{
    public:
        LevelUpState(StateStack& stack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);

    private:
        sf::RectangleShape mBackground;
        std::vector<Button> mButtons;
        std::vector<sf::Text> mTexts;
        Player& mPlayer;
};

#endif // LEVELUPSTATE_HPP