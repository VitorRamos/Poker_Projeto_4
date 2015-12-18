#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

class Game
{
    // Classe Game
    bool next, show_cards, call, raise;
    int blinds, pote, BLIND, UpBLIND, start_money, blind, bigblind, mao, num_players, turn, cont_turn;
    string table[5];

    // Outras Classes
    Player* jogador;
    Pack baralho;

    // Necessário para Show()
    HDC hdc;
    HINSTANCE hInst;
    HWND hwnd;
public:

    bool frize, end_turn;
    char op;

    // Inicialização
    void DefaultParameters();
    BOOL SetParameters(HWND, UINT, WPARAM, LPARAM);
    void Inicialize();

    // Loop
    void InitHand();
    void StartHand();
    bool NextHand();

    // Display
    void IntPaint(HDC HdC, HINSTANCE HInst, HWND HWnd) { hdc= HdC; hInst= HInst; hwnd= HWnd; }
    void Show();

    // Processamentos
    void Human();
    void Computer();
    void Processes(char, int);
    void WinnerLoser();
};

#include "game.cpp"

#endif // GAME_H_INCLUDED
