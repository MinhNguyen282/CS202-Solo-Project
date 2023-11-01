#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"
#include "CommandQueue.hpp"
#include "Witch.hpp"
#include <map>

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
            charge,
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
    public:
        void handleEvent(const sf::Event& event, CommandQueue& commands);
        void handleRealtimeInput(CommandQueue& commands);
        void setMissionStatus(MissionStatus status);
        MissionStatus getMissionStatus() const;
        sf::Vector2f getWorldPosition() const;
    private:
        std::map<sf::Keyboard::Key, Action> mKeyBinding;
        std::map<sf::Mouse::Button, Action> mMouseBinding;
        std::map<Action, Command> mActionBinding;
        MissionStatus mCurrentMissionStatus;
};

#endif // PLAYER_HPP