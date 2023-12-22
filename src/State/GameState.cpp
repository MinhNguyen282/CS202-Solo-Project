#include "include/GameState.hpp"
#include "include/MusicPlayer.hpp"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.fonts, *context.sounds)
, mPlayer(*context.player)
{
    mPlayer.subtractPlayedTime(mPlayer.getPlayedTime());
    mPlayer.setInvicible(false);
    context.music->play(Music::Before5MinsTheme);
    mMusicIndex = 0;
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
    
    // Cheat mode if press Ctrl + Shift + C
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        requestStackPush(States::Cheat);
    }

   return true;
}

bool GameState::update(sf::Time deltaTime)
{

    //set invicible
    if (mWorld.getInvicible() != mPlayer.isInvicible())
    {
        mWorld.setInvicible(mPlayer.isInvicible());
    }

    //set played time
    if (mWorld.getPlayedTime() != mPlayer.getPlayedTime())
    {
        mWorld.setPlayedTime(mPlayer.getPlayedTime());
    }

    //set music
    if (mWorld.getPlayedTime() >= sf::seconds(15*60) && mMusicIndex==2){
        mMusicIndex++;
        getContext().music->play(Music::BossTheme);
    }
    else if (mWorld.getPlayedTime() >= sf::seconds(10*60) && mMusicIndex==1) {
        mMusicIndex++;
        getContext().music->play(Music::After10MinsTheme);
    }
    else if (mWorld.getPlayedTime() >= sf::seconds(5*60) && mMusicIndex==0)
    {
        mMusicIndex++;
        getContext().music->play(Music::After5MinsTheme);
    } 
    
    mWorld.update(deltaTime);

    if (!mWorld.hasAlivePlayer())
    {
        std::cout << "Fail!\n";
        mPlayer.setMissionStatus(Player::MissionFailure);
        
        // save upgrade points
        int mUpgradePoints;
        std::ifstream ifs("Data/Upgrade/mUpgradePoints.dat");
        ifs >> mUpgradePoints;
        ifs.close();
        mUpgradePoints += mWorld.getScore();
        std::ofstream ofs("Data/Upgrade/mUpgradePoints.dat");
        ofs << mUpgradePoints;
        ofs.close();

        mPlayer.setUpgradePoints(mWorld.getScore());
        
        requestStackPush(States::GameOver);
    }
    else if (mWorld.isBossDefeated())
    {
        std::cout << "Win!\n";
        mPlayer.setMissionStatus(Player::MissionSuccess);

        // save upgrade points
        int mUpgradePoints;
        std::ifstream ifs("Data/Upgrade/mUpgradePoints.dat");
        ifs >> mUpgradePoints;
        ifs.close();
        mUpgradePoints += mWorld.getScore();
        std::ofstream ofs("Data/Upgrade/mUpgradePoints.dat");
        ofs << mUpgradePoints;
        ofs.close();

        mPlayer.setUpgradePoints(mWorld.getScore());

        requestStackPush(States::GameOver);
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands, deltaTime);
    return true;
}

void GameState::draw()
{
    mWorld.draw();
}
