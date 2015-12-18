#include <time.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <commctrl.h>

using namespace std;

#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetPokerParameters(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND BTN[4], hEdit;

#include "utility.h"
#include "pack.h"
#include "player.h"
#include "game.h"
#include "engine.h"

Windows win;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR uCmdLine, int uCmdShow)
{
    if(!win.Register(hInst))
        return 0;
    if(!win.Create())
        return 0;

    win.Update(uCmdShow);
    win.Loop();

    return win.msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return win.Event(hwnd, msg, wParam, lParam);
}
BOOL CALLBACK SetPokerParameters(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return win.Poker.SetParameters(hwnd, msg, wParam, lParam);
}
