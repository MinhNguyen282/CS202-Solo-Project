#include "include/TitleState.hpp"
#include "include/Utility.hpp"
#include "include/ResourcesHolder.hpp"
#include "include/MenuState.hpp"

#include <SFML/Graphics.hpp>

TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, mText()
, mShowText(true)
, mTextEffectTime(sf::Time::Zero)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::Title));
    
    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("Press any key to start");
    mText.setOutlineColor(sf::Color::Black);
    mText.setOutlineThickness(1.f);
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}

void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);

    if (mShowText)
        window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
        requestStackPush(States::Menu);
	}

	return true;
}