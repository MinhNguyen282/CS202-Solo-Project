#ifndef SETTINGSTATE_HPP
#define SETTINGSTATE_HPP

#include "State.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Label.hpp"

#include <SFML/Graphics.hpp>

#include <array>

class SettingState : public State
{
    public:
        SettingState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time deltaTime);
        virtual bool handleEvent(const sf::Event& event);
    private:
        void updateLabels();
        void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);
    private:
        sf::Sprite mBackgroundSprite;
        GUI::Container mGUIContainer;
        std::array<GUI::Button::Ptr, Player::actionCount> mBindingButtons;
        std::array<GUI::Label::Ptr, Player::actionCount> mBindingLabels;
};

#endif // SETTINGSTATE_HPP