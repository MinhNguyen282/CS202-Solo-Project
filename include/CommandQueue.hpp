#ifndef COMMAND_QUEUE_HPP
#define COMMAND_QUEUE_HPP

#include "Command.hpp"

#include <queue>

class CommandQueue
{
    public:
        void push(const Command& command);
        Command pop();
        bool isEmpty() const;
        int size() const;
    private:
        std::queue<Command> mQueue;
};

#endif // COMMAND_QUEUE_HPP