
void Player::Show(HDC hdc, HWND hwnd)
{
    char buffer[35];

    hdc = GetDC(hwnd);

    SetTextColor(hdc, RGB(0,255,0));
    SetBkColor(hdc, RGB(0,0,0));
    SetBkMode(hdc, OPAQUE);


    sprintf(buffer, "                               ");
    TextOut(hdc, posX, posY, buffer, strlen(buffer));
    if(blind)
    {
        sprintf(buffer, "Blind");
        TextOut(hdc, posX, posY-30, buffer, strlen(buffer));
    }
    else if(bigblind)
    {
        sprintf(buffer, "BigBlind");
        TextOut(hdc, posX, posY-30, buffer, strlen(buffer));
    }

    sprintf(buffer, "                               ");
    TextOut(hdc, posX, posY, buffer, strlen(buffer));

    sprintf(buffer, name.c_str());
    TextOut(hdc, posX, posY, buffer, strlen(buffer));

    sprintf(buffer, "                               ");
    TextOut(hdc, posX, posY+30, buffer, strlen(buffer));

    sprintf(buffer, action.c_str());
    TextOut(hdc, posX, posY+30, buffer, strlen(buffer));

    sprintf(buffer, "                               ");
    TextOut(hdc, posX, posY+15, buffer, strlen(buffer));

    sprintf(buffer, "%d", money);
    TextOut(hdc, posX, posY+15, buffer, strlen(buffer));


    ReleaseDC(hwnd, hdc);

}
