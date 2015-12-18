
void Game::DefaultParameters()
{
    BLIND= 10, UpBLIND= 5, start_money= 5000, blind= 0, bigblind= 1, turn= 2, num_players= 9, mao= 1;
}
BOOL Game::SetParameters(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hwnd, IDC_MONEY, WM_SETTEXT, 0,  (LPARAM)"5000");
        SendDlgItemMessage(hwnd, IDC_UPBLIND, WM_SETTEXT, 0,  (LPARAM)"5");
        SendDlgItemMessage(hwnd, IDC_BLIND, WM_SETTEXT, 0,  (LPARAM)"40");
        SendDlgItemMessage(hwnd, IDC_NUMPLAYERS, WM_SETTEXT, 0,  (LPARAM)"9");
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDOK:
            {
                int X;
                BOOL bSucesse;
                bool ok= true;

                X= GetDlgItemInt(hwnd, IDC_MONEY, &bSucesse, FALSE);;
                if(X > 0)
                    start_money= X;
                else
                {
                    MessageBox(hwnd, "Dinheiro inicial deve ser maior que 0", "Alerta", MB_OK | MB_ICONEXCLAMATION);
                    ok= false;
                }

                X= GetDlgItemInt(hwnd, IDC_BLIND, &bSucesse, FALSE);
                if(start_money > X)
                    BLIND= X;
                else
                {
                    MessageBox(hwnd, "Blind deve ser menor que o dinheiro inicial", "Alerta", MB_OK | MB_ICONEXCLAMATION);
                    ok= false;
                }

                X= GetDlgItemInt(hwnd, IDC_UPBLIND, &bSucesse, FALSE);
                if(X > 0)
                    UpBLIND= X;
                else
                {
                    MessageBox(hwnd, "Blind precisa aumentar", "Alerta", MB_OK | MB_ICONEXCLAMATION);
                    ok= false;
                }

                X= GetDlgItemInt(hwnd, IDC_NUMPLAYERS, &bSucesse, FALSE);
                if(X > 3 && X < 10)
                    num_players= X;
                else
                {
                    MessageBox(hwnd, "Numero de jogadores deve ser entre 4 e 9", "Alerta", MB_OK | MB_ICONEXCLAMATION);
                    ok= false;
                }

                if(ok)
                {
                    int ret= MessageBox(hwnd, "Começar um novo jogo", "Aviso", MB_YESNO | MB_ICONEXCLAMATION);
                    if(ret == IDYES)
                    {
                        blind= 0, bigblind= 1, turn= 2, mao= 1;
                        Inicialize();
                        InitHand();
                        Show();
                    }
                    EndDialog(hwnd, 0);
                }
            }
            break;
        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}
void Game::Inicialize()
{
    string Nomes[]= { "Johnny Moss", "Doyle Brunson", "Amarillo Slim", "Stu Ungar", "Johnny Chan", "Phil Hellmuth",
    "Phil Ivey", "Daniel Negreanu", "Chris Moneymaker", "Alexandre Gomes"};
    jogador= new Player[num_players];

    int pX[]= {20, 20, 20, 180, 330, 480, 630, 630, 630};
    int pY[]= {115, 270, 425, 425, 425, 425, 425, 270, 115};

    for(int i=0; i<num_players; i++)
    {
        jogador[i].posX= pX[i], jogador[i].posY= pY[i];
        jogador[i].human= false, jogador[i].fold= false, jogador[i].loser= false, jogador[i].winner= false, jogador[i].all_in= false;
        jogador[i].money= start_money, jogador[i].name= Nomes[i], jogador[i].action= " ";
    }
    jogador[4].human= true, jogador[4].name= "Vitor Ramos";
}
void Game::Show()
{
    hdc = GetDC(hwnd);

    SetTextColor(hdc, RGB(0,255,0));
    SetBkColor(hdc, RGB(0,0,0));
    SetBkMode(hdc, OPAQUE);

    char buffer[80];

    sprintf(buffer, "                                                                                ");
    TextOut(hdc, 250,100, buffer, strlen(buffer));

    sprintf(buffer, "Mao %d Blind %d Pote %d", mao, blinds, pote);
    TextOut(hdc, 250,100, buffer, strlen(buffer));

    HBRUSH blue_brush= CreateHatchBrush(HS_DIAGCROSS, RGB(0,0,255));
    HBRUSH red_brush= CreateSolidBrush(RGB(255,0,0));
    HBRUSH green_brush= CreateSolidBrush(RGB(0,255,0));
    HBRUSH gray_brush= CreateSolidBrush(RGB(200,200,200));

    RECT quadrado[10]={
        {20 , 10, 140, 110}, // Jogador 1
        {20 , 170, 140, 260}, // Jogador 2
        {20 , 330 , 140, 420}, // Jogador 3
        {180, 330, 300, 420}, // Jogador 4
        {330, 330, 450, 420}, // Humano
        {480, 330, 600, 420}, // Jogador 6
        {630, 330, 750, 420}, // Jogador 7
        {630, 170, 750, 260}, // Jogador 8
        {630, 10 , 750, 110}, // Jogador 9
        {180, 300, 600, 210} // Mesa
    };

    FillRect(hdc, &quadrado[9], blue_brush);
    for(int i=0; i<num_players; i++)
    {
        if(jogador[i].winner)
            FillRect(hdc, &quadrado[i], red_brush);
        else if(turn == i)
            FillRect(hdc, &quadrado[i], green_brush);
        else if(jogador[i].loser || jogador[i].fold)
            FillRect(hdc, &quadrado[i], gray_brush);
        else
            FillRect(hdc, &quadrado[i], blue_brush);
    }

    float x[]= {185, 275, 365, 455, 545};
    for(int i=0; i<5; i++)
    {
        if(table[i] != " ")
            Bitmap(hdc, hInst, x[i], 215, 48, 76, StringToBtn(table[i]));
        else
            Bitmap(hdc, hInst, x[i], 215, 48, 76, IDB_FLIPSIDE);
    }

    for(int i=0; i<num_players; i++)
    {
        if( (show_cards || jogador[i].human ) && !jogador[i].fold)
        {
            Bitmap(hdc, hInst, jogador[i].posX+5, jogador[i].posY-90, 48, 76, StringToBtn(jogador[i].hand[0]));
            Bitmap(hdc, hInst, jogador[i].posX+65, jogador[i].posY-90, 48, 76, StringToBtn(jogador[i].hand[1]));
        }
        else if(!jogador[i].fold || !jogador[i].loser)
        {
            Bitmap(hdc, hInst, jogador[i].posX+5, jogador[i].posY-90, 48, 76, IDB_FLIPSIDE);
            Bitmap(hdc, hInst, jogador[i].posX+65, jogador[i].posY-90, 48, 76, IDB_FLIPSIDE);
        }
    }

    ReleaseDC(hwnd, hdc);

    for(int i=0; i<num_players; i++)
    {
        if(jogador[i].human)
            jogador[i].Show(hdc, hwnd);
        else
            jogador[i].Show(hdc, hwnd);
    }

}
void Game::InitHand()
{
    cont_turn= 0;
    end_turn=frize=show_cards= false;

    ShowWindow(BTN[3], SW_HIDE);

    baralho.Initialize();
    baralho.Scrambles();

    blinds= BLIND*2, pote= 0;

    SendDlgItemMessage(hwnd, 0, WM_SETTEXT, 0, (LPARAM)IntToString(blinds).c_str());

    for(int i=0; i<5; i++)
        table[i]= " ";
    for(int i=0; i<num_players; i++)
    {
        jogador[i].action= " ";
        jogador[i].fold= false, jogador[i].winner= false, jogador[i].blind= false, jogador[i].bigblind= false, jogador[i].all_in= false;
        if(jogador[i].loser)
        {
            jogador[i].hand[0]= "XX", jogador[i].hand[1]= "XX";
        }
        else
        {
            jogador[i].hand[0]= baralho.Card(), jogador[i].hand[1]= baralho.Card();
        }
    }
    jogador[blind].blind= true;
    jogador[bigblind].bigblind= true;
    if(jogador[blind].money <= BLIND)
    {
        pote+=jogador[blind].money;
        jogador[blind].all_in= true;
        jogador[blind].action= "Apostou Tudo "+IntToString(jogador[blind].money);
        jogador[blind].money= 0;
        jogador[blind].blind= false;
    }
    else
    {
        pote+=BLIND;
        jogador[blind].money-=BLIND;
        jogador[blind].action= "Pagou "+IntToString(BLIND);
    }
    if(jogador[bigblind].money <= BLIND*2)
    {
        pote+=jogador[bigblind].money;
        jogador[bigblind].all_in= true;
        jogador[bigblind].action= "Apostou Tudo "+IntToString(jogador[bigblind].money);
        jogador[bigblind].money= 0;
        jogador[bigblind].bigblind= false;
    }
    else
    {
        pote+=BLIND*2;
        jogador[bigblind].money-=BLIND*2;
        jogador[bigblind].action= "Pagou "+IntToString(BLIND*2);
    }
}
void Game::StartHand()
{
    if(cont_turn < num_players)
    {
        int AllComeOut= 0;
        for(int l=0; l<num_players; l++)
            if(jogador[l].fold || jogador[l].loser)
                AllComeOut++;

        if(turn == num_players) turn= 0;

        if(AllComeOut == num_players-1)
        {
            for(int i=0; i<num_players; i++)
                if(!jogador[i].loser && !jogador[i].fold)
                {
                    jogador[i].winner= true;
                    jogador[i].action= "Fez Todos Sairem";
                    jogador[i].money+=pote;
                }
            ShowWindow(BTN[0], SW_HIDE);
            ShowWindow(BTN[1], SW_HIDE);
            ShowWindow(BTN[2], SW_HIDE);
            ShowWindow(BTN[3], SW_SHOW);
            ShowWindow(hEdit, SW_HIDE);
            Show();
            frize=end_turn= true;
            // Parar
        }
        if(!end_turn)
        {
            next= true;
            if(jogador[turn].loser)
                jogador[turn].action= "Perdeu";
            else if(jogador[turn].fold)
                jogador[turn].action= "Desistiu";
            else if(jogador[turn].all_in)
            {
                call= true;
                jogador[turn].action= "Apostou Tudo";
            }
            else
            {
                if(jogador[turn].human)
                    Human();
                else
                {
                    Computer();
                    jogador[turn].blind= false;
                    jogador[turn].bigblind= false;
                }

                if(raise || jogador[turn].all_in)
                {
                    raise= false;
                    cont_turn= 0;

                    jogador[turn].blind= false;
                    jogador[turn].bigblind= false;
                }
            }
            if(next)
            {
                SendDlgItemMessage(hwnd, 0, WM_SETTEXT, 0,  (LPARAM)IntToString(blinds).c_str());
                Show();
                turn++;
                cont_turn++;
            }
        }
    }
    else
    {
        if(call)
        {
            if(table[0] == " ")
                for(int i=0; i<3; i++)
                    table[i]= baralho.Card();
            else
                for(int i=2; i<5; i++)
                    if(table[i] == " ")
                    {
                        table[i]= baralho.Card();
                        break;
                    }
            if(table[4] != " ")
            {
                WinnerLoser();
                ShowWindow(BTN[0], SW_HIDE);
                ShowWindow(BTN[1], SW_HIDE);
                ShowWindow(BTN[2], SW_HIDE);
                ShowWindow(BTN[3], SW_SHOW);
                ShowWindow(hEdit, SW_HIDE);
                end_turn= true;
                Show();
            }
        }
        raise=call= false;
        blinds=cont_turn=0;
    }
}
void Game::Human()
{
//    Show();
    next= false;
    BOOL bSucesse;
    bool l1= false, l2= false;
    int qnt= 0;


    qnt= GetDlgItemInt(hwnd, 0, &bSucesse, FALSE);

    if(jogador[turn].blind)
    {
        if(jogador[turn].money+BLIND <= blinds)
            l1= true;
        else
            l2= true;
    }
    else if(jogador[turn].bigblind)
    {
        if(jogador[turn].money+BLIND*2 <= blinds)
            l1= true;
        else
            l2= true;
    }
    else
    {
        if(jogador[turn].money <= blinds)
            l1= true;
        else
            l2= true;
    }
    if(l1)
    {
        //possible= "1- Aposta Tudo, 4- Desistir";
        ShowWindow(BTN[0], SW_SHOW);
        ShowWindow(BTN[1], SW_HIDE);
        ShowWindow(BTN[2], SW_SHOW);
        ShowWindow(hEdit, SW_HIDE);
        if(op == '1' || op == '4')
            next= true;
    }
    if(l2)
    {
        //possible= "1- Aposta Tudo, 3- Chamar, 4- Desistir";
        // possible= "1- Aposta Tudo, 2- Dobar,  3- Chamar, 4- Desistir";
        ShowWindow(BTN[0], SW_SHOW);
        ShowWindow(BTN[1], SW_SHOW);
        ShowWindow(BTN[2], SW_SHOW);
        ShowWindow(hEdit, SW_SHOW);

        if(op == '3')
        {
            if(qnt == blinds)
            {
                next= true;
            }
            else if(qnt >= blinds*2)
            {
                if(qnt >= jogador[turn].money)
                {
                    MessageBox(NULL, "Você Não Tem Todo Esse Dinheiro", "Aviso", MB_OK | MB_ICONEXCLAMATION);
                    SendDlgItemMessage(hwnd, 0, WM_SETTEXT, 0,  (LPARAM)IntToString(blinds).c_str());
                    op= ' ';
                    next= false;
                }
                else
                {
                    op= '2';
                    next= true;
                }
            }
            else
            {
                MessageBox(NULL, "O valor deve ser no minimo o dobro do blind", "Aviso", MB_OK | MB_ICONEXCLAMATION);
                SendDlgItemMessage(hwnd, 0, WM_SETTEXT, 0,  (LPARAM)IntToString(blinds).c_str());
                op= ' ';
                next= false;
            }
        }
        if(op == '1' || op == '4')
            next= true;
    }
    if(next)
    {
        if(op == '4')
        {
            ShowWindow(BTN[0], SW_HIDE);
            ShowWindow(BTN[1], SW_HIDE);
            ShowWindow(BTN[2], SW_HIDE);
            ShowWindow(BTN[3], SW_HIDE);
            ShowWindow(hEdit, SW_HIDE);
        }
        Processes(op, qnt);
        jogador[turn].blind= false;
        jogador[turn].bigblind= false;
        op= ' ';
    }
}
void Game::Computer()
{
    int valor[2]= {0, 0};
    for(int i=13; i>=0; i--)
    {
        if(jogador[turn].hand[0].at(0) == cards[i])
            valor[0]= i;
        if(jogador[turn].hand[1].at(0) == cards[i])
            valor[1]= i;
    }
    if(blinds == 0)
        Processes('3', 0);
    else if(blinds >= jogador[turn].money)
    {
        if(table[0] != " ")
        {
            string full_hand;
            full_hand= table[0]+" "+table[1]+" "+table[2]+" "+jogador[turn].hand[0]+" "+jogador[turn].hand[1];
            int melhor= Jogada(full_hand);
            if(melhor > 500)
                Processes('1', 0);
            else
                Processes('4', 0);
        }
        else if(jogador[turn].hand[0].at(0) == jogador[turn].hand[1].at(0)) // cartas iguais
        {
            Processes('1', 0);
        }
        else
        {
            if(valor[0] > 10 && valor[1] > 10)
                Processes('1', 0);
            else
                Processes('4', 0);
        }
    }
    else
    {
        if(table[0] != " ")
        {
            string full_hand;
            full_hand= table[0]+" "+table[1]+" "+table[2]+" "+jogador[turn].hand[0]+" "+jogador[turn].hand[1];
            int melhor= Jogada(full_hand);
            if(melhor > 700)
                Processes('1', 0);
            else if(melhor > 500 && jogador[turn].money > blinds*2)
                Processes('2', blinds*2);
            else if(melhor > 200 && jogador[turn].money > blinds)
                Processes('3', 0);
            else
                Processes('4', 0);
        }
        else if(jogador[turn].hand[0].at(0) != jogador[turn].hand[1].at(0)) // cartas diferentes
        {
            if(jogador[turn].hand[0].at(1) == jogador[turn].hand[1].at(1)) // mesmo naipe
            {
                if(valor[0] < 5 && valor[1] < 5 && jogador[turn].money > blinds)
                    Processes('3', 0);
                else if( (valor[0] > 8 || valor[1] > 8) && jogador[turn].money > blinds*2)
                    Processes('2', blinds*2);
                else
                    Processes('4', 0);
            }
            else // naipe diferente
            {
                if(valor[0] < 8 && valor[1] < 8)
                    Processes('4', 0);
                else if(jogador[turn].money > blinds)
                    Processes('3', 0);
                else
                    Processes('4', 0);
            }
        }
        else // cartas iguais
        {
            if(valor[0] < 7 && valor[1] < 7 && jogador[turn].money > blinds)
                Processes('3', 0);
            else if(jogador[turn].money > blinds*2)
                Processes('2', blinds*2);
            else
                Processes('4', 0);
        }
    }
}
void Game::Processes(char op, int qnt)
{
    switch(op)
    {
    case '1': //Apostou Tudo
        {
            jogador[turn].action= "Apostou Tudo "+IntToString(jogador[turn].money);
            if(jogador[turn].money > blinds)
                blinds= jogador[turn].money;
            pote+=jogador[turn].money;
            jogador[turn].money= 0;
            jogador[turn].all_in= true;
        }
        break;
    case '2': // Aumentar
        {
            raise= true;
            if(jogador[turn].blind)
            {
                pote+= (qnt-BLIND);
                blinds= qnt;
                jogador[turn].money-= (qnt-BLIND);
                jogador[turn].action= "Aumentou "+IntToString(qnt-BLIND);
            }
            else if(jogador[turn].bigblind)
            {
                pote+= (qnt-(BLIND*2));
                blinds= qnt;
                jogador[turn].money-= (qnt-(BLIND*2));
                jogador[turn].action= "Aumentou "+IntToString(qnt-(BLIND*2) );
            }
            else
            {
                pote+=qnt;
                blinds= qnt;
                jogador[turn].money-=qnt;
                jogador[turn].action= "Aumentou "+IntToString(qnt);
            }
        }
        break;
    case '3': // Chamar
        {
            call= true;
            if(jogador[turn].blind)
            {
                pote+= (blinds-BLIND);
                jogador[turn].money-= (blinds-BLIND);
                jogador[turn].action= "Chamou "+IntToString(blinds-BLIND);
            }
            else if(jogador[turn].bigblind)
            {
                pote+= (blinds-(BLIND*2));
                jogador[turn].money-= (blinds-(BLIND*2));
                jogador[turn].action= "Chamou "+IntToString(blinds- (BLIND*2) );
            }
            else
            {
                pote+=blinds;
                jogador[turn].money-=blinds;
                jogador[turn].action= "Chamou "+IntToString(blinds);
            }
        }
        break;
    case '4':
        {
            jogador[turn].fold= true;
            jogador[turn].action= "Desistiu ";
        }
        break;
    }
}
void Game::WinnerLoser()
{
    show_cards= true;
    int best_play_i[num_players];
    string best_play_s[num_players];
    for(int i=0; i<num_players; i++)
    {
        best_play_i[i]= 0;
        if(!jogador[i].fold && !jogador[i].loser)
        {
            string full_hand[7];
            for(int x=0; x<5; x++)
            {
                full_hand[x]= table[x];
            }
            full_hand[5]= jogador[i].hand[0];
            full_hand[6]= jogador[i].hand[1];
            for(int p1=0; p1<7; p1++)
            {
                for(int p2=0; p2<7 && p2 != p1; p2++)
                {
                    for(int p3=0; p3<7 && p3 != p2 && p3 != p1; p3++)
                    {
                        for(int p4=0; p4<7 && p4 != p1 && p4 != p2 && p4 != p3; p4++)
                        {
                            for(int p5=0; p5<7 && p5 != p4 && p5 != p3 && p5 != p2 && p5 != p1; p5++)
                            {
                                string jogada= " ";
                                if(   jogador[i].hand[0] == full_hand[p1] || jogador[i].hand[0] == full_hand[p2]
                                   || jogador[i].hand[0] == full_hand[p3] || jogador[i].hand[0] == full_hand[p4]
                                   || jogador[i].hand[0] == full_hand[p5] || jogador[i].hand[1] == full_hand[p1]
                                   || jogador[i].hand[1] == full_hand[p2] || jogador[i].hand[1] == full_hand[p3]
                                   || jogador[i].hand[1] == full_hand[p4] || jogador[i].hand[1] == full_hand[p5]
                                   )
                                {
                                    jogada= full_hand[p1]+" "+full_hand[p2]+" "+full_hand[p3]+" "+full_hand[p4]+" "+full_hand[p5];

                                    if(Jogada(jogada) > best_play_i[i])
                                    {
                                        best_play_i[i]= Jogada(jogada);
                                        best_play_s[i]= jogada;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    int best= 0;
    for(int i=0; i<num_players; i++)
    {
        if(best_play_i[i] > best)
            best= best_play_i[i];

        if(best_play_i[i] > 99 && best_play_i[i] < 199)
            jogador[i].action= "Carta Alta";
        else if(best_play_i[i] > 199 && best_play_i[i] < 299)
            jogador[i].action= "Um Par";
        else if(best_play_i[i] > 299 && best_play_i[i] < 399)
            jogador[i].action= "Dois Pares";
        else if(best_play_i[i] > 399 && best_play_i[i] < 499)
            jogador[i].action= "Trinca ou Trio";
        else if(best_play_i[i] > 499 && best_play_i[i] < 599)
            jogador[i].action= "Sequencia";
        else if(best_play_i[i] > 599 && best_play_i[i] < 699)
            jogador[i].action= "Flush";
        else if(best_play_i[i] > 699 && best_play_i[i] < 799)
            jogador[i].action= "FullHouse";
        else if(best_play_i[i] > 799 && best_play_i[i] < 899)
            jogador[i].action= "Quadra";
        else if(best_play_i[i] > 899 && best_play_i[i] < 999)
            jogador[i].action= "Sequencia do mesmo nipe";
        else if(best_play_i[i] == 1000)
            jogador[i].action= "Sequencia Real";
    }
    int num_winner= 0, *winners;
    for(int i=0; i<num_players; i++)
    {
        if(best_play_i[i] == best)
            num_winner++;
    }
    winners= new int[num_winner];
    for(int i=0, cont= 0; i<num_players; i++)
    {
        if(best_play_i[i] == best)
        {
            winners[cont]= i;
            cont++;
        }
    }
    for(int i=0; i<num_players; i++)
    {
        for(int j=0; j<num_winner; j++)
        {
            if(i == winners[j])
            {
                jogador[i].money+=(pote/num_winner);
                jogador[i].winner= true;
            }
        }
    }
    for(int i=0; i<num_players; i++)
    {
        bool winner= false;
        for(int j= 0; j<num_winner; j++)
            if(i == winners[j])
                winner= true;
        if(jogador[i].all_in && !winner)
        {
            jogador[i].loser= true;
            jogador[i].action= "Perdeu";
        }
    }
    frize= true;
}
bool Game::NextHand()
{
    int AllComeOut= 0;
    for(int i=0; i<num_players; i++)
        if(jogador[i].loser)
            AllComeOut++;

    if(AllComeOut >= num_players-1)
    {
        for(int i=0; i<num_players; i++)
            if(!jogador[i].loser)
                jogador[i].action= "Vencedor";
        Show();
        return false;
    }
    blind++;
    bigblind++;
    if(blind >= num_players) blind= 0;
    if(bigblind >= num_players) bigblind= 0;
    while(jogador[blind].loser)
    {
        blind++;
        if(blind >= num_players) blind= 0;
    }
    while(jogador[bigblind].loser || bigblind == blind)
    {
        bigblind++;
        if(bigblind >= num_players) bigblind= 0;
    }

    turn= bigblind+1;
    if(turn >= num_players) turn= 0;
    while(jogador[turn].loser)
    {
        turn++;
        if(turn >= num_players) turn= 0;
    }

    mao++;
    if(mao%UpBLIND == 0) BLIND*=2;
    return true;
}
