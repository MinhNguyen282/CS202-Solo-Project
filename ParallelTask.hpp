#ifndef PARALLEL_TASK_HPP
#define PARALLEL_TASK_HPP

#include <SFML/System.hpp>

class ParallelTask
{
    public:
        ParallelTask();
        void execute();
        bool isFinished();
        float getCompletion();
    private:
        void runTask();
    private:
        sf::Thread mThread;
        bool mFinished;
        sf::Mutex mMutex;
        sf::Clock mElapsedTime;
};

#endif // PARALLEL_TASK_HPP