#include "Game.hpp"

sf::Time TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: mWindow(sf::VideoMode(1200, 720), "Little Witch", sf::Style::Close)
, mWorld(mWindow)
{

}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            if (!mIsPaused) update(TimePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    CommandQueue& commands = mWorld.getCommandQueue();

    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mPlayer.handleEvent(event, commands);

        if (event.type == sf::Event::GainedFocus)
        {
            mIsPaused = false;
        }
        else if (event.type == sf::Event::LostFocus)
        {
            mIsPaused = true;
        }
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
    mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time deltaTime)
{
    mWorld.update(deltaTime);
}

void Game::render()
{
    mWindow.clear(sf::Color::White);
    mWorld.draw();
    
    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}
