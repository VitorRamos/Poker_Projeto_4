#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

struct Player
{
    int posX, posY;
    bool fold, loser, winner, blind, bigblind, all_in, human;
    int money;
    string name, action, hand[2];

    void Show(HDC, HWND);
};

#include "player.cpp"

#endif // PLAYER_H_INCLUDED
