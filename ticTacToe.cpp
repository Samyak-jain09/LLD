#include <bits/stdc++.h>
using namespace std;

enum Pieces {
    EMPTY,
    O,
    X
};

class PlayingPiece {
public:
    PlayingPiece(Pieces type) : type(type) {}
    virtual ~PlayingPiece() = default;

    virtual void printPiece() = 0;

    Pieces getType() {
        return type;
    }

private:
    Pieces type;
};

class PlayingPieceEmpty : public PlayingPiece {
public:
    PlayingPieceEmpty() : PlayingPiece(Pieces::EMPTY) {}

    void printPiece() override {
        cout << " ";
    }
};

class PlayingPieceX : public PlayingPiece {
public:
    PlayingPieceX() : PlayingPiece(Pieces::X) {}

    void printPiece() override {
        cout << "X";
    }
};

class PlayingPieceO : public PlayingPiece {
public:
    PlayingPieceO() : PlayingPiece(Pieces::O) {}

    void printPiece() override {
        cout << "O";
    }
};

class Board {
public:
    Board(int size)
        : size(size),
          board(size, vector<shared_ptr<PlayingPiece>>(size)) {

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                board[i][j] = make_shared<PlayingPieceEmpty>();
    }

    void printBoard() {
        cout << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                board[i][j]->printPiece();
                if (j != size - 1)
                    cout << " | ";
            }
            cout << endl;
        }
        cout << endl;
    }

    vector<pair<int, int>> getFreeCells() {
        vector<pair<int, int>> cells;

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (board[i][j]->getType() == Pieces::EMPTY)
                    cells.push_back({i, j});

        return cells;
    }

    bool addPiece(int row, int col, shared_ptr<PlayingPiece> piece) {

        if (row < 0 || row >= size || col < 0 || col >= size)
            return false;

        if (board[row][col]->getType() != Pieces::EMPTY)
            return false;

        board[row][col] = piece;
        return true;
    }

    int getSize() {
        return size;
    }

    shared_ptr<PlayingPiece> getBoardPiece(int row, int col) {
        return board[row][col];
    }

private:
    int size;
    vector<vector<shared_ptr<PlayingPiece>>> board;
};

class Player {
public:
    Player(string name, shared_ptr<PlayingPiece> piece)
        : name(name), piece(piece) {}

    string getName() {
        return name;
    }

    shared_ptr<PlayingPiece> getPiece() {
        return piece;
    }

private:
    string name;
    shared_ptr<PlayingPiece> piece;
};

class Game {
public:
    void initializeGame(deque<Player> players, int size) {
        this->players = players;
        board = make_unique<Board>(size);
    }

    string start() {

        while (true) {

            if (board->getFreeCells().empty())
                return "Tie";

            Player playerTurn = players.front();
            players.pop_front();

            board->printBoard();

            cout << "Player " << playerTurn.getName()
                 << " enter row and column : ";

            int row, col;
            cin >> row >> col;

            if (!board->addPiece(row, col, playerTurn.getPiece())) {

                cout << "Invalid Move!\n";
                players.push_front(playerTurn);
                continue;
            }

            if (isWinner(row, col, playerTurn.getPiece()))
                return playerTurn.getName();

            players.push_back(playerTurn);
        }
    }

private:
    deque<Player> players;
    unique_ptr<Board> board;

    bool isWinner(int row, int col,
                  shared_ptr<PlayingPiece> piece) {

        bool rowMatch = true;
        bool colMatch = true;
        bool diagMatch = true;
        bool antiDiagMatch = true;

        Pieces type = piece->getType();

        for (int i = 0; i < board->getSize(); i++) {
            if (board->getBoardPiece(row, i)->getType() != type)
                rowMatch = false;
        }

        for (int i = 0; i < board->getSize(); i++) {
            if (board->getBoardPiece(i, col)->getType() != type)
                colMatch = false;
        }

        for (int i = 0; i < board->getSize(); i++) {
            if (board->getBoardPiece(i, i)->getType() != type)
                diagMatch = false;
        }

        for (int i = 0; i < board->getSize(); i++) {
            if (board->getBoardPiece(i,
                    board->getSize() - 1 - i)->getType() != type)
                antiDiagMatch = false;
        }

        return rowMatch || colMatch || diagMatch || antiDiagMatch;
    }
};

int main() {

    auto pieceO = make_shared<PlayingPieceO>();
    auto pieceX = make_shared<PlayingPieceX>();

    Player player1("Player1", pieceO);
    Player player2("Player2", pieceX);

    Game game;
    game.initializeGame({player1, player2}, 3);

    string winner = game.start();

    if (winner == "Tie")
        cout << "Match Tied\n";
    else
        cout << winner << " wins!\n";

    return 0;
}