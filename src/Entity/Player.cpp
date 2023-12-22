#include "include/Player.hpp"
#include "include/CommandQueue.hpp"
#include "include/Witch.hpp"
#include "include/Utility.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

struct WitchMover
{
    sf::Vector2f velocity;
    WitchMover(float vx, float vy)
    : velocity(vx, vy)
    {
        
    }
    void operator () (Witch& witch, sf::Time) const
    {
        witch.accelerate(velocity * witch.getMaxSpeed());
        witch.setAnimation(Witch::Animation::Walk);
    }
};

Player::Player()
: mPlayedTime(sf::Time::Zero)
{
    mInvicible = false;
    initializeActions();

    mActionBinding[moveLeft].action = derivedAction<Witch>(WitchMover(-1.f, 0.f));
    mActionBinding[moveRight].action = derivedAction<Witch>(WitchMover(1.f, 0.f));
    mActionBinding[moveUp].action = derivedAction<Witch>(WitchMover(0.f, -1.f));
    mActionBinding[moveDown].action = derivedAction<Witch>(WitchMover(0.f, 1.f));
    mActionBinding[attack].action = derivedAction<Witch>([] (Witch& w, sf::Time) {
        w.fire();
    });
    mActionBinding[launchAbility].action = derivedAction<Witch>([] (Witch& w, sf::Time) {
        w.launchAbility();
    });
    mActionBinding[launchDebuff].action = derivedAction<Witch>([] (Witch& w, sf::Time) {
        w.laundDebuff();
    });
    mActionBinding[launchUltimate].action = derivedAction<Witch>([] (Witch& w, sf::Time) {
        w.launchUltimate();
    });

    for(auto &pair : mActionBinding)
    {
        pair.second.category = Category::Player;
    }
}

void Player::setInvicible(bool invicible)
{
    mInvicible = invicible;
}

bool Player::isInvicible() const
{
    return mInvicible;
}

void Player::initializeActions()
{
    mStringToAction["moveLeft"] = moveLeft;
    mStringToAction["moveRight"] = moveRight;
    mStringToAction["moveUp"] = moveUp;
    mStringToAction["moveDown"] = moveDown;
    mStringToAction["attack"] = attack;
    mStringToAction["launchAbility"] = launchAbility;
    mStringToAction["launchDebuff"] = launchDebuff;
    mStringToAction["launchUltimate"] = launchUltimate;

    mActionToString[moveLeft] = "moveLeft";
    mActionToString[moveRight] = "moveRight";
    mActionToString[moveUp] = "moveUp";
    mActionToString[moveDown] = "moveDown";
    mActionToString[attack] = "attack";
    mActionToString[launchAbility] = "launchAbility";
    mActionToString[launchDebuff] = "launchDebuff";
    mActionToString[launchUltimate] = "launchUltimate";

    std::ifstream ifs("Data/Settings.dat");
    if (ifs.is_open())
    {
        std::string key;
        std::string action;
        while(ifs >> action >> key)
        {
            sf::Keyboard::Key keyBind = toKeyboardKey(key);
            Action actionBind = mStringToAction[action];
            assignKey(actionBind, keyBind);
        }
    }
    else
    {
        std::cout << "Error: Unable to open file Data/Settings.dat" << std::endl;
    }
    ifs.close();
    mMouseBinding[sf::Mouse::Left] = attack;
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
    for(auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
    {
        if (itr->second == action)
        {
            mKeyBinding.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
    mKeyBinding[key] = action;
    writeToFile();
}

void Player::writeToFile()
{
    std::ofstream ofs("Data/Settings.dat");
    if (ofs.is_open())
    {
        for(auto pair : mKeyBinding)
        {
            ofs << mActionToString[pair.second] << " " << toString(pair.first) << std::endl;
        }
    }
    else
    {
        std::cout << "Error: Unable to open file Data/Settings.dat" << std::endl;
    }
    ofs.close();
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
    for(auto pair : mKeyBinding)
    {
        if (pair.second == action)
        {
            return pair.first;
        }
    }
    return sf::Keyboard::Unknown;
}

bool Player::isRealtimeAction(Action action)
{
    switch(action)
    {
        case moveLeft:
        case moveRight:
        case moveUp:
        case moveDown:
        case attack:
        case launchAbility:
        case launchDebuff:
        case launchUltimate:
            return true;
        default:
            return false;
    }
}

void Player::handleRealtimeInput(CommandQueue& commands, sf::Time deltaTime)
{
    mPlayedTime += deltaTime;
    for(auto pair : mKeyBinding)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
        {
            commands.push(mActionBinding[pair.second]);
        }
    }
    for(auto pair : mMouseBinding)
    {
        if (sf::Mouse::isButtonPressed(pair.first) && isRealtimeAction(pair.second))
        {
            commands.push(mActionBinding[pair.second]);
        }
    }
}

void Player::setMissionStatus(MissionStatus status)
{
    mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
    return mCurrentMissionStatus;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{

}

void Player::setUpgradePoints(int points)
{
    mUpgradePoints = points;
}

int Player::getUpgradePoints() const
{
    return mUpgradePoints;
}

void Player::addPlayedTime(sf::Time playedTime)
{
    std::cout << "addPlayedTime\n";
    mPlayedTime += playedTime;
}

void Player::subtractPlayedTime(sf::Time playedTime)
{
    mPlayedTime -= playedTime;
}

sf::Time Player::getPlayedTime() const
{
    return mPlayedTime;
}