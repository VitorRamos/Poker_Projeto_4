#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

void SetDefaultFont( HWND hWnd, int identifier )
{
    SendDlgItemMessage(hWnd, identifier, WM_SETFONT,
        (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
}

HWND CreateButtonEx( HWND hWnd, HINSTANCE hInstance, int identifier, int x, int y, int width, int height, const char* tempText, unsigned long WS_CUSTOM)
{
    HWND hButtonTemp;

    hButtonTemp = CreateWindowEx(0, "BUTTON", tempText,
        WS_CHILD | WS_CUSTOM | WS_VISIBLE, x, y, width, height, hWnd,
        (HMENU)identifier, hInstance, NULL);
    SetDefaultFont(hWnd, identifier);
    return hButtonTemp;
}

void Bitmap(HDC hdc, HINSTANCE hInstance, int posX, int posY, int width, int heigth, int IDD)
{
    HDC TempDC;
    HBITMAP bmp;

    bmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDD));
    TempDC = CreateCompatibleDC(hdc);
    SelectObject(TempDC, bmp);

    BitBlt(hdc, posX, posY, width, heigth, TempDC, 0, 0, SRCCOPY);

    DeleteDC(TempDC);
    DeleteObject(bmp);
}

const char cards[]= {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

int HighCard(string mao) // cartão de valor mais alto.
{
    for(int i=13; i>=0; i--)
        if(mao.find(cards[i])!=string::npos)
            return i;
    return false;
}
int OnePar(string mao) // Duas cartas do mesmo valor.
{
    for(int i=13; i>=0; i--)
    {
        if(mao.find(cards[i])!=string::npos)
        {
            mao.replace(mao.find(cards[i]), 1, "");
            if(mao.find(cards[i])!=string::npos)
            {
                mao.replace(mao.find(cards[i]), 1, "");
                if(mao.find(cards[i])==string::npos) return i+2;
            }
        }
    }
    return false;
}
int TowPar(string mao) // Dois pares diferentes.
{
    int cont=0, par1=0, par2=0;
    for(int i=13; i>=0; i--)
    {
        if(mao.find(cards[i])!=string::npos)
        {
            mao.replace(mao.find(cards[i]), 1, "");
            if(mao.find(cards[i])!=string::npos)
            {
                if(cont == 0) par1= i;
                if(cont == 1) par2= i;
                mao.replace(mao.find(cards[i]), 1, "");
                if(mao.find(cards[i])==string::npos)
                    cont++;
            }
        }
    }
    if(par1 != par2) return (cont == 2) ? (par1+par2+4) : (0);
    return false;
}
int ThreeofaKind(string mao) // Três cartas do mesmo valor
{
    for(int i=13; i>=0; i--)
    {
        int cont= 0;
        while(mao.find(cards[i])!=string::npos)
        {
            mao.replace(mao.find(cards[i]), 1, "");
            cont++;
        }
        if(cont == 3)
            return i+2;
    }
    return false;
}
bool Hetero(string mao) // Todos os cartões são valores consecutivos.
{
    bool suc= true;
    int valor[5], p=0;
    for(unsigned int i=0; i<mao.length(); i++)
    {
        for(int j=0; j<13; j++)
        {
            if(mao.at(i) == cards[j])
            {
                valor[p]= j+2;
                p++;
                break;
            }
        }
    }
    for(int i=0; i<p; i++)
    {
        for(int j=i; j<p; j++)
        {
            if(valor[i]>valor[j])
            {
                int aux= valor[i];
                valor[i]= valor[j];
                valor[j]= aux;
            }
        }
    }
    int teste= valor[0];
    for(int i=1; i<p; i++)
    {
        teste++;
        if(teste!=valor[i]) suc= false;
    }
    return suc;
}
bool Rente(string mao) // Todos os cartões do mesmo naipe.
{
    for(unsigned int i=4; i<mao.length(); i+=3)
    {
        if(mao.at(i)!=mao.at(1)) return false;
    }
    return true;
}
bool FullHouse(string mao) // Três de um tipo e um par.
{
    if(ThreeofaKind(mao) !=0 && OnePar(mao) != 0)
    {
        return true;
    }
    return false;
}
int FourofaKind(string mao) // Quatro cartas de mesmo valor.
{
    for(int i=13; i>=0; i--)
    {
        int cont= 0;
        while(mao.find(cards[i])!=string::npos)
        {
            mao.replace(mao.find(cards[i]), 1, "");
            cont++;
        }
        if(cont == 4)
            return i+2;
    }
    return false;
}
bool StraightFlush(string mao) // Todos os cartões são valores consecutivos de mesmo naipe.
{
    if(Rente((mao)) && Hetero(mao)) return true;
    return false;
}
bool RoyalFlush(string mao) // Dez, Valete, Rainha, Rei, Ás, no mesmo naipe.
{
    if(Rente((mao)) && mao.find('T')!=string::npos
       && mao.find('J')!=string::npos && mao.find('Q')!=string::npos
       && mao.find('K')!=string::npos && mao.find('A')!=string::npos)
        return true;
    return false;
}
int Jogada(string mao)
{
    // Melhorar
    if(RoyalFlush(mao))
        return 1000;
    else if(StraightFlush(mao))
        return 900;
    else if(FourofaKind(mao))
        return 800+FourofaKind(mao);
    else if(FullHouse(mao))
        return 700+ThreeofaKind(mao)+OnePar(mao);
    else if(Rente(mao))
        return 600+HighCard(mao);
    else if(Hetero(mao))
        return 500+HighCard(mao);
    else if(ThreeofaKind(mao))
        return 400+ThreeofaKind(mao);
    else if(TowPar(mao))
        return 300+TowPar(mao);
    else if(OnePar(mao))
        return 200+OnePar(mao);
    else if(HighCard(mao))
        return 100+HighCard(mao);
    return 0;
}
int StringToInt(string s)
{
    int n;
    stringstream ss;
    ss << s;
    ss >> n;
    return n;
}
string IntToString(int n)
{
    string s;
    stringstream ss;
    ss << n;
    ss >> s;
    return s;
}

int StringToBtn(string card)
{
    if(card == "XX")
        return 99;

    int adc= 0;
    switch(card.at(1))
    {
    case 'O':
        break;
    case 'C': adc+=13;
        break;
    case 'E': adc+=26;
        break;
    case 'P': adc+=39;
        break;
    }
    string num;
    num= card.at(0);
    if(num == "A")
        adc+=12;
    else if(num == "K")
        adc+=11;
    else if(num == "Q")
        adc+=10;
    else if(num == "J")
        adc+=9;
    else if(num == "T")
        adc+=8;
    else
        adc+= StringToInt(num)-2;
    return adc+100;
}

#endif // UTILITY_H_INCLUDED
