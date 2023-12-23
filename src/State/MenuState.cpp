#include "include/MenuState.hpp"
#include "include/Utility.hpp"
#include "include/Button.hpp"
#include "include/MusicPlayer.hpp"
#include "include/ResourcesHolder.hpp"

#include <SFML/Graphics.hpp>

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
{
    sf::Texture& texture = context.textures->get(Textures::Title);
    sf::Font& font = context.fonts->get(Fonts::Main);

	//play menu music 
	context.music->play(Music::MenuTheme);

	mTitle.setFont(context.fonts->get(Fonts::Title));
	mTitle.setString("The Little Witch's Adventure");
	mTitle.setCharacterSize(75);
	mTitle.setFillColor(sf::Color::Black);
	mTitle.setPosition((1200 - mTitle.getGlobalBounds().getSize().x)/2, 100.f);

    mBackgroundSprite.setTexture(texture);

	mPanel.setTexture(context.textures->get(Textures::Panel));
	mPanel.setPosition(100.f, 350.f);


	Button playButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 400.f), "Play", [this](){
		requestStackPop();
		requestStackPush(States::Continue);
	});

	Button settingsButton(*context.textures, *context.fonts, sf::Vector2f(250.f, 500.f), "Settings", [this](){
		requestStackPush(States::Setting);
	});

	Button upgradeButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 500.f), "Upgrade", [this](){
		requestStackPush(States::Upgrade);
	});

	Button creditButton(*context.textures, *context.fonts, sf::Vector2f(750.f, 500.f), "Credit", [this](){
		requestStackPush(States::Credit);
	});

	Button exitButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 600.f), "Exit", [this](){
		requestStackPop();
	});

	mButtons.push_back(std::move(playButton));
	mButtons.push_back(std::move(settingsButton));
	mButtons.push_back(std::move(upgradeButton));
	mButtons.push_back(std::move(creditButton));
	mButtons.push_back(std::move(exitButton));
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mTitle);
	window.draw(mPanel);
	for (auto &button : mButtons){
		window.draw(button);
	}
}

bool MenuState::update(sf::Time)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	for(int i=0; i<mButtons.size(); i++){
		mButtons[i].update(sf::Time::Zero, mousePos);
	}
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	for(int i=0; i<mButtons.size(); i++){
		mButtons[i].handleEvent(event, mousePos);
	}
	// Press Ctrl + Shift + F to enter config state
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F && event.key.control && event.key.shift)
	{
		requestStackPush(States::Config);
	}
    return false;
}