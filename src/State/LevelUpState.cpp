#include "include/LevelUpState.hpp"
#include "include/Player.hpp"
#include "include/Utility.hpp"
#include "include/SoundPlayer.hpp"

LevelUpState::LevelUpState(StateStack& stack, Context context)
: State(stack, context)
, mBackground()
, mPlayer(*context.player)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    mBackground.setSize(windowSize);
    mBackground.setFillColor(sf::Color(0, 0, 0, 150));

    context.sounds->play(SoundEffect::LevelUp);

    // Level Up Text
    sf::Text levelUpText;
    levelUpText.setFont(font);
    levelUpText.setString("Level Up!");
    levelUpText.setCharacterSize(35);
    centerOrigin(levelUpText);
    levelUpText.setPosition(0.5f * windowSize.x, 0.2f * windowSize.y);
    mTexts.push_back(std::move(levelUpText));

    Button addHitpointsButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 300.f), "+10 Hitpoint", [this](){
        mPlayer.mCommand = "AddHitpoints";
        requestStackPop();
    });

    Button addDamageButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 400.f), "+10 Damage", [this](){
        mPlayer.mCommand = "AddDamage";
        requestStackPop();
    });

    Button addMovementSpeedButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 500.f), "+10 Movement Speed", [this](){
        mPlayer.mCommand = "AddMovementSpeed";
        requestStackPop();
    });

    mButtons.push_back(std::move(addHitpointsButton));
    mButtons.push_back(std::move(addDamageButton));
    mButtons.push_back(std::move(addMovementSpeedButton));
}

void LevelUpState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackground);

    for(auto& text : mTexts)
        window.draw(text);

    for(auto& button : mButtons)
        window.draw(button);
}

bool LevelUpState::update(sf::Time dt)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].update(dt, mousePos);
    }
    return false;
}

bool LevelUpState::handleEvent(const sf::Event& event)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].handleEvent(event, mousePos);
    }
    return false;
}