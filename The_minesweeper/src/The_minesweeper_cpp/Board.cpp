#include "The_minesweeper_heder/Board.h"

//Конструктор Board
Board::Board(int r, int c, int m) : rows(r), cols(c), mines(m), grid(r, vector<Cell>(c)) {
    srand(static_cast<unsigned>(time(nullptr)));
}

//Размещает мины, гарантируя, что первая открытая клетка и её соседи безопасны
void Board::placeMines(int safeRow, int safeCol) {
    vector<pair<int, int>> allPositions;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (abs(r - safeRow) <= 1 && abs(c - safeCol) <= 1) {
                continue;
            }
            allPositions.push_back({r, c});
        }
    }
    
    random_shuffle(allPositions.begin(), allPositions.end());
    
    for (int i = 0; i < mines; i++) {
        int r = allPositions[i].first;
        int c = allPositions[i].second;
        grid[r][c].isMine = true;
    }
}

/*
void Board::placeMines(int safeRow, int safeCol)
{
    int placed = 0;

    while (placed < mines)
    {
        int r = rand() % rows;
        int c = rand() % cols;

        if (r == safeRow && c == safeCol)
            continue;

        if (grid[r][c].isMine)
            continue;

        grid[r][c].isMine = true;
        placed++;
    }
}
*/

//Рекурсивно открывает пустые клетки
void Board::floodFill(int r, int c) {
    static int dr[8] = {-1,-1,-1,0,0,1,1,1};
    static int dc[8] = {-1,0,1,-1,1,-1,0,1};

    for (int i = 0; i < 8; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (inBounds(nr,nc)) {
            Cell& next = grid[nr][nc];
            if (!next.isRevealed && !next.isMine) {
                next.isRevealed = true;
                if (next.neighboringMines == 0)
                    floodFill(nr, nc);
            }
        }
    }
}

//Считает количество мин вокруг каждой клетки
void Board::calculateNeighborMines() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c].isMine) {
                grid[r][c].neighboringMines = -1;
                continue;
            }
            
            int count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (inBounds(nr, nc) && grid[nr][nc].isMine) {
                        count++;
                    }
                }
            }
            grid[r][c].neighboringMines = count;
        }
    }
}

//Полный сброс поля
void Board::reset(int safeRow, int safeCol) {
    // Очищаем поле
    for (auto& row : grid)
        for (auto& cell : row)
            cell = Cell();
    
    // Размещаем мины
    placeMines(safeRow, safeCol);
    
    // Вычисляем соседей
    calculateNeighborMines();
}

//Проверка выхода за границы массива
bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

//Открытие клетки
bool Board::reveal(int r, int c) {
    if (!inBounds(r,c)) return false;
    Cell& cell = grid[r][c];
    if (cell.isRevealed || cell.isFlagged) return false;

    cell.isRevealed = true;
    if (cell.isMine) return true;
    if (cell.neighboringMines == 0) floodFill(r,c);
    return false;
}

//Работа с флагами 
bool Board::toggleFlag(int r, int c) {
    if (!inBounds(r, c) || grid[r][c].isRevealed)
        return false;
    
    // Если клетка уже помечена флагом - снимаем флаг
    if (grid[r][c].isFlagged) {
        grid[r][c].isFlagged = false;
        return true;
    }
    
    // Проверяем, сколько флагов уже поставлено
    int currentFlags = countFlags();
    
    // Если уже поставлено максимальное количество флагов - не ставим новый
    if (currentFlags >= mines) {
        return false; 
    }
    
    // Ставим флаг
    grid[r][c].isFlagged = true;
    return true;
}

//Проверка победы
bool Board::checkWin() const {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            const Cell& cell = grid[r][c];
            if (!cell.isMine && !cell.isRevealed)
                return false;
        }
    }
    return true;
}
//Счетчик флагов 
int Board::countFlags() const {
    int count = 0;
    for (const auto& row : grid)
        for (const auto& cell : row)
            if (cell.isFlagged)
                count++;
    return count;
}

//Счетчик открытых клеток 
int Board::countRevealed() const {
    int count = 0;
    for (const auto& row : grid)
        for (const auto& cell : row)
            if (cell.isRevealed)
                count++;
    return count;
}