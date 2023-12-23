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

void Player::setScore(int score)
{
    mScore = score;
}

int Player::getScore() const
{
    return mScore;
}

void Player::setTable(std::vector<WitchData> table)
{
    Table = table[Witch::BlueWitch];
}

WitchData Player::getTable() const
{
    return Table;
}

void Player::setCurrentHitpoints(int hitpoints)
{
    mCurrentHitpoints = hitpoints;
}

int Player::getCurrentHitpoints() const
{
    return mCurrentHitpoints;
}

void Player::setLevel(int level)
{
    mLevel = level;
}

int Player::getLevel() const
{
    return mLevel;
}

void Player::setExp(int exp)
{
    mExp = exp;
}

int Player::getExp() const
{
    return mExp;
}

void Player::saveInfomation()
{
    std::ofstream ofs("Data/Save/sav.dat");
    ofs << mPlayedTime.asSeconds() << std::endl;
    ofs << mScore << std::endl;
    ofs << mCurrentHitpoints << std::endl;
    ofs << mLevel << std::endl;
    ofs << mExp << std::endl;
    ofs.close();
    ofs.open("Data/Save/savTable.dat");
    ofs << Table.hitpoints << std::endl;
    ofs << Table.speed << std::endl;
    ofs << Table.fireInterval.asSeconds() << std::endl;
    ofs << Table.abilityInterval.asSeconds() << std::endl;
    ofs << Table.debuffInterval.asSeconds() << std::endl;
    ofs << Table.ultimateInterval.asSeconds() << std::endl;
    ofs << Table.mCoolDown << std::endl;
    ofs.close();
}

void Player::readInfomation(){
    std::ifstream ifs("Data/Save/sav.dat");
    if (ifs.is_open()){
        float playedTime;
        ifs >> playedTime;
        mPlayedTime = sf::seconds(playedTime);
        ifs >> mScore;
        ifs >> mCurrentHitpoints;
        ifs >> mLevel;
        ifs >> mExp;
        ifs.close();
    }
    else{
        std::cout << "Error: Unable to open file Data/Save/sav.dat" << std::endl;
    }
    std::ifstream ifs2("Data/Save/savTable.dat");
    WitchData witchData;
    if (ifs2.is_open()){
        ifs2 >> witchData.hitpoints;
        ifs2 >> witchData.speed;
        float x;
        ifs2 >> x;
        witchData.fireInterval = sf::seconds(x);
        ifs2 >> x;
        witchData.abilityInterval = sf::seconds(x);
        ifs2 >> x;
        witchData.debuffInterval = sf::seconds(x);
        ifs2 >> x;
        witchData.ultimateInterval = sf::seconds(x);
        ifs2 >> witchData.mCoolDown;
        ifs2.close();
    }
    else{
        std::cout << "Error: Unable to open file Data/Save/savTable.dat" << std::endl;
    }
    Table = witchData;
}