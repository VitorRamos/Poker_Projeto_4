#ifndef PACK_H_INCLUDED
#define PACK_H_INCLUDED

class Pack
{
    string card[52];
    int distCard;
public:
    void Initialize();
    void Scrambles();
    string Card();
};

#include "pack.cpp"

#endif // PACK_H_INCLUDED
