#include "include/PauseState.hpp"
#include "include/Utility.hpp"
#include "include/MusicPlayer.hpp"
#include "include/ResourcesHolder.hpp"

#include <SFML/Graphics.hpp>

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context) 
, mBackgroundSprite()
, mPlayer(*context.player)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f viewSize = context.window->getView().getSize();

	mPanel.setTexture(context.textures->get(Textures::bigPanel));
	mPanel.setPosition((1200-366)/2,100);

	Button resumeButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 100.f + 92.5), "Resume", [this](){
		requestStackPop();
	});
	Button saveButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 100.f + 2 * 92.5 + 50), "Save", [this](){
		mPlayer.saveInfomation();
	});
	Button exitButton(*context.textures, *context.fonts, sf::Vector2f(500.f, 100.f + 3 * 92.5 + 2 * 50), "Exit", [this](){
		requestStateClear();
		requestStackPush(States::Menu);
	});	

	mButtons.push_back(std::move(resumeButton));
	mButtons.push_back(std::move(saveButton));
	mButtons.push_back(std::move(exitButton));

	context.music->setPaused(true);
}

PauseState::~PauseState()
{
	getContext().music->setPaused(false);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPanel);
	for(auto& button : mButtons)
		window.draw(button);
}

bool PauseState::update(sf::Time deltaTime)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	for(int i=0; i<mButtons.size(); i++){
		mButtons[i].update(deltaTime, mousePos);
	}
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
	for(int i=0; i<mButtons.size(); i++){
		mButtons[i].handleEvent(event, mousePos);
	}
	return false;
}