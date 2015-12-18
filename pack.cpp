
void Pack::Initialize()
{
    string nipe[]= {"O", "C", "E", "P"};
    string cards[]= {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
    int cont= 0;
    distCard= -1;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<13; j++)
        {
            card[cont++]= cards[j]+nipe[i];
        }
    }
}
void Pack::Scrambles()
{
    srand(time(NULL));
    for(int i=0; i<52; i++)
    {
        int aleatorio= rand()%52;
        string aux= card[i];
        card[i]= card[aleatorio];
        card[aleatorio]= aux;
    }
}
string Pack::Card()
{
    distCard++;
    return card[distCard];
}
