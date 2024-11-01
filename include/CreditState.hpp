#ifndef CREDITSTATE_HPP
#define CREDITSTATE_HPP

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"

class CreditState : public State
{
    public:
        CreditState(StateStack& stack, Context context);
        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time deltaTime);
        virtual void draw();
    private:
        sf::Sprite mBackgroundSprite, mPanel;
        sf::Text mCreditText;
        sf::Text mAuthorText;
        std::vector<Button> mButtons;
};

#endif // CREDITSTATE_HPP