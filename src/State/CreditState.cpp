#include "include/CreditState.hpp"
#include "include/Utility.hpp"
#include "include/Button.hpp"
#include "include/ResourcesHolder.hpp"

#include <SFML/Graphics.hpp>

CreditState::CreditState(StateStack& stack, Context context)
: State(stack, context)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::Title));
    mPanel.setTexture(context.textures->get(Textures::CreditPanel));
	mPanel.setPosition((1200 - mPanel.getGlobalBounds().width)/2, (720 - mPanel.getGlobalBounds().height)/2);

    Button backButton(*context.textures, *context.fonts, sf::Vector2f(100.f, 100.f), "Back", [this](){
        requestStackPop();
    });

    mAuthorText.setFont(context.fonts->get(Fonts::Main));
    mAuthorText.setString("Dev: Nguyen Huu Hoang Minh - 22APCS1 - HCMUS");
    mAuthorText.setCharacterSize(35);
    mAuthorText.setFillColor(sf::Color::White);
    mAuthorText.setOutlineColor(sf::Color::Black);
    mAuthorText.setOutlineThickness(2.f);
    sf::FloatRect textRect = mAuthorText.getLocalBounds();
    mAuthorText.setOrigin(textRect.left + textRect.width/2.f, textRect.top  + textRect.height/2.f);
    mAuthorText.setPosition(600, 300);

    mCreditText.setFont(context.fonts->get(Fonts::Main));
    mCreditText.setString("Assets from: itch.io, opengameart.org, freesound.org");
    mCreditText.setCharacterSize(35);
    mCreditText.setFillColor(sf::Color::White);
    mCreditText.setOutlineColor(sf::Color::Black);
    mCreditText.setOutlineThickness(2.f);
    textRect = mCreditText.getLocalBounds();
    mCreditText.setOrigin(textRect.left + textRect.width/2.f, textRect.top  + textRect.height/2.f);
    mCreditText.setPosition(600, 400);

    mButtons.push_back(std::move(backButton));
}
 
bool CreditState::handleEvent(const sf::Event& event)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].handleEvent(event, mousePos);
    }
    return false;
}

bool CreditState::update(sf::Time deltaTime)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].update(deltaTime, mousePos);
    }
    return false;
}

void CreditState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mPanel);
    window.draw(mAuthorText);
    window.draw(mCreditText);
    for(auto& button : mButtons)
        window.draw(button);
}
