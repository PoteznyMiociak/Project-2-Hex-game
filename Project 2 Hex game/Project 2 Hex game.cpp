#include <iostream>
#include<string>
using namespace std;

#define COMMANDNUMBER 19

bool InSet(char symbol)
{
    char Symbols[] = { '-','>','<','r','b' };
    for (int i = 0; i < 5; i++)
    {
        if (symbol == Symbols[i])
            return true;
    }
    return false;
}

//struct BoardInfo
//{
//    int board_size, red_paws, blue_paws;
//    
//};

int main()
{
    int board_size = 0, red_paws = 0, blue_paws = 0, licznik = 0;
    char symbol;
    string command;
    cin >> symbol;
    while (InSet(symbol) == true)
    {
        if (symbol == '<' || symbol == '>')
            board_size++;
        else if (symbol == 'r')
            red_paws++;
        else if (symbol == 'b')
            blue_paws++;
        cin >> symbol;
    }

    while (getline(cin, command) && /*licznik!=COMMANDNUMBER*/)       //ten warunek nie dziala na stosie  bo tam jest plansza zapytanie plansza zapytanie itd
    {
        licznik++;
        if (command == "OARD_SIZE")
            cout << (board_size == 2 ? board_size /2 : board_size /4)<< endl;
        else if (command == "PAWNS_NUMBER")
            cout << red_paws + blue_paws << endl;
        else if (command == "IS_BOARD_CORRECT")
            cout << (red_paws == blue_paws ? "YES" : "NO") << endl;
    }

}
