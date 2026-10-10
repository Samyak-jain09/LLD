#include <bits/stdc++.h>
using namespace std;

class Player
{
private:
    int id;
    string name;
    int currPosition;

public:
    Player(int id, string name) : id(id), name(name), currPosition(0) {}
    string getName() const
    {
        return this->name;
    }
    int getPosition() const
    {
        return this->currPosition;
    }
    void setPosition(int position)
    {
        this->currPosition = position;
    }
};

class Dice
{
private:
    int numberOfDice;

public:
    Dice(int number) : numberOfDice(number) {}
    int rollDice()
    {
        int total = 0;
        int dices = numberOfDice;
        while (dices--)
        {
            total += (rand() % 6) + 1;
        }
        return total;
    }
};

class Jump
{
private:
    int start;
    int end;

public:
    Jump(int st, int en) : start(st), end(en) {}
    int getStart() const
    {
        return this->start;
    }
    int getEnd() const
    {
        return this->end;
    }
};

class Cell
{
private:
    unique_ptr<Jump> jump;

public:
    Cell() = default;
    void setJump(unique_ptr<Jump> jump)
    {
        this->jump = move(jump);
    }
    const Jump *getJump() const
    {
        return (this->jump).get();
    }
};

class Board
{
private:
    vector<vector<unique_ptr<Cell>>> board;
    int size;

public:
    Board(int size) : size(size)
    {
        board.resize(size);
        for (int i = 0; i < size; ++i)
        {
            board[i].resize(size);
            for (int j = 0; j < size; ++j)
            {
                board[i][j] = make_unique<Cell>();
            }
        }
    }
    
    int getSize() const
    {
        return this->size;
    }

    const Cell *getCell(int position) const
    {
        int idx = position - 1; 
        int x = idx / size;
        int y = idx % size;
        if (x < 0 || x >= size || y < 0 || y >= size)
        {
            return nullptr;
        }
        return board[x][y].get();
    }

    void setCellJump(int position, unique_ptr<Jump> jump)
    {
        int idx = position - 1; 
        int row = idx / size;
        int col = idx % size;
        if (row >= 0 && row < size && col >= 0 && col < size)
        {
            board[row][col]->setJump(move(jump));
        }
    }
};
class Game
{
private:
    unique_ptr<Dice> dice;
    deque<unique_ptr<Player>> players;
    unique_ptr<Board> board;
    bool isFinish = false;

public:
    Game(unique_ptr<Dice> dice, unique_ptr<Board> board) : dice(std::move(dice)), board(std::move(board)) {}

    void addPlayer(unique_ptr<Player> player)
    {
        players.push_back(move(player));
    }

    void startGame()
    {
        if (players.size() == 0)
        {
            cout << "No players are added in the game\n";
            return;
        }
        int sizeOfBoard = board->getSize();
        int winningPosition = sizeOfBoard * sizeOfBoard; 

        while (!isFinish)
        {
            unique_ptr<Player> player = move(players.front());
            players.pop_front();

            int nextPosition = dice->rollDice();
            int newPosition = player->getPosition() + nextPosition;

            if (newPosition > winningPosition)
            {
                cout << "Try rolling the dice again \n";
                players.push_front(move(player));
            }
            else
            {
                const Cell *cell = board->getCell(newPosition);

                if (cell == nullptr)
                {
                    cout << "Try rolling the dice again \n";
                    players.push_front(move(player));
                }
                else
                {
                    if (cell->getJump() != nullptr)
                    {
                        newPosition = cell->getJump()->getEnd();
                    }

                    cout << "NEW position of player " << player->getName() << " is " << newPosition << endl;
                    player->setPosition(newPosition);

                    if (newPosition == winningPosition)
                    {
                        cout << "Player " << player->getName() << " won the game\n";
                        isFinish = true;
                    }
                    else
                    {
                        players.push_back(move(player));
                    }
                }
            }
        }
    }
};

int main()
{
    srand(time(0));

    auto board = make_unique<Board>(10);

    board->setCellJump(3, make_unique<Jump>(3, 22));   // Ladder
    board->setCellJump(95, make_unique<Jump>(95, 12)); // Snake
    board->setCellJump(20, make_unique<Jump>(20, 45)); // Ladder

    auto dice = make_unique<Dice>(1);

    Game game(move(dice), move(board));

    game.addPlayer(make_unique<Player>(1, "Alice"));
    game.addPlayer(make_unique<Player>(2, "Bob"));

    cout << "--- Snake and Ladders Game Started ---\n";
    game.startGame();

    return 0;
}