#include "include/SettingState.hpp"
#include "include/Utility.hpp" 
#include "include/ResourcesHolder.hpp"
#include "include/MusicPlayer.hpp"
#include "include/SoundPlayer.hpp"

#include <string.h>
#include <SFML/Graphics/RenderWindow.hpp>

SettingState::SettingState(StateStack& stack, Context context)
: State(stack, context)
, mActiveButtons(-1)
, mIsScrolling(false)
, mIsScrollingSound(false)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::Title));
	mBlur.setSize(sf::Vector2f(1200.f, 720.f));
	mBlur.setFillColor(sf::Color(73, 66, 228));
	mBlur.setPosition(0.f, 0.f);

	mTitle.setFont(context.fonts->get(Fonts::Title));
	mTitle.setString("Settings");
	mTitle.setCharacterSize(75);
	mTitle.setFillColor(sf::Color::White);
	mTitle.setPosition((1200 - mTitle.getGlobalBounds().width)/2.f, 25.f);

	mMusicText.setFont(context.fonts->get(Fonts::Main));
	mMusicText.setString("Music");
	mMusicText.setCharacterSize(25);
	mMusicText.setPosition(520 - mMusicText.getGlobalBounds().width - 15, 190 + (40 - mMusicText.getGlobalBounds().height)/8);

	mMusicScrollBar.setTexture(context.textures->get(Textures::scrollBar));
	mMusicScrollBar.setPosition(600, 200);

	mMusicIcon.setTexture(context.textures->get(Textures::musicIcon));
	mMusicIcon.setPosition(520, 178);

	mMusicScrollThumb.setTexture(context.textures->get(Textures::scrollBarButtonNormal));
	float percent = getContext().music->getVolume() / 100.f;
	mMusicScrollThumb.setPosition(600.f + 420.f * percent, 190);

	mMusicLabel.setFont(context.fonts->get(Fonts::Main));
	mMusicLabel.setString(std::to_string(static_cast<int>(percent * 100.f)));
	mMusicLabel.setCharacterSize(25);
	mMusicLabel.setPosition(1120, 190 + (40 - mMusicLabel.getGlobalBounds().height)/8);

	mSoundText.setFont(context.fonts->get(Fonts::Main));
	mSoundText.setString("Sound");
	mSoundText.setCharacterSize(25);
	mSoundText.setPosition(520 - mSoundText.getGlobalBounds().width - 15, 390 + (40 - mSoundText.getGlobalBounds().height)/8);

	mSoundScrollBar.setTexture(context.textures->get(Textures::scrollBar));
	mSoundScrollBar.setPosition(600, 400);

	mSoundIcon.setTexture(context.textures->get(Textures::soundIcon));
	mSoundIcon.setPosition(520, 378);

	mSoundScrollThumb.setTexture(context.textures->get(Textures::scrollBarButtonNormal));
	percent = getContext().sounds->getVolume() / 100.f;
	mSoundScrollThumb.setPosition(600.f + 420.f * percent, 390);

	mSoundLabel.setFont(context.fonts->get(Fonts::Main));
	mSoundLabel.setString(std::to_string(static_cast<int>(percent * 100.f)));
	mSoundLabel.setCharacterSize(25);
	mSoundLabel.setPosition(1120, 390 + (40 - mSoundLabel.getGlobalBounds().height)/8);

	Button exit(*context.textures, *context.fonts, sf::Vector2f(100.f, 50.f), "Back", [this] (){
		requestStackPop();
	});

    addButtonLabel(Player::moveLeft, 150.f, "Move Left", context);
    addButtonLabel(Player::moveRight, 210.f, "Move Right", context);
    addButtonLabel(Player::moveUp, 270.f, "Move Up", context);
    addButtonLabel(Player::moveDown, 330.f, "Move Down", context);
    addButtonLabel(Player::launchAbility, 390.f, "Ability", context);
	addButtonLabel(Player::launchDebuff, 450.f, "Debuff", context);
	addButtonLabel(Player::launchUltimate, 510.f, "Ultimate", context);
    addButtonLabel(Player::attack, 570.f, "Attack", context);

	mButtons.push_back(std::move(exit));

    updateLabels();
}

void SettingState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mBlur);
	window.draw(mTitle);
	window.draw(mMusicScrollBar);
	window.draw(mMusicScrollThumb);
	window.draw(mMusicIcon);
	window.draw(mMusicLabel);
	window.draw(mMusicText);
	window.draw(mSoundScrollBar);
	window.draw(mSoundScrollThumb);
	window.draw(mSoundIcon);
	window.draw(mSoundLabel);
	window.draw(mSoundText);
	for(std::size_t i = 0; i < Player::actionCount; ++i)
	{
		window.draw(mBindingButtons[i]);
		window.draw(mBindingTexts[i]);
		window.draw(mBindingLabels[i]);
	}
	for(auto& button : mButtons)
	{
		window.draw(button);
	}
}

bool SettingState::update(sf::Time)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	for(auto & button : mButtons)
		button.update(sf::Time::Zero, mousePos);

	if (mIsScrolling){
		float percent = (mousePosF.x - 640.f) / 420.f;
		if (percent < 0.f)
			percent = 0.f;
		else if (percent > 1.f)
			percent = 1.f;
		mMusicScrollThumb.setTexture(getContext().textures->get(Textures::scrollBarButtonSelected));
		mMusicScrollThumb.setPosition(600.f + 420.f * percent, 190);
		getContext().music->setVolume(percent * 100.f);
		mMusicLabel.setString(std::to_string(static_cast<int>(percent * 100.f)));
	}
	else{
		mMusicScrollThumb.setTexture(getContext().textures->get(Textures::scrollBarButtonNormal));
	}

	if (mIsScrollingSound){
		float percent = (mousePosF.x - 640.f) / 420.f;
		if (percent < 0.f)
			percent = 0.f;
		else if (percent > 1.f)
			percent = 1.f;
		mSoundScrollThumb.setTexture(getContext().textures->get(Textures::scrollBarButtonSelected));
		mSoundScrollThumb.setPosition(600.f + 420.f * percent, 390);
		getContext().sounds->setVolume(percent * 100.f);
		mSoundLabel.setString(std::to_string(static_cast<int>(percent * 100.f)));
	}
	else{
		mSoundScrollThumb.setTexture(getContext().textures->get(Textures::scrollBarButtonNormal));
	}
	
	for(std::size_t action = 0; action < Player::actionCount; ++action){
		mBindingButtons[action].setTexture(getContext().textures->get(Textures::ButtonNormal));
		mBindingLabels[action].setFillColor(sf::Color::White);
	}

	if (mActiveButtons == -1){
		for(std::size_t action = 0; action < Player::actionCount; ++action)
		{
			if(mBindingButtons[action].getGlobalBounds().contains(mousePosF))
			{
				mBindingButtons[action].setTexture(getContext().textures->get(Textures::ButtonPressed));
				mBindingLabels[action].setFillColor(sf::Color::Red);
			}
			else
			{
				mBindingButtons[action].setTexture(getContext().textures->get(Textures::ButtonNormal));
				mBindingLabels[action].setFillColor(sf::Color::White);
			}
		}
	}
	else{
		mBindingButtons[mActiveButtons].setTexture(getContext().textures->get(Textures::ButtonPressed));
		mBindingLabels[mActiveButtons].setFillColor(sf::Color::Red);
	}

	return true;
}

bool SettingState::handleEvent(const sf::Event& event)
{
	for(auto& button : mButtons)
		button.handleEvent(event, sf::Mouse::getPosition(*getContext().window));
	
	if (event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
		sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		for(std::size_t action = 0; action < Player::actionCount; ++action)
		{
			if(mBindingButtons[action].getGlobalBounds().contains(mousePosF))
			{
				mActiveButtons = action;
				mBindingButtons[action].setTexture(getContext().textures->get(Textures::ButtonPressed));
				mBindingLabels[action].setFillColor(sf::Color::Red);
			}
		}
		if (mMusicScrollThumb.getGlobalBounds().contains(mousePosF))
		{
			mIsScrolling = true;
		}
		if (mSoundScrollThumb.getGlobalBounds().contains(mousePosF))
		{
			mIsScrollingSound = true;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased  && event.mouseButton.button == sf::Mouse::Left)
	{
		mIsScrolling = false;
		mIsScrollingSound = false;
		saveSettings();
	}
	else if (event.type == sf::Event::KeyPressed && mActiveButtons != -1)
	{
		getContext().player->assignKey(static_cast<Player::Action>(mActiveButtons), event.key.code);
		mBindingButtons[mActiveButtons].setTexture(getContext().textures->get(Textures::ButtonNormal));
		mBindingLabels[mActiveButtons].setFillColor(sf::Color::White);
		mActiveButtons = -1;
		updateLabels();
	}
	else if (event.type == sf::Event::KeyReleased && mActiveButtons != -1)
	{
		mBindingButtons[mActiveButtons].setTexture(getContext().textures->get(Textures::ButtonNormal));
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
	mBindingButtons[action].setPosition(80.f, y);
	mBindingButtons[action].setTexture(context.textures->get(Textures::ButtonNormal));

	mBindingTexts[action].setFont(context.fonts->get(Fonts::Main));
	mBindingTexts[action].setString(text);
	mBindingTexts[action].setCharacterSize(20);
	mBindingTexts[action].setPosition(80.f + (200 - mBindingTexts[action].getGlobalBounds().width)/2, y + (50 - mBindingTexts[action].getGlobalBounds().height)/4);
	mBindingTexts[action].setFillColor(sf::Color::White);

	mBindingLabels[action].setFont(context.fonts->get(Fonts::Main));
	mBindingLabels[action].setString(": " + toString(context.player->getAssignedKey(action)));
	mBindingLabels[action].setCharacterSize(20);
	mBindingLabels[action].setPosition(80.f + mBindingButtons[action].getGlobalBounds().width + 10, y + (50 - mBindingLabels[action].getGlobalBounds().height)/4);
	mBindingLabels[action].setFillColor(sf::Color::White);
}

void SettingState::saveSettings()
{
	std::ofstream file;
	file.open("Data/SoundSetting.dat");
	std::cout << "Music volume: " << getContext().music->getVolume() << std::endl;
	file << getContext().music->getVolume() << std::endl;
	file << getContext().sounds->getVolume() << std::endl;
	file.close();
}