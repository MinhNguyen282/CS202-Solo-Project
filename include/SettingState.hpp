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
        void saveSettings();
    private:
        void updateLabels();
        void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);
    private:
        sf::Sprite mBackgroundSprite;
        sf::RectangleShape mBlur;
        sf::Text mTitle;
        std::size_t mActiveButtons;
        std::vector<Button> mButtons;

        sf::Sprite mMusicScrollBar;
        sf::Sprite mMusicScrollThumb;
        sf::Sprite mMusicIcon;
        sf::Text mMusicLabel;
        sf::Text mMusicText;
        bool mIsScrolling;

        sf::Sprite mSoundScrollBar;
        sf::Sprite mSoundScrollThumb;
        sf::Sprite mSoundIcon;
        sf::Text mSoundLabel;
        sf::Text mSoundText;
        bool mIsScrollingSound;

        std::array<sf::Sprite, Player::actionCount> mBindingButtons;
        std::array<sf::Text, Player::actionCount> mBindingTexts;
        std::array<sf::Text, Player::actionCount> mBindingLabels;
};

#endif // SETTINGSTATE_HPP