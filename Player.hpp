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
        void assignKey(Action action, sf::Keyboard::Key key);
        sf::Keyboard::Key getAssignedKey(Action action) const;
    private:
        static bool isRealtimeAction(Action action);
    public:
        void handleEvent(const sf::Event& event, CommandQueue& commands);
        void handleRealtimeInput(CommandQueue& commands);
    private:
        std::map<sf::Keyboard::Key, Action> mKeyBinding;
        std::map<sf::Mouse::Button, Action> mMouseBinding;
        std::map<Action, Command> mActionBinding;
};

#endif // PLAYER_HPP