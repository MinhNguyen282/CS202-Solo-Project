#include <iostream>
#include "include/Application.hpp"

using namespace std;

int main()
{
    try
    {
        cout << "Minh dep trai! \n";
        Application app;
        app.run();
    }
    catch (std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << '\n';
    }
    return 0;
}