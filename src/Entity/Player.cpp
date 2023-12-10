#include "include/Player.hpp"
#include "include/CommandQueue.hpp"
#include "include/Witch.hpp"

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
{
    mKeyBinding[sf::Keyboard::A] = moveLeft;
    mKeyBinding[sf::Keyboard::D] = moveRight;
    mKeyBinding[sf::Keyboard::W] = moveUp;
    mKeyBinding[sf::Keyboard::S] = moveDown;
    mMouseBinding[sf::Mouse::Left] = attack;
    mKeyBinding[sf::Keyboard::Space] = attack;
    mKeyBinding[sf::Keyboard::Q] = launchAbility;
    mKeyBinding[sf::Keyboard::E] = launchDebuff;

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

    for(auto &pair : mActionBinding)
    {
        pair.second.category = Category::Player;
    }
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
            return true;
        default:
            return false;
    }
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
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