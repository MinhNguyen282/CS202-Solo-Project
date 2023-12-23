#include "include/ContinueState.hpp"

ContinueState::ContinueState(StateStack& stack, Context context)
: State(stack, context)
, mBackgroundSprite()
, mText()
, mPlayer(*context.player)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    mBackgroundSprite.setSize(windowSize);
    mBackgroundSprite.setFillColor(sf::Color::Black);

    mText.setFont(font);
    mText.setString("Continue the last game?");
    mText.setCharacterSize(35);
    mText.setFillColor(sf::Color::White);
    mText.setOutlineColor(sf::Color::Black);
    mText.setOutlineThickness(2.f);
    sf::FloatRect textRect = mText.getLocalBounds();
    mText.setOrigin(textRect.left + textRect.width/2.f, textRect.top  + textRect.height/2.f);
    mText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

    Button yesButton(*context.textures, *context.fonts, sf::Vector2f(800.f/3, 500.f), "Yes", [this](){
		requestStackPop();
        mPlayer.readInfomation();
        mPlayer.isContinue = true;
		requestStackPush(States::Loading);
	});
    Button noButton(*context.textures, *context.fonts, sf::Vector2f(200 + 1600.f/3, 500.f), "No", [this](){
        requestStackPop();
        requestStackPush(States::Loading);
        mPlayer.isContinue = false;
    });

    mButtons.push_back(std::move(yesButton));
    mButtons.push_back(std::move(noButton));
}

void ContinueState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mText);
    for(auto& button : mButtons)
        window.draw(button);
}

bool ContinueState::update(sf::Time deltaTime)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].update(deltaTime, mousePos);
    }
    return false;
}

bool ContinueState::handleEvent(const sf::Event& event)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
    for(int i=0; i<mButtons.size(); i++){
        mButtons[i].handleEvent(event, mousePos);
    }
    return false;
}