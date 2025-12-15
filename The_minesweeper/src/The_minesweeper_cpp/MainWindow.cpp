#include "The_minesweeper_heder/MainWindow.h"
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

MainWindow::MainWindow(Point xy, int w, int h, const string& title, int rows, int cols, int mines, bool countdownMode, int countdownTime)
    : Window(xy, w, h, title),
      rows(rows), cols(cols), mines(mines),
      countdownMode(countdownMode),
      initialTime(countdownTime),
      board(rows, cols, mines),
      newGameButton(Point((w - 230) / 2, 20), 120, 30, "Новая игра", cbNewGame),
      menuButton(Point((w - 230) / 2 + 120 + 10, 20), 100, 30, "Меню", cbMenu),
      gameTime(0),
      firstClick(true),
      gameOver(false),
      gameWon(false),
      timerStarted(false),
      timerActive(false)
{

    int centerX = w / 2;
    int newGameBtnX = (w - 230) / 2;
    int menuBtnX = newGameBtnX + 120 + 10;
    timeText = new Text(Point(newGameBtnX + 60 - 40, 70), ""); 
    mineText = new Text(Point(menuBtnX + 50 - 25, 70), "");
    statusText = new Text(Point(centerX - 50, 100), "");

    int fieldWidth = cols * CELL_SIZE;
    int fieldHeight = rows * CELL_SIZE;
    origin_x = (w - fieldWidth) / 2; 
    origin_y = 130; 
    
    int bottomMargin = 30; 
    int availableHeight = h - origin_y - bottomMargin;
    

    setupUI();
    setupCallbacks();
    
    board.reset();
    updateStatus();
    drawBoard();
    
    FltkInterface::setFixedSize(this, w, h);
}

MainWindow::~MainWindow() {
    stopTimer();
    
    delete timeText;
    delete mineText;
    delete statusText;
}

void MainWindow::setupUI() {
    // Прикрепляем кнопки
    attach(newGameButton);
    attach(menuButton);
    
    // Настраиваем тексты
    timeText->set_font_size(16);
    timeText->set_color(Color::dark_blue);
    attach(*timeText);
    
    mineText->set_font_size(16);
    mineText->set_color(Color::dark_red);
    attach(*mineText);
    
    statusText->set_font_size(14);
    statusText->set_color(Color::black);
    attach(*statusText);
    
    // Создаем клетки
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int x = origin_x + c * CELL_SIZE;
            int y = origin_y + r * CELL_SIZE;
            
            Rectangle* rect = new Rectangle(Point(x, y), CELL_SIZE, CELL_SIZE);
            rect->set_color(Color::black);
            rect->set_fill_color(Color::darker_gray);
            cellRects.push_back(rect);
            attach(*rect);
            
            Text* t = new Text(Point(x + CELL_SIZE/3, y + CELL_SIZE*2/3), "");
            t->set_font_size(14);
            t->set_color(Color::black);
            cellTexts.push_back(t);
            attach(*t);
        }
    }
}


void MainWindow::setupCallbacks() {
    newGameButton.on_click = [this]() { onNewGame(); };
    menuButton.on_click = [this]() { onMenu(); };
}

void MainWindow::drawBoard() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            updateCell(r, c);
        }
    }
    redraw();
}

//Обновление  графических свойств одной кнопки 
void MainWindow::updateCell(int r, int c) {
    int index = r * cols + c;
    if (index < 0 || index >= cellRects.size()) return;
    
    Cell& cell = board.grid[r][c];
    Rectangle& rect = cellRects[index];
    Text& text = cellTexts[index];
    
    if (cell.isRevealed) {
        rect.set_fill_color(Color::white);
        if (cell.isMine) {
            text.set_label("💣");
            text.set_color(Color::red);
        } else if (cell.neighboringMines > 0) {
            text.set_label(to_string(cell.neighboringMines));
            
            // Цвета цифр 
            switch(cell.neighboringMines) {
                case 1: text.set_color(Color::blue); break;
                case 2: text.set_color(Color::green); break;
                case 3: text.set_color(Color::red); break;
                case 4: text.set_color(Color::dark_blue); break;
                case 5: text.set_color(Color::dark_red); break;
                case 6: text.set_color(Color::dark_cyan); break;
                case 7: text.set_color(Color::black); break;
                case 8: text.set_color(Color::dark_yellow); break;
            }
        } else {
            text.set_label("");
        }
    } else {
        text.set_label(cell.isFlagged ? "🚩" : "");
    }
}

//Обработка клика по клетке
void MainWindow::clickCellAtIndex(int r, int c, bool rightClick) {
    if (gameOver) return;
    if (!board.inBounds(r,c)) return;

    Cell& cell = board.grid[r][c];

    if (rightClick) {
        if (!cell.isRevealed) {
            board.toggleFlag(r,c);
            int flags = board.countFlags();
            int remaining = mines - flags;
            statusText->set_label("Мины: " + to_string(remaining));
        }
        drawBoard();
        return;
    }

    if (firstClick) {
        board.reset(r,c); 
        firstClick = false;
        startTimer();
    }

    if (cell.isFlagged || cell.isRevealed) return;

    bool exploded = board.reveal(r,c);

    if (exploded) {
        for (int rr=0; rr<rows; rr++)
            for (int cc=0; cc<cols; cc++)
                if (board.grid[rr][cc].isMine)
                    board.grid[rr][cc].isRevealed = true;

        gameOver = true;
        statusText->set_label("Статус: Проигрыш");
        drawBoard();
        FltkInterface::showMessage("Вы проиграли!");
        return;
    }

    if (board.checkWin()) {
        gameOver = true;
        statusText->set_label("Статус: Победа");
        drawBoard();
        FltkInterface::showMessage("Вы выиграли!");
        return;
    }

    drawBoard();
}

bool MainWindow::coordsToIndex(int x, int y, int& out_r, int& out_c) {
    if (x < origin_x || y < origin_y) return false;
    out_c = (x - origin_x) / CELL_SIZE;
    out_r = (y - origin_y) / CELL_SIZE;
    return out_r >= 0 && out_r < rows && out_c >= 0 && out_c < cols;
}

void MainWindow::onNewGame() {
    stopTimer();
    
    // Сбрасываем состояние игры
    board.reset();
    firstClick = true;
    gameOver = false;
    gameWon = false;
    gameTime = 0;  // Сбрасываем время
    timerStarted = false;
    
    // Сбрасываем отображение клеток
    for (int i = 0; i < cellRects.size(); ++i) {
        cellRects[i].set_fill_color(Color::darker_gray);
        cellTexts[i].set_label("");
    }
    
    // Обновляем статус и отрисовываем
    updateStatus();
    drawBoard();
    
    
}

void MainWindow::onMenu() {
    hide();
}

void MainWindow::onTimer() {
    if (!timerActive || gameOver) {
        // Если таймер не активен или игра окончена - не обновляем
        return;
    }
    
    if (countdownMode) {
        if (gameTime >= initialTime) {
            endGame(false);  // Время вышло
            return;
        }
    }
    
    gameTime++;
    updateStatus();
    
    // Перезапускаем таймер ТОЛЬКО если игра еще идет
    if (timerActive && !gameOver) {
        FltkInterface::repeat_timeout(1.0, timerCallback, this);
    }
}

void MainWindow::timerCallback(void data) {
    MainWindow* window = static_cast<MainWindow*>(data);
    if (window && window->timerActive && !window->gameOver) {
        window->onTimer();
    }
}

// Запуск таймера
void MainWindow::startTimer() {
    if (!timerActive && !timerStarted) {
        timerActive = true;
        timerStarted = true;
        FltkInterface::add_timeout(1.0, timerCallback, this);
    }
}

// Остановка таймера
void MainWindow::stopTimer() {
    if (timerActive) {
        timerActive = false;
        timerStarted = false;
        FltkInterface::remove_timeout(timerCallback, this);
    }
}

// Колбэки
void MainWindow::cbNewGame(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

void MainWindow::cbMenu(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

int MainWindow::handle(int event) {
    switch(event) {
        case FltkInterface::PUSH: {
            int x = FltkInterface::event_x();
            int y = FltkInterface::event_y();
            bool rightClick = FltkInterface::isRightMouse();
            
            int r, c;
            if (coordsToIndex(x, y, r, c)) {
                clickCellAtIndex(r, c, rightClick);
                return 1;
            }
            break;
        }
    }
    return Window::handle(event);
}

string MainWindow::formatTime(int seconds) const {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    stringstream ss;
    ss << setw(2) << setfill('0') << minutes << ":" 
       << setw(2) << setfill('0') << secs;
    return ss.str();
}

void MainWindow::updateStatus() {
    // Время
    if (countdownMode) {
        int remaining = max(0, initialTime - gameTime);  // Не меньше 0
        timeText->set_label("Время: " + formatTime(remaining));
        if (remaining < 60) {
            timeText->set_color(Color::red);
        } else {
            timeText->set_color(Color::dark_blue);
        }
    } else {
        timeText->set_label("Время: " + formatTime(gameTime));
        timeText->set_color(Color::dark_blue);
    }
    
    // Мины
    int flags = board.countFlags();
    int remaining = mines - flags;
    mineText->set_label("Мины: " + to_string(remaining));
    
    // Статус
    if (gameOver) {
        statusText->set_label(gameWon ? "ПОБЕДА!" : "ПРОИГРЫШ!");
        statusText->set_color(gameWon ? Color::green : Color::red);
    } else {
        statusText->set_label(firstClick ? "Начните игру" : "Игра идет...");
        statusText->set_color(Color::black);
    }
    
    redraw();
}

void MainWindow::endGame(bool win) {
    gameOver = true;
    gameWon = win;
    
    // Останавливаем таймер
    stopTimer();
    
    // Показываем все мины при проигрыше
    if (!win) {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board.grid[r][c].isMine) {
                    board.grid[r][c].isRevealed = true;
                }
            }
        }
    }
    
    drawBoard();
    updateStatus();
    
    // Показываем сообщение
    if (win) {
        FltkInterface::showMessage("🎉 Поздравляем! Вы выиграли!");
    } else {
        if (countdownMode && gameTime >= initialTime) {
            FltkInterface::showMessage("⏰ Время вышло! Вы проиграли.");
        } else {
            FltkInterface::showMessage("💥 Вы наступили на мину! Игра окончена.");
        }
    }
}