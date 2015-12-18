#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#define Cpoker "Real Poker"

class Windows
{
    // Windows
    WNDCLASSEX wc;
    HWND hwnd;
    HINSTANCE hInstance;

    // Impressão
    PAINTSTRUCT	ps;
    HDC		hdc;

public:
    // Game
    Game Poker;

    // Windows
    MSG msg;

    // Windows
    bool Register(HINSTANCE);
    bool Create();
    void Update(int);
    LRESULT Event(HWND, UINT, WPARAM, LPARAM);
    void Loop();
};

#include "engine.cpp"

#endif // ENGINE_H_INCLUDED
