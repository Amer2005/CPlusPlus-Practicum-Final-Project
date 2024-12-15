#include <iostream>

using namespace std;

const int MAX_SIZE = 100;

enum ActionTypesEnum
{
    Additon = '+',
    Subtraction = '-',
    Division = '/',
    Multiplication = 'x'
};

struct Cell {
    int value;

    ActionTypesEnum actionType;
};

struct Player
{
    int x;
    int y;

    double totalSum = 0;

    bool used = false;
};

struct Board
{
    Cell cells[MAX_SIZE][MAX_SIZE];

    int Rows;
    int Cols;
};

struct Game {
    
};

int main()
{
    
}
