#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

struct Cell {
    bool isMine = false;// true — в клетке есть мина, false — безопасная
    bool isRevealed = false;// true — клетка открыта игроком
    bool isFlagged = false;// true — игрок поставил флаг 
    int neighboringMines = 0;// количество мин в соседних 8 клетках
};


class Board {
private:
    void placeMines(int safeRow, int safeCol);
    void floodFill(int r, int c);
    void calculateNeighborMines();

public:
    int rows, cols, mines;
    vector<vector<Cell>> grid;

    Board(int r, int c, int m);
    void reset(int safeRow = -1, int safeCol = -1);
    bool inBounds(int r, int c) const;
    bool reveal(int r, int c);
    bool toggleFlag(int r, int c); 
    bool checkWin() const;
    int countFlags() const;
    int countRevealed() const;
};