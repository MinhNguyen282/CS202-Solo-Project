#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"

#include <SFML/Graphics.hpp>

class MenuState : public State
{
    public:
        MenuState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time deltaTime);
        virtual bool handleEvent(const sf::Event& event);

        void updateOptionText();

    private:
        enum OptionNames
        {
            Play,
            Exit,
        };
    
    private:
        sf::Sprite mBackgroundSprite;

        std::vector<sf::Text> mOptions;
        std::size_t mOptionIndex;
};

#endif // MENUSTATE_HPP