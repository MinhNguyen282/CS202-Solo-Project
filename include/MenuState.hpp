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
        sf::Text mTitle;
        sf::RectangleShape mPanel;

        sf::RectangleShape playButton;
        sf::Text playText;
        sf::RectangleShape exitButton;
        sf::Text exitText;
        sf::RectangleShape settingsButton;
        sf::Text settingsText;
        sf::RectangleShape upgradeButton;
        sf::Text upgradeText;
        sf::RectangleShape creditButton;
        sf::Text creditText;
        std::vector<sf::RectangleShape> mButtons;
        std::vector<sf::Text> mOptions;
};

#endif // MENUSTATE_HPP