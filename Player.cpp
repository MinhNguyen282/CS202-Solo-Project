#include "Player.hpp"
#include "CommandQueue.hpp"
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
        witch.accelerate(velocity);
        witch.setAnimation(Witch::Animation::Walk);
    }
};

Player::Player()
{
    mKeyBinding[sf::Keyboard::Left] = moveLeft;
    mKeyBinding[sf::Keyboard::Right] = moveRight;
    mKeyBinding[sf::Keyboard::Up] = moveUp;
    mKeyBinding[sf::Keyboard::Down] = moveDown;
    mKeyBinding[sf::Keyboard::Space] = attack;

    mActionBinding[moveLeft].action = derivedAction<Witch>(WitchMover(-200.f, 0.f));
    mActionBinding[moveRight].action = derivedAction<Witch>(WitchMover(200.f, 0.f));
    mActionBinding[moveUp].action = derivedAction<Witch>(WitchMover(0.f, -200.f));
    mActionBinding[moveDown].action = derivedAction<Witch>(WitchMover(0.f, 200.f));
    mActionBinding[attack].action = derivedAction<Witch>([] (Witch& w, sf::Time) { w.setAnimation(Witch::Animation::Attack); });

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
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyReleased)
    {
        Command switchIdle;
        switchIdle.category = Category::Player;
        switchIdle.action = derivedAction<Witch>([] (Witch& w, sf::Time) { if (!w.isAttack()) w.setAnimation(Witch::Animation::Idle); });
        commands.push(switchIdle);
    }
}