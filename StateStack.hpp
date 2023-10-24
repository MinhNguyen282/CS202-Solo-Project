#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <functional>
#include <cassert>
#include "State.hpp"
#include "StateIdentifiers.hpp"

namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
    public:
        enum Action
        {
            Push,
            Pop,
            Clear,
        };
    public:
        explicit StateStack(State::Context context);
        template<typename T>
        void registerState(States::ID stateID);
        void update(sf::Time dt);
        void draw();
        void handleEvent(const sf::Event& event);

        void pushState(States::ID stateID);
        void popState();
        void clearStates();

        bool isEmpty() const;

    private:
        State::Ptr createState(States::ID stateID);
        void applyPendingChange();

    private:
        struct PendingChange
        {
            explicit PendingChange(Action action, States::ID stateID = States::None);

            Action action;
            States::ID stateID;
        };

    private:
        std::vector<State::Ptr> mStack;
        std::vector<PendingChange> mPendingList;
        State::Context mContext;
        std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template<typename T>
void StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this] ()
    {
        return State::Ptr(new T(*this, mContext));
    };
}

#endif // STATESTACK_HPP