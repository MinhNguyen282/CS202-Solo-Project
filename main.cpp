#include <iostream>
#include "include/Application.hpp"
#include <Windows.h>

using namespace std;

int main()
{
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
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