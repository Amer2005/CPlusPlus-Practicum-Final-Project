#include <iostream>
#include <windows.h> 
#include <fstream>

const int MAX_SIZE = 20;
const int MIN_SIZE = 4;
const char SAVE_FILE_NAME[] = "save-game.txt";

const int EXTRA_SPACES_PER_CELL = 2;
const int MAXVALUE_SCALER = 1;
const int GENERATED_VALUE_DIVIDER = 2;

const HANDLE HConsole = GetStdHandle(STD_OUTPUT_HANDLE);

unsigned long PLAYER1_BAKCGROUND_COLOR = BACKGROUND_BLUE;
unsigned long PLAYER1_FOREGROUND_COLOR = FOREGROUND_BLUE;

unsigned long PLAYER2_BAKCGROUND_COLOR = BACKGROUND_GREEN;
unsigned long PLAYER2_FOREGROUND_COLOR = FOREGROUND_GREEN;

unsigned long PLAYER_HEADFORE_COLOR = FOREGROUND_RED;

unsigned long PLAYER_USEDCELL_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
unsigned long DEAFULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

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

void SetConsoleColor(unsigned long colorToSet)
{
    SetConsoleTextAttribute(HConsole, colorToSet);
}

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

void SetStartingCells(Board& board)
{
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
}

void CreateBoard(Board &board)
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

            int generationCoefficient = row + col;

            if (row + col > minRowCol)
            {
                generationCoefficient = (board.rows - row - 1) + (board.cols - col - 1);
            }

            maxGeneration = generationCoefficient * MAXVALUE_SCALER;

            currentCell.value =  GenerateRandomNumber((maxGeneration) / GENERATED_VALUE_DIVIDER, maxGeneration);
            currentCell.actionType = (ActionTypesEnum)GenerateRandomNumber(1, 4);
            currentCell.used = Unused;
        }
    }

    SetStartingCells(board);
}

bool CreateGame(Game &game, int rows, int cols)
{
    srand(time(0));

    if (rows > MAX_SIZE || cols > MAX_SIZE)
    {
        return false;
    }

    if (rows < MIN_SIZE || cols < MIN_SIZE)
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

void PrintSpaces(int count)
{
    for (int i = 0; i < count; i++)
    {
        std::cout << ' ';
    }
}

void PrintCell(Game& game, int row, int col, int maxLength)
{
    Cell currentCell = game.board.cells[row][col];

    if (currentCell.used == UsedPlayer1)
    {
        SetConsoleColor(PLAYER1_BAKCGROUND_COLOR | BACKGROUND_INTENSITY |
            PLAYER_USEDCELL_COLOR | FOREGROUND_INTENSITY);
    }
    else if (currentCell.used == UsedPlayer2)
    {
        SetConsoleColor(PLAYER2_BAKCGROUND_COLOR | BACKGROUND_INTENSITY |
            PLAYER_USEDCELL_COLOR | FOREGROUND_INTENSITY);
    }

    if (game.player1.x == row && game.player1.y == col)
    {
        SetConsoleColor(PLAYER1_BAKCGROUND_COLOR |
            PLAYER_HEADFORE_COLOR | FOREGROUND_INTENSITY);
    }

    else if (game.player2.x == row && game.player2.y == col)
    {
        SetConsoleColor(PLAYER2_BAKCGROUND_COLOR |
            PLAYER_HEADFORE_COLOR | FOREGROUND_INTENSITY);
    }

    int currentLength = GetNumberLength(currentCell.value) + 1;

    PrintSpaces((maxLength - currentLength) / 2 + (maxLength - currentLength) % 2);

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

    PrintSpaces((maxLength - currentLength) / 2);
}

void PrintGame(Game& game)
{

    int maxNumber = GetMaxNumber(game.board);

    int maxLength = GetNumberLength(maxNumber) + 1;

    maxLength += EXTRA_SPACES_PER_CELL; //adding spaces so it looks better;


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
            PrintCell(game, row, col, maxLength);

            SetConsoleColor(DEAFULT_COLOR);
            std::cout << '|';
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < totalLines; i++)
    {
        std::cout << (char)((i == 0 || i == totalLines - 1) ? '+' : '-');
    }
   
    SetConsoleColor(DEAFULT_COLOR);

    std::cout << std::endl;

    std::cout << "BLUE: "<< Round(game.player1.totalSum);

    std::cout << "         GREEN: " << Round(game.player2.totalSum)<<std::endl;

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
    case Additon: 
        player.totalSum += currentCell.value; break;
    case Subtraction: 
        player.totalSum -= currentCell.value; break;
    case Multiplication: 
        player.totalSum *= currentCell.value; break;
    case Division: 
        player.totalSum /= currentCell.value; break;
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

void SaveGame(std::ofstream& ofs, const Game& game)
{
    ofs << game.board.rows << " " << game.board.cols << " ";

    for (int rows = 0; rows < game.board.rows; rows++)
    {
        for (int cols = 0; cols < game.board.cols; cols++)
        {
            ofs << game.board.cells[rows][cols].value << " "
                << (int)game.board.cells[rows][cols].actionType << " "
                << (int)game.board.cells[rows][cols].used << " ";
        }
    }

    ofs << game.player1.totalSum << " "
        << game.player1.x << " "
        << game.player1.y << " "
        << (int)game.player1.PlayerTurn << " ";

    ofs << game.player2.totalSum << " "
        << game.player2.x << " "
        << game.player2.y << " "
        << (int)game.player2.PlayerTurn << " ";

    ofs << (int)game.PlayerTurn;
}

void SaveGame(const char* saveFilePath, const Game& game)
{
    if (!saveFilePath)
    {
        return;
    }

    std::ofstream ofs(saveFilePath);

    if (!ofs.is_open())
    {
        return;
    }

    SaveGame(ofs, game);

    ofs.close();
}

void PrintPlayerName(PlayerTurns player)
{
    if (player == Player1)
    {
        SetConsoleColor(PLAYER1_FOREGROUND_COLOR);
        std::cout << "Player 1";
    }
    else
    {
        SetConsoleColor(PLAYER2_FOREGROUND_COLOR);
        std::cout << "Player 2";
    }
}

void GameLoop(Game& game)
{
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

        PrintPlayerName(game.PlayerTurn);

        SetConsoleColor(DEAFULT_COLOR);
        std::cout<< " Move: " << std::endl;

        while (true) {

            std::cin >> input;

            if (input[0] == 'e') {
                SaveGame(SAVE_FILE_NAME, game);
                std::cout << "Game Saved!" << std::endl;
                return;
            }

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
        PrintPlayerName(Player1);
        std::cout << " won the game with sum: " << Round(game.player1.totalSum) << std::endl;
        PrintPlayerName(Player2);
        std::cout << " score: " << Round(game.player2.totalSum) << std::endl;
    }
    else if (game.player2.totalSum > game.player1.totalSum)
    {
        PrintPlayerName(Player2);
        std::cout << " won the game with sum: " << Round(game.player2.totalSum) << std::endl;

        PrintPlayerName(Player1);
        std::cout << " score: " << Round(game.player1.totalSum) << std::endl;
    }
    else
    {
        std::cout << "It was a draw with total sum: " << Round(game.player2.totalSum) << std::endl;
    }
}

void StartGame(int rows, int cols)
{
    Game game;

    CreateGame(game, rows, cols);

    GameLoop(game);
}

Game LoadGameFromFile(std::ifstream& ifs)
{
    Game game;

    ifs >> game.board.rows >> game.board.cols;

    for (int rows = 0; rows < game.board.rows; rows++)
    {
        for (int cols = 0; cols < game.board.cols; cols++)
        {
            int actionTypeInt = 1;
            int usedInt = 1;
            ifs >> game.board.cells[rows][cols].value >> actionTypeInt >> usedInt;

            game.board.cells[rows][cols].actionType = (ActionTypesEnum)actionTypeInt;
            game.board.cells[rows][cols].used = (UsedTypeEnum)usedInt;
        }
    }

    int playerTurnEnumInt = 1;

    ifs >> game.player1.totalSum >> game.player1.x >> game.player1.y >> playerTurnEnumInt;
    game.player1.PlayerTurn = (PlayerTurns)playerTurnEnumInt;

    ifs >> game.player2.totalSum >> game.player2.x >> game.player2.y >> playerTurnEnumInt;
    game.player2.PlayerTurn = (PlayerTurns)playerTurnEnumInt;

    ifs >> playerTurnEnumInt;

    game.PlayerTurn = (PlayerTurns)playerTurnEnumInt;

    return game;
}

Game LoadGameFromFile(const char* saveFilePath)
{
    if (!saveFilePath)
    {
        return {};
    }

    std::ifstream ifs(saveFilePath);

    if (!ifs.is_open())
    {
        return {};
    }

    Game game = LoadGameFromFile(ifs);

    ifs.close();

    return game;
}

void LoadGame()
{
    Game game = LoadGameFromFile(SAVE_FILE_NAME);

    GameLoop(game);
}

void LaunchMainMenu()
{
    std::cout << "Type 'l' to load old game or type 'n' to begin new game" << std::endl;

    char input;
    std::cin >> input;

    if (input == 'l')
    {
        LoadGame();
    }
    else
    {
        int rows, cols;

        std::cout << "Enter row count: " << std::endl;

        std::cin >> rows;

        std::cout << "Enter collumn count: " << std::endl;

        std::cin >> cols;

        system("cls");
        StartGame(rows, cols);
    }
}

int main()
{
    LaunchMainMenu();
}
