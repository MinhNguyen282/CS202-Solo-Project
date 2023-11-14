#include "include/MenuState.hpp"
#include "include/Utility.hpp"
#include "include/Button.hpp"
#include "include/ResourcesHolder.hpp"

#include <SFML/Graphics.hpp>

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
    sf::Texture& texture = context.textures->get(Textures::Title);
    sf::Font& font = context.fonts->get(Fonts::Main);

    mBackgroundSprite.setTexture(texture);
	mPanel.setTexture(context.textures->get(Textures::Panel));
	mPanel.setPosition((1200 - mPanel.getGlobalBounds().width)/2, 200);

	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	playButton->setPosition((1200 - playButton->getBoundingRect().width)/2 , 230);
	playButton->setText("Play");
	playButton->setTextPosition((playButton->getBoundingRect().width - playButton->getTextBounding().width)/2,(playButton->getBoundingRect().height - playButton->getTextBounding().height)/4);
	playButton->setTextSize(20);
	playButton->setTextColor(sf::Color::Black);
	playButton->setCallback([this] ()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto abilityButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	abilityButton->setPosition((1200 - abilityButton->getBoundingRect().width)/2, 290);
	abilityButton->setText("Ability");
	abilityButton->setTextPosition((abilityButton->getBoundingRect().width - abilityButton->getTextBounding().width)/2,(abilityButton->getBoundingRect().height - abilityButton->getTextBounding().height)/4);
	abilityButton->setTextSize(20);
	abilityButton->setTextColor(sf::Color::Black);
	abilityButton->setCallback([this] ()
	{
		//requestStackPush(States::Ability);
	});

	auto settingButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	settingButton->setPosition((1200 - settingButton->getBoundingRect().width)/2, 350);
	settingButton->setText("Setting");
	settingButton->setTextPosition((settingButton->getBoundingRect().width - settingButton->getTextBounding().width)/2,(settingButton->getBoundingRect().height - settingButton->getTextBounding().height)/4);
	settingButton->setTextSize(20);
	settingButton->setTextColor(sf::Color::Black);
	settingButton->setCallback([this] ()
	{
		requestStackPush(States::Setting);
	});

	auto creditButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	creditButton->setPosition((1200 - creditButton->getBoundingRect().width)/2, 410);
	creditButton->setText("Credit");
	creditButton->setTextPosition((creditButton->getBoundingRect().width - creditButton->getTextBounding().width)/2,(creditButton->getBoundingRect().height - creditButton->getTextBounding().height)/4);
	creditButton->setTextSize(20);
	creditButton->setTextColor(sf::Color::Black);
	creditButton->setCallback([this] ()
	{
		requestStackPush(States::Credit);
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	exitButton->setPosition((1200 - exitButton->getBoundingRect().width)/2, 470);
	exitButton->setText("Exit");
	exitButton->setTextPosition((exitButton->getBoundingRect().width - exitButton->getTextBounding().width)/2,(exitButton->getBoundingRect().height - exitButton->getTextBounding().height)/4);
	exitButton->setTextSize(20);
	exitButton->setTextColor(sf::Color::Black);
	exitButton->setCallback([this] ()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(abilityButton);
	mGUIContainer.pack(settingButton);
	mGUIContainer.pack(creditButton);
	mGUIContainer.pack(exitButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	window.draw(mPanel);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
    return false;
}