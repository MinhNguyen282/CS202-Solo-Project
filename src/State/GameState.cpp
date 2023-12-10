#include "include/GameState.hpp"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.fonts)
, mPlayer(*context.player)
{

}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = mWorld.getCommandQueue();

    // get mouse position
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*getContext().window);
    mWorld.setMousePosition(mousePosition);

    mPlayer.handleEvent(event, commands);

    // Escape pressed, trigger the pause screen
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        requestStackPush(States::Pause);
    
   return true;
}

bool GameState::update(sf::Time deltaTime)
{
    mWorld.update(deltaTime);

    if (!mWorld.hasAlivePlayer())
    {
        std::cout << "Fail!\n";
        mPlayer.setMissionStatus(Player::MissionFailure);
        requestStackPush(States::GameOver);
    }
    else if (mWorld.isBossDefeated())
    {
        std::cout << "Win!\n";
        mPlayer.setMissionStatus(Player::MissionSuccess);
        requestStackPush(States::GameOver);
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);
    return true;
}

void GameState::draw()
{
    mWorld.draw();
}
