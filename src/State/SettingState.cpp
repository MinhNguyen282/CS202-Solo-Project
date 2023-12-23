#include "include/SettingState.hpp"
#include "include/Utility.hpp" 
#include "include/ResourcesHolder.hpp"

#include <string.h>
#include <SFML/Graphics/RenderWindow.hpp>

SettingState::SettingState(StateStack& stack, Context context)
: State(stack, context)
, mActiveButtons(-1)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::Title));
	mBlur.setSize(sf::Vector2f(1200.f, 720.f));
	mBlur.setFillColor(sf::Color(0, 0, 0, 200));
	mBlur.setPosition(0.f, 0.f);

	mExit.setSize(sf::Vector2f(100.f, 50.f));
	mExit.setFillColor(sf::Color::Black);
	mExit.setOutlineThickness(2.f);
	mExit.setOutlineColor(sf::Color::White);
	mExit.setPosition(50.f, 90.f);

	mExitText.setFont(context.fonts->get(Fonts::Main));
	mExitText.setString("Exit");
	mExitText.setCharacterSize(20);
	mExitText.setFillColor(sf::Color::White);
	mExitText.setPosition(55.f, 95.f);

    addButtonLabel(Player::moveLeft, 150.f, "Move Left", context);
    addButtonLabel(Player::moveRight, 210.f, "Move Right", context);
    addButtonLabel(Player::moveUp, 270.f, "Move Up", context);
    addButtonLabel(Player::moveDown, 330.f, "Move Down", context);
    addButtonLabel(Player::launchAbility, 390.f, "Ability", context);
	addButtonLabel(Player::launchDebuff, 450.f, "Debuff", context);
	addButtonLabel(Player::launchUltimate, 510.f, "Ultimate", context);
    addButtonLabel(Player::attack, 570.f, "Attack", context);

    updateLabels();
}

void SettingState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mBlur);
	window.draw(mExit);
	window.draw(mExitText);
	for(std::size_t i = 0; i < Player::actionCount; ++i)
	{
		window.draw(mBindingButtons[i]);
		window.draw(mBindingTexts[i]);
		window.draw(mBindingLabels[i]);
	}
}

bool SettingState::update(sf::Time)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	
	for(std::size_t action = 0; action < Player::actionCount; ++action){
		mBindingButtons[action].setFillColor(sf::Color::Black);
		mBindingTexts[action].setFillColor(sf::Color::White);
		mBindingLabels[action].setFillColor(sf::Color::White);
	}

	if (mActiveButtons == -1){
		for(std::size_t action = 0; action < Player::actionCount; ++action)
		{
			if(mBindingButtons[action].getGlobalBounds().contains(mousePosF))
			{
				mBindingButtons[action].setFillColor(sf::Color::White);
				mBindingTexts[action].setFillColor(sf::Color::Black);
				mBindingLabels[action].setFillColor(sf::Color::Red);
			}
			else
			{
				mBindingButtons[action].setFillColor(sf::Color::Black);
				mBindingTexts[action].setFillColor(sf::Color::White);
				mBindingLabels[action].setFillColor(sf::Color::White);
			}
		}
	}
	else{
		mBindingButtons[mActiveButtons].setFillColor(sf::Color::White);
		mBindingTexts[mActiveButtons].setFillColor(sf::Color::Black);
		mBindingLabels[mActiveButtons].setFillColor(sf::Color::Red);
	}

	if (mExit.getGlobalBounds().contains(mousePosF)){
		mExit.setFillColor(sf::Color::White);
		mExit.setOutlineColor(sf::Color::Black);
		mExitText.setFillColor(sf::Color::Red);
	}
	else{
		mExit.setFillColor(sf::Color::Black);
		mExit.setOutlineColor(sf::Color::White);
		mExitText.setFillColor(sf::Color::White);
	}

	return true;
}

bool SettingState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		for(std::size_t action = 0; action < Player::actionCount; ++action)
		{
			if(mBindingButtons[action].getGlobalBounds().contains(mousePosF))
			{
				mActiveButtons = action;
				mBindingButtons[action].setFillColor(sf::Color::White);
				mBindingTexts[action].setFillColor(sf::Color::Black);
				mBindingLabels[action].setFillColor(sf::Color::Red);
			}
		}
		if (mExit.getGlobalBounds().contains(mousePosF)){
			requestStackPop();
		}
	}
	else if (event.type == sf::Event::KeyPressed && mActiveButtons != -1)
	{
		getContext().player->assignKey(static_cast<Player::Action>(mActiveButtons), event.key.code);
		mBindingButtons[mActiveButtons].setFillColor(sf::Color::Black);
		mBindingTexts[mActiveButtons].setFillColor(sf::Color::White);
		mBindingLabels[mActiveButtons].setFillColor(sf::Color::White);
		mActiveButtons = -1;
		updateLabels();
	}
	else if (event.type == sf::Event::KeyReleased && mActiveButtons != -1)
	{
		mBindingButtons[mActiveButtons].setFillColor(sf::Color::Black);
		mBindingTexts[mActiveButtons].setFillColor(sf::Color::White);
		mBindingLabels[mActiveButtons].setFillColor(sf::Color::White);
		mActiveButtons = -1;
	}
	else if (event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Right)
	{
		mActiveButtons = -1;
	}
	return false;
}

void SettingState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::actionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i].setString(": " + toString(key));
	}
}

void SettingState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action].setSize(sf::Vector2f(100.f, 50.f));
	mBindingButtons[action].setPosition(80.f, y);
	mBindingButtons[action].setFillColor(sf::Color::Black);
	mBindingButtons[action].setOutlineColor(sf::Color::White);
	mBindingButtons[action].setOutlineThickness(2.f);

	mBindingTexts[action].setFont(context.fonts->get(Fonts::Main));
	mBindingTexts[action].setPosition(85.f, y + 5.f);
	mBindingTexts[action].setString(text);
	mBindingTexts[action].setCharacterSize(20);
	mBindingTexts[action].setFillColor(sf::Color::White);

	mBindingLabels[action].setFont(context.fonts->get(Fonts::Main));
	mBindingLabels[action].setPosition(190.f, y + 5.f);
	mBindingLabels[action].setString(": " + toString(context.player->getAssignedKey(action)));
	mBindingLabels[action].setCharacterSize(20);
	mBindingLabels[action].setFillColor(sf::Color::White);
}