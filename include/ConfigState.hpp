#ifndef CONFIGSTATE_HPP
#define CONFIGSTATE_HPP

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Button.hpp"
#include "Enemy.hpp"
#include "DataTables.hpp"

class ConfigState : public State
{
    public:
        ConfigState(StateStack& stack, Context context);
        virtual bool handleEvent(const sf::Event& event);
        virtual bool update(sf::Time deltaTime);
        virtual void draw();
        void initStat(Textures::ID id, float x, float y, Enemy::Type type);
    private:
        sf::RectangleShape mBackgroundBlur;
        sf::Text mText;
        std::vector<Button> mButtons;
        std::vector<sf::Sprite> mSprites;
        std::map<Textures::ID, std::string> mStatNames;
        std::vector<EnemyData> mEnemyData;
};

#endif // CONFIGSTATE_HPP