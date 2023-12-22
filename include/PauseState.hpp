#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include <SFML/Graphics.hpp>

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
        sf::Text mPausedText;
        sf::Text mInstructionText;
};

#endif // PAUSESTATE_HPP