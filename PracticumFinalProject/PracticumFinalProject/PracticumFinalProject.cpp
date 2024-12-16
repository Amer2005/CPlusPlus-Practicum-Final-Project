#include <iostream>
#include <windows.h> 

const int MAX_SIZE = 20;
const int MIN_SIZE = 4;

enum ActionTypesEnum
{
    Additon = 1,
    Subtraction = 2,
    Division = 3,
    Multiplication = 4
};

enum UsedTypeEnum
{
    Unused = 0,
    UsedPlayer1 = 1,
    UsedPlayer2 = 2
};

enum PlayerTurns {
    Player1 = 1,
    Player2 = 2
};

struct Cell {
    int value = 0;

    ActionTypesEnum actionType = Additon;

    UsedTypeEnum used = Unused;
};

struct Player
{
    int x = 0;
    int y = 0;

    double totalSum = 0;

    PlayerTurns PlayerTurn = Player1;
};

struct Board
{
    Cell cells[MAX_SIZE][MAX_SIZE];

    int rows = 0;
    int cols = 0;
};

struct Game {
    Board board;

    Player player1;
    Player player2;

    PlayerTurns PlayerTurn = Player1;
};

double Round(double d)
{
    double x = d * 100.00;
    if (x < 0)
        x = x - 0.5;
    else
        x = x + 0.5;
    int y = x;
    x = y / 100.00;
    return x;
}

int GenerateRandomNumber(int min, int max)
{
    int range = max - min + 1;
    int rng = rand();
    int num = rng % range + min;

    return num;
}

bool CreateBoard(Board &board)
{
    int minRowCol = (board.rows < board.cols) ? board.rows : board.cols;

    for (int row = 0; row < board.rows; row++)
    {
        for (int col = 0; col < board.cols; col++)
        {
            Cell &currentCell = board.cells[row][col];

            if ((row == 0 && col == 0) || (row == board.rows - 1 && col == board.cols - 1))
            {

                currentCell.value = 0;
                currentCell.actionType = Additon;
                currentCell.used = row == 0 ? UsedPlayer1 : UsedPlayer2;

                continue;
            }

            int maxGeneration = 0;
            if (row + col <= minRowCol)
            {
                maxGeneration = row + col;
                
            }
            else
            {
                maxGeneration = (board.rows - row - 1) + (board.cols - col - 1);
            }

            currentCell.value =  GenerateRandomNumber((maxGeneration) / 2, maxGeneration);
            currentCell.actionType = (ActionTypesEnum)GenerateRandomNumber(1, 4);
            currentCell.used = Unused;
        }
    }

    board.cells[0][1].value = 2;
    board.cells[0][1].actionType = Multiplication;

    board.cells[board.rows - 1][board.cols - 2].value = 2;
    board.cells[board.rows - 1][board.cols - 2].actionType = Multiplication;

    board.cells[1][0].value = 2;
    board.cells[1][0].actionType = Division;

    board.cells[board.rows - 2][board.cols - 1].value = 2;
    board.cells[board.rows - 2][board.cols - 1].actionType = Division;

    if (board.rows % 2 != 0 && board.cols % 2 != 0)
    {
        board.cells[board.rows / 2][board.cols / 2].value = 0;
        board.cells[board.rows / 2][board.cols / 2].actionType = Multiplication;
    }
    else
    {
        board.cells[board.rows / 2][board.cols / 2].value = 0;
        board.cells[board.rows / 2][board.cols / 2].actionType = Multiplication;

        board.cells[board.rows / 2 - (board.rows + 1) % 2][board.cols / 2 - (board.cols + 1) % 2].value = 0;
        board.cells[board.rows / 2 - (board.rows + 1) % 2][board.cols / 2 - (board.cols + 1) % 2].actionType = Multiplication;
    }
    return true;
}

bool CreateGame(Game &game, int rows, int cols)
{
    srand(time(0));

    if (rows > MAX_SIZE || cols > MAX_SIZE)
    {
        return false;
    }

    if (rows <= MIN_SIZE || cols <= MIN_SIZE)
    {
        return false;
    }

    game.board.rows = rows;
    game.board.cols = cols;

    game.player1.x = 0;
    game.player1.y = 0;
    game.player1.PlayerTurn = Player1;

    game.player2.x = rows - 1;
    game.player2.y = cols - 1;
    game.player2.PlayerTurn = Player2;
   

    CreateBoard(game.board);
}

int GetNumberLength(int num)
{
    int length = 0;
    do
    {
        length++;
        num /= 10;
    } while (num != 0);

    return length;
}

int GetMaxNumber(Board& board)
{
    int maxNumber = board.cells[0][0].value;

    for (int row = 0; row < board.rows; row++)
    {
        for (int col = 0; col < board.cols; col++)
        {
            if (maxNumber < board.cells[row][col].value)
            {
                maxNumber = board.cells[row][col].value;
            }
        }
    }

    return maxNumber;
}

void PrintGame(Game& game)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int maxNumber = GetMaxNumber(game.board);

    int maxLength = GetNumberLength(maxNumber) + 1;

    maxLength += 2; //adding spaces so it looks better;


    int totalLines = game.board.cols * (maxLength + 1) + 1;

    for (int row = 0; row < game.board.rows; row++)
    {
        for (int i = 0; i < totalLines; i++)
        {
            std::cout << (char)((i == 0 || i == totalLines - 1) ? '+' : '-');
        }
        std::cout << std::endl;
        std::cout << '|';

        for (int col = 0; col < game.board.cols; col++)
        {
            Cell currentCell = game.board.cells[row][col];

            if (currentCell.used == UsedPlayer1)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE  | BACKGROUND_INTENSITY |
                    FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }
            else if (currentCell.used == UsedPlayer2)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN  | BACKGROUND_INTENSITY |
                    FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }

            if (game.player1.x == row && game.player1.y == col)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE |
                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }

            else if (game.player2.x == row && game.player2.y == col)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN |
                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }

            int currentLength = GetNumberLength(currentCell.value) + 1;

            for (int i = 0; i < (maxLength - currentLength) / 2 + (maxLength - currentLength) % 2; i++)
            {
                std::cout << ' ';
            }

            char currentSymbol = '+';

            switch (currentCell.actionType)
            {
            case Additon: currentSymbol = '+'; break;
            case Subtraction: currentSymbol = '-'; break;
            case Multiplication: currentSymbol = 'x'; break;
            default: currentSymbol = '/';
            }

            std::cout << currentSymbol;
            std::cout << currentCell.value;

            for (int i = 0; i < (maxLength - currentLength) / 2; i++)
            {
                std::cout << ' ';
            }

            SetConsoleTextAttribute(hConsole, 15);
            std::cout << '|';
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < totalLines; i++)
    {
        std::cout << (char)((i == 0 || i == totalLines - 1) ? '+' : '-');
    }

    std::cout << std::endl;

    
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "BLUE: "<< Round(game.player1.totalSum);


    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "         GREEN: " << Round(game.player2.totalSum)<<std::endl;

    SetConsoleTextAttribute(hConsole, 15);
}

bool CheckCell(Game& game, int row, int col)
{
    if (row < 0 || row >= game.board.rows)
    {
        return false;
    }

    if (col < 0 || col >= game.board.cols)
    {
        return false;
    }

    return game.board.cells[row][col].used == Unused;
}

bool AreThereAvailableMoves(Game& game, Player& player)
{
    if (CheckCell(game, player.x + 1, player.y)) return true;
    if (CheckCell(game, player.x + 1, player.y + 1)) return true;
    if (CheckCell(game, player.x + 1, player.y - 1)) return true;
    if (CheckCell(game, player.x - 1, player.y)) return true;
    if (CheckCell(game, player.x - 1, player.y + 1)) return true;
    if (CheckCell(game, player.x - 1, player.y - 1)) return true;
    if (CheckCell(game, player.x, player.y + 1)) return true;
    if (CheckCell(game, player.x, player.y - 1)) return true;

    return false;
}

void MovePlayer(Game& game, Player& player, int newRow, int newCol)
{
    Cell& currentCell = game.board.cells[newRow][newCol];

    currentCell.used = player.PlayerTurn == Player1 ? UsedPlayer1 : UsedPlayer2;

    switch (currentCell.actionType)
    {
    case Additon: player.totalSum += currentCell.value; break;
    case Subtraction: player.totalSum -= currentCell.value; break;
    case Multiplication: player.totalSum *= currentCell.value; break;
    case Division: player.totalSum /= currentCell.value; break;
    default:
        break;
    }

    player.x = newRow;
    player.y = newCol;
}

bool DoInputForPlayer(Game& game, Player& player, char* move)
{
    int newRow = player.x, newCol = player.y;
    if (move[0] == 'w' || move[1] == 'w')
    {
        newRow--;
    }
    if (move[0] == 'a' || move[1] == 'a')
    {
        newCol--;
    }
    if (move[0] == 's' || move[1] == 's')
    {
        newRow++;
    }
    if (move[0] == 'd' || move[1] == 'd')
    {
        newCol++;
    }

    if (CheckCell(game, newRow, newCol))
    {
        MovePlayer(game, player, newRow, newCol);

        return true;
    }
    else
    {
        return false;
    }
}

void StartGame(int rows, int cols)
{
    Game game;

    CreateGame(game, rows, cols);

    PrintGame(game);

    while (true)
    {
        Player& currentPlayer = ((game.PlayerTurn == Player1) ? game.player1 : game.player2);
        if (!AreThereAvailableMoves(game, currentPlayer))
        {
            break;
        }
        char input[MAX_SIZE];
        std::cout << "Enter w a s d to move" << std::endl;
        std::cout << "Enter 'e' to exit and save" << std::endl;

        std::cout << ((game.PlayerTurn == Player1) ? "Blue Player Move:" : "Green Player Move:") << std::endl;
        
       

        

        while (true) {

            std::cin >> input;


            if (DoInputForPlayer(game, currentPlayer, input))
            {
                break;
            }

            std::cout << "invalid input try again" << std::endl;
        }
        system("cls");
        PrintGame(game);

        ((game.PlayerTurn == Player1) ? game.PlayerTurn = Player2 : game.PlayerTurn = Player1);
    }

    if (game.player1.totalSum > game.player2.totalSum)
    {
        std::cout << "Player 1 won the game with sum: " << Round(game.player1.totalSum)<<std::endl;
        std::cout << "Player 2 score: " << Round(game.player2.totalSum) << std::endl;
    }
    else if (game.player2.totalSum > game.player1.totalSum)
    {
        std::cout << "Player 2 won the game with sum: " << Round(game.player2.totalSum) << std::endl;
        std::cout << "Player 1 score: " << Round(game.player1.totalSum) << std::endl;
    }
    else
    {
        std::cout << "It was a draw with total sum: " << Round(game.player2.totalSum) << std::endl;
    }
}

int main()
{
    StartGame(7, 7);
}
