#include <iostream>
#include "stos.h"
#include <string.h>
#include <fstream>
#include <string>
using namespace std;

#define BLUEPAWS 'b'
#define REDPAWS 'r'
#define BREAKPOINT "---"
#define BREAKPOINT_SIZE 3
#define BLUEPATTERN "< b >"
#define REDPATTERN "< r >"
#define EMPTYPATTERN "<   >"
#define PATTERNS_SIZE 5
#define MAX_ROWS_COLUMNS 11
#define MAX_LINE_COMMAND_SIZE 70

#define PROSTO 'p'
#define LEWO 'l'
#define PRAWO 'r'
#define LEWYSKOS 'z'
#define PRAWYSKOS 'x'
#define GORA 'g'

struct Board {
    char board[MAX_ROWS_COLUMNS][MAX_ROWS_COLUMNS];
    int halfline;
    int size;
    int redPawns;
    int bluePawns;
    bool IsBoardCorrect;
    char HowWon;
    bool win_in_one_move_red;
    bool win_in_one_move_blue;
};


bool IsLetter(char litera)
{
    if (litera == 'B' || litera == 'I' || litera == 'P' || litera == 'C')
        return true;
    return false;
}

int IleRazyWystepuje(char* inputString, char searchChar) {
    int count = 0;
    for (int i = 0; inputString[i] != '\0'; ++i) {
        if (inputString[i] == searchChar) {
            count++;
        }
    }
    return count;
}

bool InString(char* mainString, const char searchString[], int size) {
    int indexSearch = size;
    int index = 0;
    for (int i = 0; mainString[i] != '\0'; i++)
    {
        if (mainString[i] == searchString[index]) 
            index++;
        else
            index = 0;
        if (index == indexSearch)
            return true;
    }
    return false;
}

void AddPawnsToBoard(char line[], Board& board,int licznik,bool second_half, int range){   
    if (InString(line, BREAKPOINT, BREAKPOINT_SIZE))
        return;
    char temp[MAX_LINE_COMMAND_SIZE];
    memset(temp, 0, sizeof(temp));

    int index = 0;
    int row = licznik - 2;
    int column = 0;
    if (second_half == true && board.size!=1){
        row = range-1;
        board.halfline >= range-1 ? column = range-1 : column = board.halfline;
    }
    for (int i = 0; line[i]!='\0'; i++) {
        temp[index++] = line[i];
        if (InString(temp, BLUEPATTERN, PATTERNS_SIZE)) {
            memset(temp, 0, sizeof(temp));
            board.board[row][column] = 'b';
            row -= 1, column += 1;
            index = 0;
        }
        else if (InString(temp, REDPATTERN, PATTERNS_SIZE)) {
            board.board[row][column] = 'r';
            memset(temp, 0, sizeof(temp));
            index = 0;
            row -= 1, column += 1;
            
        }
        else if (InString(temp, EMPTYPATTERN, PATTERNS_SIZE)) {
            board.board[row][column] = '.';
            memset(temp, 0, sizeof(temp));
            row -= 1, column += 1;
            index = 0;
        }     
    }
}


void PrintBoard(Board game)
{
    for (int i = 0; i < game.size; i++)
    {
        for (int j = 0; j < game.size; j++)
            cout << game.board[i][j] << ' ';
        cout << endl;
    }
}
    
bool DidBlueWon(Board& board,int columns,int rows) {
    stos path;
    bool won = true;
    bool visited[MAX_ROWS_COLUMNS][MAX_ROWS_COLUMNS];
    memset(visited, false, sizeof(visited));
    while (rows + 1 != board.halfline && columns != board.halfline) {
        visited[rows][columns] = true;
        if (rows + 1 != board.halfline && board.board[rows + 1][columns] == 'b' && visited[rows + 1][columns] != true) {
            path.dodawanie(PROSTO);
            rows++;
        }
        else if (rows != 0 && board.board[rows - 1][columns] == 'b' && visited[rows - 1][columns] != true) {
            path.dodawanie(GORA);
            rows--;
        }
        else if (columns + 1 != board.halfline && rows + 1 != board.halfline && board.board[rows + 1][columns + 1] == 'b' && visited[rows + 1][columns + 1] != true) {
            path.dodawanie(PRAWYSKOS);
            rows++;
            columns++;
        }
        else if (columns != 0 && rows != 0 && board.board[rows - 1][columns - 1] == 'b' && visited[rows - 1][columns - 1] != true) {
            path.dodawanie(LEWYSKOS);
            rows--;
            columns--;
        }
        else if (columns + 1 != board.halfline && board.board[rows][columns + 1] == 'b' && visited[rows][columns + 1] != true) {
            path.dodawanie(PRAWO);
            columns++;
        }
        else if (columns != 0 && board.board[rows][columns - 1] == 'b' && visited[rows][columns - 1] != true) {
            path.dodawanie(LEWO);
            columns--;
        }
        else {
            if (!path.empty()) {
                board.board[rows][columns] = 'b';
                char lastDirection = path.GetAndDelete();
                if (lastDirection == PROSTO)
                    rows--;
                else if (lastDirection == PRAWYSKOS) {
                    rows--;
                    columns--;
                }
                else if (lastDirection == LEWYSKOS) {
                    rows++;
                    columns++;
                }
                else if (lastDirection == GORA)
                    rows++;
                else if (lastDirection == PRAWO)
                    columns--;
                else
                    columns++;
            }
            else {
                won = false;
                break;
            }
        }
    }
    if (won) 
    {
        return true;
    }
    memset(visited, false, sizeof(visited));
    
    return false;
}

bool DidRedWon(Board& board, int columns, int rows) {
    stos path; 
    bool won = true;
    bool visited[MAX_ROWS_COLUMNS][MAX_ROWS_COLUMNS];
    memset(visited, false, sizeof(visited));
    while (rows != board.halfline && columns + 1 != board.halfline) {
        visited[rows][columns] = true;  
        if (columns + 1 != board.halfline && board.board[rows][columns + 1] == 'r' && visited[rows][columns + 1]!=true) {
            path.dodawanie(PROSTO);  
            columns++;
        }
        else if (columns != 0 && board.board[rows ][columns- 1] == 'r' && visited[rows ][columns- 1] != true) {
            path.dodawanie(GORA);
            columns--;
        }
        else if (columns + 1 != board.halfline && rows + 1 != board.halfline && board.board[rows + 1][columns + 1] == 'r' && visited[rows + 1][columns + 1]!=true) {
            path.dodawanie(LEWYSKOS);  
            rows++;
            columns++;
        }
        else if (columns != 0 && rows != 0 && board.board[rows - 1][columns - 1] == 'r' && visited[rows - 1][columns - 1] != true) {
            path.dodawanie(PRAWYSKOS);
            rows--;
            columns--;
        }
        else if (rows + 1 != board.halfline && board.board[rows + 1][columns] == 'r' && visited[rows + 1][columns]!=true) {
            path.dodawanie(LEWO); 
            rows++;
        }
        else if (rows != 0 && board.board[rows - 1][columns] == 'r'  && visited[rows - 1][columns]!=true) {
            path.dodawanie(PRAWO);  
            rows--;
        }
        else {                  
            if (!path.empty()) {
                board.board[rows][columns] = 'r';
                char lastDirection = path.GetAndDelete();
                if (lastDirection == PROSTO)
                    columns--;
                else if (lastDirection == LEWYSKOS) {
                    rows--;
                    columns--;
                }
                else if (lastDirection == PRAWYSKOS) {
                    rows++;
                    columns++;
                }
                else if (lastDirection == GORA)
                    columns++;
                else if (lastDirection == LEWO)
                    rows--;
                else
                    rows++;
            }
            else {
                won = false;
                break;
            }
        }
    }
    memset(visited, false, sizeof(visited));
    if (won) {
        return true;
    }
    
    
    return false;
}


bool CheckWinRed(Board& board) {
    bool wynik = false;
    for (int i = 0; i < board.size; i++)
    {
        if (board.board[i][0] == 'r')
            wynik = DidRedWon(board,0,i);
        if (wynik == true)
            return true;
    }
    return false;
}


bool CheckWinBlue(Board& board) {
    bool wynik = false;
    for (int i = 0; i < board.size; i++)
    {
        if (board.board[0][i] == 'b')
            wynik = DidBlueWon(board,i,0);
        if (wynik == true)
            return true;
    }
    return false;
}


void outputToFileWithEndl(const std::string& str) {
    std::ofstream outputFile("output.txt", std::ios::app); // Open file in append mode
    if (outputFile.is_open()) {
        outputFile << str << '\n' << std::endl;
        outputFile.close();
    }
}

void GameOver(Board& board) {
    if (board.IsBoardCorrect==false) {
        cout << "NO" << endl;
        return;
    }
    
    if (CheckWinBlue(board)==true)
    {
        cout << "YES BLUE" << endl;
        board.HowWon = 'b';
        return;
    }
    else if (CheckWinRed(board)==true)
    {
        cout << "YES RED" << endl;
        board.HowWon = 'r';
        return;
    }
    else
        cout << "NO" << endl;

}


bool DeleteAndCheckBoard(Board& board, bool WinR, bool WinB)
{
    bool outcome = true;
    char pionek = ' ';
    for (int i = 0; i < board.size; i++)
    {
        for (int j = 0; j < board.size; j++)
        {
            pionek = board.board[i][j];
            board.board[i][j] = '.';
            if (pionek == 'r' && WinR !=false)
            {
                outcome = CheckWinRed(board);
            }
            else if (pionek == 'b' && WinB!=false)
            {
                outcome = CheckWinBlue(board);
            }
            board.board[i][j] = pionek;
            if (outcome == false)
                return false;
        }
    }
    return true;

}


bool IsBoardPossible(Board& board)
{
    bool WinR = false, WinB=false;
    if (board.IsBoardCorrect == false){
        return false;
    }
    if (board.size == 1)
    {
        return true;
    }
    WinR = CheckWinRed(board);
    WinB = CheckWinBlue(board);
    if (WinR && WinB)
        return false;
    else if (!WinR && !WinB)
        return true;
    else if (WinR && board.bluePawns == board.redPawns)
        return false;
    else if (WinB && board.bluePawns + 1 == board.redPawns)
        return false;
    else if (DeleteAndCheckBoard(board, WinR, WinB) == true)
        return false;
    else
        return true;

}

char CheckWhoseMove(Board board)
{
    if (board.bluePawns == 0 && board.redPawns == 0)
        return 'r';
    else if (board.redPawns == board.bluePawns+1)
        return 'b';
    else
        return 'r';
}


int CountFreeCells(Board game)
{
    int wolne = 0;
    for (int i = 0; i < game.size; i++)
    {
        for (int j = 0; j < game.size; j++)
        {
            if (game.board[i][j] == '.')
                wolne++;
        }
    }
    return wolne;
}


bool CheckIfCanWin(Board& game, char player)
{
    char pionek;
    bool outcome = false;
    for (int i = 0; i < game.size; i++)
    {
        for (int j = 0; j < game.size; j++)
        {
            pionek = game.board[i][j];
            if (pionek == '.')
            {
                game.board[i][j] = player;
                if (player =='r')
                    outcome = CheckWinRed(game);
                else
                    outcome = CheckWinBlue(game);
                game.board[i][j] = '.';
            }
            if (outcome == true)
                return true;
        }
    }
    return false;
}

bool TwoMovesWin(Board& game, char player)
{
    char pionek;
    bool outcome = false;
    for (int i = 0; i < game.size; i++)
    {
        for (int j = 0; j < game.size; j++)
        {
            pionek = game.board[i][j];
            if (pionek == '.')
            {
                game.board[i][j] = player;
                for (int k = 0; k < game.size; k++)
                {
                    for (int l = 0; l < game.size; l++)
                    {
                        pionek = game.board[k][l];
                        if (pionek == '.')
                        {
                            game.board[k][l] = player;
                            if (player == 'r')
                                outcome = CheckWinRed(game);
                            else
                                outcome = CheckWinBlue(game);
                            game.board[k][l] = '.';
                        }
                        if (outcome == true)
                            break;
                    }
                    if (outcome == true)
                        break;
                }
                game.board[i][j] = '.';
                if (outcome == true)
                    return true;
            }
        }
    }
    return false;
}


bool IsThereAPlaceNotToWinInOneMove(Board& game,char player)
{
    char pionek;
    bool outcome = true;
    for (int i = 0; i < game.size; i++)
    {
        for (int j = 0; j < game.size; j++)
        {
            pionek = game.board[i][j];
            if (pionek == '.')
            {
                game.board[i][j] = player;
                if (player == 'r')
                    outcome = CheckWinRed(game);
                else
                    outcome = CheckWinBlue(game);
                game.board[i][j] = '.';
            }
            if (outcome == false)
                return true;
        }
    }
    return false;
}


void CanWin(Board& board,char line[])
{
    char move = CheckWhoseMove(board);
    int iloscWolnychPol = CountFreeCells(board);
    if (board.IsBoardCorrect == false || IsBoardPossible(board)==false || iloscWolnychPol==0 || CheckWinBlue(board) == true || CheckWinRed(board)==true)
    {
        cout << "NO" << endl;
        return;
    }
    if (InString(line, "RED_WIN_IN_1", 12))
    {
        if (move == 'r' && CheckIfCanWin(board, 'r'))
        {
            cout << "YES" << endl;
            board.win_in_one_move_red = true;
            return;
        }
        else if (move == 'b' && CheckIfCanWin(board, 'r') && iloscWolnychPol > 1)
        {
            cout << "YES" << endl;
            board.win_in_one_move_red = true;
            return;
        }
        else
            cout << "NO" << endl;
    }
    else if (InString(line, "BLUE_WIN_IN_1", 13))
    {
        if (move == 'b' && CheckIfCanWin(board, 'b'))
        {
            cout << "YES" << endl;
            board.win_in_one_move_blue = true;
            return;
        }
        else if (move == 'r' && CheckIfCanWin(board, 'b') && iloscWolnychPol > 1)
        {
            cout << "YES" << endl;
            board.win_in_one_move_blue = true;
            return;
        }
        else
            cout << "NO" << endl;
    }
    else if (InString(line, "RED_WIN_IN_2", 12))
    {
        if (iloscWolnychPol < 3 || board.redPawns+2<board.size || IsThereAPlaceNotToWinInOneMove(board,'r')==false)
        {
            cout << "NO" << endl;
            return;
        }
        else if (TwoMovesWin(board, 'r') && iloscWolnychPol >= 3 && move=='r')
            cout << "YES" << endl;
        else if (TwoMovesWin(board, 'r') && iloscWolnychPol >= 4 && move=='b')
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
    else if (InString(line, "BLUE_WIN_IN_2", 13))
    {
        if (iloscWolnychPol < 3 || board.bluePawns + 2 < board.size || IsThereAPlaceNotToWinInOneMove(board, 'b') == false)
        {
            cout << "NO" << endl;
            return;
        }
        else if (TwoMovesWin(board, 'b') && iloscWolnychPol >= 3 && move == 'b')
            cout << "YES" << endl;
        else if (TwoMovesWin(board, 'b') && iloscWolnychPol >= 4 && move == 'r')
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }

}


int CountCellsInRow(char line[])
{
    int cells = 0;
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '<')
            cells++;
    }
    return cells;

}


void CheckIfSecondHalf(char line[],int& len,bool& second_half,int& range)
{
    int current_len = 0;
    for (int i = 0; line[i] != '\0'; i++)
        current_len++;
    if (current_len < len)
        second_half = true;
    if (current_len > len)
        range = CountCellsInRow(line);
    len = current_len;
}


void InitializeOrResetBoard(Board& game)
{
    game.size = 0;
    game.redPawns = 0;
    game.bluePawns = 0;
    game.halfline = 0;
    game.IsBoardCorrect = false;
    game.HowWon = 'n';      //none of them
    memset(game.board, 0, sizeof(game.board));
    game.win_in_one_move_red = false;
    game.win_in_one_move_blue = false;
}

void ParseInput(Board& board) {             
    char line[MAX_LINE_COMMAND_SIZE];
    char a = ' ';
    int licznik = 0, len = 0, range = 0;
    bool half = false;
    while (true) {

        cin.getline(line, MAX_LINE_COMMAND_SIZE);
        while (IsLetter(line[0])==false) {
            licznik++;
            board.redPawns += IleRazyWystepuje(line, REDPAWS);
            board.bluePawns += IleRazyWystepuje(line, BLUEPAWS);
            CheckIfSecondHalf(line,len,half,range);
            if (half == true)
                board.halfline++;
            AddPawnsToBoard(line, board,licznik,half,range);
            cin.getline(line, MAX_LINE_COMMAND_SIZE);
        }
        board.size = (licznik - 1) / 2;
        //PrintBoard(board);
        if (board.redPawns == board.bluePawns || board.redPawns == board.bluePawns + 1)
            board.IsBoardCorrect = true;
        while (!InString(line, BREAKPOINT, BREAKPOINT_SIZE)) {
            if (InString(line, "BOARD_SIZE", 10))       //dziala
                cout << board.size << endl;
            else if (InString(line, "PAWNS_NUMBER", 12))        //dziala
                cout << board.redPawns + board.bluePawns << endl;
            else if (InString(line, "IS_BOARD_CORRECT", 16))        //dziala
            {
                if (board.IsBoardCorrect == true)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            }
            else if (InString(line, "IS_GAME_OVER", 12))        //dziala
                GameOver(board);
            else if (InString(line, "IS_BOARD_POSSIBLE", 17))       //hehe dziala
            {
                if (IsBoardPossible(board) == true)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            }
            else if (InString(line, "NAIVE", 5))        //dziala elo
                CanWin(board,line);
            if (cin.eof() == true)
                break;
            cin.getline(line, MAX_LINE_COMMAND_SIZE);
        }
        InitializeOrResetBoard(board);
        licznik = 1; 
        if (cin.eof() == true)
            break;
        half = false;
        range = 0, len = 0;
    }
}

int main() {
    Board board;
    InitializeOrResetBoard(board);
    ParseInput(board);
    return 0;
}
