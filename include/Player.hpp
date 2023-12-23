#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"
#include "CommandQueue.hpp"
#include "Witch.hpp"
#include "Enemy.hpp"
#include "DarkWizzard.hpp"
#include "DataTables.hpp"

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Player
{
    public:
        Player();
        enum Action
        {
            moveLeft,
            moveRight,
            moveUp,
            moveDown,
            launchAbility,
            launchDebuff,
            launchUltimate,
            attack,
            actionCount,
        };
        enum MissionStatus
        {
            MissionRunning,
            MissionFailure,
            MissionSuccess,
        };
        void assignKey(Action action, sf::Keyboard::Key key);
        sf::Keyboard::Key getAssignedKey(Action action) const;
    private:
        static bool isRealtimeAction(Action action);
        void initializeActions();
        void writeToFile();
    public:
        void handleEvent(const sf::Event& event, CommandQueue& commands);
        void handleRealtimeInput(CommandQueue& commands, sf::Time deltaTime);
        void setMissionStatus(MissionStatus status);
        MissionStatus getMissionStatus() const;

        void setUpgradePoints(int points);
        int getUpgradePoints() const;

        void setInvicible(bool invicible);
        bool isInvicible() const;

        void addPlayedTime(sf::Time playedTime);
        void subtractPlayedTime(sf::Time playedTime);
        sf::Time getPlayedTime() const;

        void setScore(int score);
        int getScore()  const;

        void setTable(std::vector<WitchData> table);
        WitchData getTable() const;

        void setCurrentHitpoints(int hitpoints);
        int getCurrentHitpoints() const;

        void setLevel(int level);
        int getLevel() const;

        void setExp(int exp);
        int getExp() const;

        void saveInfomation();
        void readInfomation();

        bool isContinue;
        std::string mCommand;
    private:
        std::map<sf::Keyboard::Key, Action> mKeyBinding;
        std::map<sf::Mouse::Button, Action> mMouseBinding;
        std::map<Action, Command> mActionBinding;
        std::map<std::string, Action> mStringToAction;
        std::map<Action, std::string> mActionToString;
        MissionStatus mCurrentMissionStatus;

        sf::Time mPlayedTime;
        int mScore;
        int mCurrentHitpoints;
        WitchData Table;
        int mLevel;
        int mExp;
        int mUpgradePoints;
        bool mInvicible;
};

#endif // PLAYER_HPP