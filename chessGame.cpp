#include <bits/stdc++.h>
using namespace std;

class Board;
class Cell;

// ---------------- Piece Hierarchy ----------------
class Piece {
public:
    Piece(bool isWhite) : isWhite(isWhite) {}
    virtual ~Piece() = default;

    virtual bool canMove(Board* board, Cell* from, Cell* to) = 0;
    bool isWhitePiece() const { return isWhite; }

private:
    bool isWhite;
};

// ---------------- Cell ----------------
class Cell {
public:
    Cell(Piece* piece, int x, int y) : piece(piece), x(x), y(y) {}
    void setPiece(Piece* p) { piece = p; }
    Piece* getPiece() const { return piece; }
    int getX() const { return x; }
    int getY() const { return y; }

private:
    Piece* piece;
    int x, y;
};

// ---------------- Board ----------------
class Board {
public:
    Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                cells[i][j] = new Cell(nullptr, i, j);
    }
    ~Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                delete cells[i][j];
    }

    Cell* getCell(int x, int y) { return cells[x][y]; }

    bool move(Cell* from, Cell* to) {
        Piece* piece = from->getPiece();
        if (piece && piece->canMove(this, from, to)) {
            to->setPiece(piece);
            from->setPiece(nullptr);
            return true;
        }
        return false;
    }

private:
    Cell* cells[8][8];
};

// ---------------- Piece Implementations ----------------
class Pawn : public Piece {
public:
    Pawn(bool isWhite) : Piece(isWhite) {}
    bool canMove(Board* board, Cell* from, Cell* to) override {
        int dir = isWhitePiece() ? -1 : 1;
        return (to->getX() == from->getX() + dir && to->getY() == from->getY() && to->getPiece() == nullptr);
    }
};

class Rook : public Piece {
public:
    Rook(bool isWhite) : Piece(isWhite) {}
    bool canMove(Board* board, Cell* from, Cell* to) override {
        return (from->getX() == to->getX() || from->getY() == to->getY());
    }
};

class Knight : public Piece {
public:
    Knight(bool isWhite) : Piece(isWhite) {}
    bool canMove(Board* board, Cell* from, Cell* to) override {
        int dx = abs(from->getX() - to->getX());
        int dy = abs(from->getY() - to->getY());
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }
};

class Bishop : public Piece {
public:
    Bishop(bool isWhite) : Piece(isWhite) {}
    bool canMove(Board* board, Cell* from, Cell* to) override {
        return abs(from->getX() - to->getX()) == abs(from->getY() - to->getY());
    }
};

class Queen : public Piece {
public:
    Queen(bool isWhite) : Piece(isWhite) {}
    bool canMove(Board* board, Cell* from, Cell* to) override {
        int dx = abs(from->getX() - to->getX());
        int dy = abs(from->getY() - to->getY());
        return (from->getX() == to->getX() || from->getY() == to->getY() || dx == dy);
    }
};

class King : public Piece {
public:
    King(bool isWhite) : Piece(isWhite) {}
    bool canMove(Board* board, Cell* from, Cell* to) override {
        int dx = abs(from->getX() - to->getX());
        int dy = abs(from->getY() - to->getY());
        return dx <= 1 && dy <= 1;
    }
};

// ---------------- Player ----------------
class Player {
public:
    Player(string name, bool isWhite) : name(name), isWhite(isWhite) {}
    string getName() const { return name; }
    bool getIsWhite() const { return isWhite; }

private:
    string name;
    bool isWhite;
};

// ---------------- Game ----------------
enum class GameState { NOT_STARTED, IN_PROGRESS, CHECKMATE, TIE };

class Game {
public:
    Game(Player* p1, Player* p2) : player1(p1), player2(p2), currentState(GameState::NOT_STARTED) {
        board = new Board();
    }
    ~Game() { delete board; }

    void start() {
        currentState = GameState::IN_PROGRESS;
        cout << "Game started between " << player1->getName() << " and " << player2->getName() << endl;

        // Demo setup: place a few pieces
        board->getCell(6,0)->setPiece(new Pawn(true));
        board->getCell(1,0)->setPiece(new Pawn(false));
        board->getCell(7,0)->setPiece(new Rook(true));
        board->getCell(7,1)->setPiece(new Knight(true));
        board->getCell(7,2)->setPiece(new Bishop(true));
        board->getCell(7,3)->setPiece(new Queen(true));
        board->getCell(7,4)->setPiece(new King(true));

        // Demo moves
        cout << "Trying pawn move..." << endl;
        if(board->move(board->getCell(6,0), board->getCell(5,0)))
            cout << "Pawn moved!" << endl;

        cout << "Trying knight move..." << endl;
        if(board->move(board->getCell(7,1), board->getCell(5,2)))
            cout << "Knight moved!" << endl;

        cout << "Trying bishop move..." << endl;
        if(board->move(board->getCell(7,2), board->getCell(5,4)))
            cout << "Bishop moved!" << endl;

        currentState = GameState::TIE; // end demo
    }

private:
    Player* player1;
    Player* player2;
    Board* board;
    GameState currentState;
};

// ---------------- Main ----------------
int main() {
    Player* player1 = new Player("Alice", true);
    Player* player2 = new Player("Bob", false);
    Game* game = new Game(player1, player2);
    game->start();
    delete player1;
    delete player2;
    delete game;
    return 0;
}
