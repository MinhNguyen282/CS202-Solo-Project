#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"

class GameOverState : public State
{
	public:
		GameOverState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);


	private:
		sf::Text mGameOverText;
		sf::Text mScoreText;
		sf::Text mGuideText;
		sf::Time mElapsedTime;
};
#endif // GAMEOVERSTATE_HPP