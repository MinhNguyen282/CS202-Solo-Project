#include "include/GameOverState.hpp"
#include "include/Utility.hpp"
#include "include/Player.hpp"
#include "include/ResourcesHolder.hpp"

GameOverState::GameOverState(StateStack& stack, Context context)
: State(stack, context)
, mGameOverText()
, mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);
	if (context.player->getMissionStatus() == Player::MissionFailure)
		mGameOverText.setString("Mission failed!");	
	else
		mGameOverText.setString("Mission successful!");

	mScoreText.setFont(font);
	mScoreText.setString("Score: " + toString(context.player->getUpgradePoints()));
	mScoreText.setCharacterSize(20);
	centerOrigin(mScoreText);
	mScoreText.setPosition(0.5f * windowSize.x, 0.5f * windowSize.y);

	mGuideText.setFont(font);
	mGuideText.setString("Press Enter to return to the main menu");
	mGuideText.setCharacterSize(20);
	centerOrigin(mGuideText);
	mGuideText.setPosition(0.5f * windowSize.x, 0.6f * windowSize.y);

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
	window.draw(mScoreText);
	window.draw(mGuideText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += dt;
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
	{
		requestStateClear();
		requestStackPush(States::Menu);
	}
	return false;
}