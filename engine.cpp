
bool Windows::Register(HINSTANCE h)
{
    hInstance= h;

    wc.cbClsExtra= 0;
    wc.cbSize= sizeof(WNDCLASSEX);
    wc.cbWndExtra= 0;
    wc.hbrBackground= (HBRUSH) (COLOR_3DLIGHT);
    wc.hCursor= LoadCursor(NULL, IDC_ARROW);
    wc.hIcon= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wc.hIconSm= LoadIcon(NULL, IDC_ICON);
    wc.hInstance= hInstance;
    wc.lpfnWndProc= WndProc;
    wc.lpszClassName= Cpoker;
    wc.lpszMenuName= MAKEINTRESOURCE(IDR_MENU);
    wc.style= CS_VREDRAW | CS_HREDRAW;

    if(!RegisterClassEx(&wc))
        return 0;
    return 1;
}

bool Windows::Create()
{
    hwnd= CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,
            Cpoker, "Poker",
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            NULL, NULL, hInstance, NULL
            );

    if(!hwnd)
        return 0;

    hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, 350, 480, 80, 25, hwnd, NULL, NULL, NULL);
    BTN[0]= CreateButtonEx( hwnd, hInstance, IDB_ALLIN, 250, 480, 80, 23, "Apostar Tudo", WS_VISIBLE );
    BTN[1]= CreateButtonEx( hwnd, hInstance, IDB_RAISE, 450, 480, 80, 23, "Pagar", WS_VISIBLE );
    BTN[2]= CreateButtonEx( hwnd, hInstance, IDB_FOLD, 350, 520, 80, 23, "Desistir", WS_VISIBLE );
    BTN[3]= CreateButtonEx( hwnd, hInstance, IDB_CONTINUE, 350, 480, 80, 23, "Continue", WS_VISIBLE );

    return 1;
}
void Windows::Update(int uCmdShow)
{
    UpdateWindow(hwnd);
    ShowWindow(hwnd, uCmdShow);
}

LRESULT Windows::Event(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDM_NOVO:
            {
                int ret= MessageBox(hwnd, "Carregar configurações padrões", "Aviso", MB_YESNOCANCEL | MB_ICONEXCLAMATION);
                if(ret == IDCANCEL);
                else if(ret == IDYES)
                {
                    Poker.DefaultParameters();
                    Poker.Inicialize();
                    Poker.InitHand();
                    Poker.IntPaint(hdc, hInstance, hwnd);
                    Poker.Show();
                }
                else
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONFIG), hwnd, SetPokerParameters);
            }
            break;
        case IDM_CONFIG:
                DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONFIG), hwnd, SetPokerParameters);
            break;
        case IDM_SOBRE:
            MessageBox(hwnd, "Desenvolvido por Vitor Ramos Gomes, RealPoker é um jogo em fase de teste.", "Sobre", MB_OK | MB_ICONASTERISK);
            break;
        case IDM_SAIR:
            PostQuitMessage(0);
            break;
        case IDB_ALLIN:
                Poker.op= '1';
            break;
        case IDB_RAISE:
                Poker.op= '3';
            break;
        case IDB_FOLD:
                Poker.op= '4';
            break;
        case IDB_CONTINUE:
            Poker.frize= false;
            Poker.Show();
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Windows::Loop()
{
    Poker.DefaultParameters();
    Poker.Inicialize();
    Poker.InitHand();
    Poker.IntPaint(hdc, hInstance, hwnd);
    Poker.Show();

    while(true)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if(!Poker.frize)
        {
            if(!Poker.end_turn)
            {
               Poker.StartHand();
               Sleep(200);
            }
            else if(!Poker.frize)
            {
                if(Poker.NextHand())
                {
                    Poker.InitHand();
                }
                else
                {
                    Poker.frize= true;
                }
            }
        }
        else
        {
        }
    }
}
