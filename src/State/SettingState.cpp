#include "include/SettingState.hpp"
#include "include/Utility.hpp" 
#include "include/ResourcesHolder.hpp"

#include <string.h>
#include <SFML/Graphics/RenderWindow.hpp>

SettingState::SettingState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::Title));

    addButtonLabel(Player::moveLeft, 150.f, "Move Left", context);
    addButtonLabel(Player::moveRight, 210.f, "Move Right", context);
    addButtonLabel(Player::moveUp, 270.f, "Move Up", context);
    addButtonLabel(Player::moveDown, 330.f, "Move Down", context);
    addButtonLabel(Player::charge, 390.f, "Charge", context);
    addButtonLabel(Player::attack, 450.f, "Attack", context);

    updateLabels();

    auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    backButton->setPosition(250.f, 510.f);
    backButton->setText("Back");
    backButton->setTextPosition((backButton->getBoundingRect().width - backButton->getTextBounding().width)/2,(backButton->getBoundingRect().height - backButton->getTextBounding().height)/4);
    backButton->setTextSize(20);
    backButton->setTextColor(sf::Color::Black);
    backButton->setCallback(std::bind(&SettingState::requestStackPop, this));

    mGUIContainer.pack(backButton);
}

void SettingState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingState::update(sf::Time)
{
	return true;
}

bool SettingState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < Player::actionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);

	return false;
}

void SettingState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::actionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i]->setText(": " + toString(key));
	}
}

void SettingState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(250.f, y);
	mBindingButtons[action]->setText(text);
    mBindingButtons[action]->setTextColor(sf::Color::Black);
    mBindingButtons[action]->setTextSize(25);
    mBindingButtons[action]->setTextPosition((mBindingButtons[action]->getBoundingRect().width - mBindingButtons[action]->getTextBounding().width)/2,(mBindingButtons[action]->getBoundingRect().height - mBindingButtons[action]->getTextBounding().height)/4);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
    mBindingLabels[action]->setTextColor(sf::Color::Black);
    mBindingLabels[action]->setTextSize(25);
	mBindingLabels[action]->setPosition(250 + mBindingButtons[action]->getBoundingRect().width + 25 , y + 10);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}