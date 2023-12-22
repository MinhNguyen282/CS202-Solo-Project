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

	mPanel.setSize(sf::Vector2f(1000.f, 350.f));
	mPanel.setFillColor(sf::Color(0, 0, 0, 200));
	mPanel.setOutlineColor(sf::Color::Blue);
	mPanel.setOutlineThickness(2.f);
	mPanel.setPosition(100.f, 350.f);


	playButton.setSize(sf::Vector2f(200.f, 50.f));
	playButton.setFillColor(sf::Color::Black);
	playButton.setOutlineThickness(2.f);
	playButton.setOutlineColor(sf::Color::White);
	playButton.setPosition(500.f, 400.f);

	playText.setFont(font);
	playText.setString("Play");
	playText.setCharacterSize(30);
	playText.setFillColor(sf::Color::White);
	playText.setPosition(500.f + (200.f - playText.getGlobalBounds().getSize().x)/2, 405.f);

	settingsButton.setSize(sf::Vector2f(200.f, 50.f));
	settingsButton.setFillColor(sf::Color::Black);
	settingsButton.setOutlineThickness(2.f);
	settingsButton.setOutlineColor(sf::Color::White);
	settingsButton.setPosition(250.f, 500.f);

	settingsText.setFont(font);
	settingsText.setString("Settings");
	settingsText.setCharacterSize(30);
	settingsText.setFillColor(sf::Color::White);
	settingsText.setPosition(250.f + (200 - settingsText.getGlobalBounds().getSize().x)/2, 505.f);

	upgradeButton.setSize(sf::Vector2f(200.f, 50.f));
	upgradeButton.setFillColor(sf::Color::Black);
	upgradeButton.setOutlineThickness(2.f);
	upgradeButton.setOutlineColor(sf::Color::White);
	upgradeButton.setPosition(500.f, 500.f);

	upgradeText.setFont(font);
	upgradeText.setString("Upgrade");
	upgradeText.setCharacterSize(30);
	upgradeText.setFillColor(sf::Color::White);
	upgradeText.setPosition(500.f + (200 - upgradeText.getGlobalBounds().getSize().x)/2, 505.f);

	creditButton.setSize(sf::Vector2f(200.f, 50.f));
	creditButton.setFillColor(sf::Color::Black);
	creditButton.setOutlineThickness(2.f);
	creditButton.setOutlineColor(sf::Color::White);
	creditButton.setPosition(750.f, 500.f);

	creditText.setFont(font);
	creditText.setString("Credit");
	creditText.setCharacterSize(30);
	creditText.setFillColor(sf::Color::White);
	creditText.setPosition(750.f + (200 - creditText.getGlobalBounds().getSize().x)/2, 505.f);

	exitButton.setSize(sf::Vector2f(200.f, 50.f));
	exitButton.setFillColor(sf::Color::Black);
	exitButton.setOutlineThickness(2.f);
	exitButton.setOutlineColor(sf::Color::White);
	exitButton.setPosition(500.f, 600.f);

	exitText.setFont(font);
	exitText.setString("Exit");
	exitText.setCharacterSize(30);
	exitText.setFillColor(sf::Color::White);
	exitText.setPosition(500.f + (200 - exitText.getGlobalBounds().getSize().x)/2, 605.f);

	mButtons.push_back(playButton);
	mButtons.push_back(settingsButton);
	mButtons.push_back(upgradeButton);
	mButtons.push_back(creditButton);
	mButtons.push_back(exitButton);

	mOptions.push_back(playText);
	mOptions.push_back(settingsText);
	mOptions.push_back(upgradeText);
	mOptions.push_back(creditText);
	mOptions.push_back(exitText);
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
	for (auto &text : mOptions){
		window.draw(text);
	}
}

bool MenuState::update(sf::Time)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	for(int i=0; i<mButtons.size(); i++){
		if (mButtons[i].getGlobalBounds().contains(mousePosF)){
			mButtons[i].setFillColor(sf::Color::White);
			mButtons[i].setOutlineColor(sf::Color::Black);
			mOptions[i].setFillColor(sf::Color::Black);
		}
		else{
			mButtons[i].setFillColor(sf::Color::Black);
			mButtons[i].setOutlineColor(sf::Color::White);
			mOptions[i].setFillColor(sf::Color::White);
		}
	}
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
		sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		if (playButton.getGlobalBounds().contains(mousePosF)){
			requestStackPop();
			requestStackPush(States::Loading);
		}
		else if (exitButton.getGlobalBounds().contains(mousePosF)){
			requestStackPop();
		}
		else if (upgradeButton.getGlobalBounds().contains(mousePosF)){
			requestStackPush(States::Upgrade);
		}
		else if (settingsButton.getGlobalBounds().contains(mousePosF)){
			requestStackPush(States::Setting);
		}
		else if (creditButton.getGlobalBounds().contains(mousePosF)){
			requestStackPush(States::Credit);
		}
	}
    return false;
}