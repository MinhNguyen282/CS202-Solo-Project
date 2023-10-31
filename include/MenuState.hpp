#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics.hpp>

class MenuState : public State
{
    public:
        MenuState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time deltaTime);
        virtual bool handleEvent(const sf::Event& event);
    
    private:
        sf::Sprite mBackgroundSprite;
        sf::Sprite mPanel;
        GUI::Container mGUIContainer;
};

#endif // MENUSTATE_HPP