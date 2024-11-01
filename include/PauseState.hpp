#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class PauseState : public State
{
    public:
        PauseState(StateStack& stack, Context context);
        ~PauseState();

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);

    private:
        sf::Sprite mBackgroundSprite;
        sf::Sprite mPanel;
        std::vector<Button> mButtons;
        Player& mPlayer;
};

#endif // PAUSESTATE_HPP