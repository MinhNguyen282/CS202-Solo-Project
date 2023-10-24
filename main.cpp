#include <bits/stdc++.h>
#include "Application.hpp"

using namespace std;

int main()
{
    try
    {
        Application app;
        app.run();
    }
    catch (std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << '\n';
    }
    return 0;
}