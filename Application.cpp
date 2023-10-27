#include "Application.hpp"  
#include "Utility.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "GameState.hpp"
#include "LoadingState.hpp"
#include "SettingState.hpp"
#include "CreditState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
: mWindow(sf::VideoMode(1200, 720), "Little Witch", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);

    mFonts.load(Fonts::Main, "Media/buddychampion.ttf");
    mTextures.load(Textures::Title, "Media/Textures/TitleScreen.png");
    mTextures.load(Textures::ButtonNormal, "Media/Textures/ButtonNormal.png");
    mTextures.load(Textures::ButtonSelected, "Media/Textures/ButtonSelected.png");
    mTextures.load(Textures::ButtonPressed, "Media/Textures/ButtonPressed.png");
    mTextures.load(Textures::smallButtonNormal, "Media/Textures/smallButtonNormal.png");
    mTextures.load(Textures::smallButtonSelected, "Media/Textures/smallButtonSelected.png");
    mTextures.load(Textures::smallButtonPressed, "Media/Textures/smallButtonPressed.png");
    mTextures.load(Textures::Panel, "Media/Textures/Panel.png");
    mTextures.load(Textures::bigPanel, "Media/Textures/bigPanel.png");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);

    registerStates();
    mStateStack.pushState(States::Title);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            if (mStateStack.isEmpty())
                mWindow.close();
        }
        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    
    mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
    mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<CreditState>(States::Credit);
    mStateStack.registerState<SettingState>(States::Setting);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<LoadingState>(States::Loading);
    mStateStack.registerState<PauseState>(States::Pause);
}