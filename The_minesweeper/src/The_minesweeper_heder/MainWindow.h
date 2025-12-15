#pragma once
#include "Graph_lib/GUI.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/Window.h"
#include "Board.h"
#include "Update_Graph_lib.h"
#include <vector>
#include <string>
#include <chrono>
using namespace Graph_lib;
using namespace std;

struct MainWindow : public Window {
public:
    MainWindow(Point xy, int w, int h, const string& title,
               int rows, int cols, int mines, bool countdownMode = false, int countdownTime = 300);
    ~MainWindow();
    
private:
    static constexpr int CELL_SIZE = 30;
    static constexpr int MARGIN_X = 20;     
    static constexpr int MARGIN_Y = 120;    
    
    // Игровые параметры
    int rows, cols, mines;
    bool countdownMode;
    int initialTime; // начальное время для обратного отсчета
    Board board;
    
    // Состояние игры
    bool firstClick = true;
    bool gameOver = false;
    bool gameWon = false;
    bool timerStarted = false;  // Флаг, что таймер запущен
    int gameTime = 0; // в секундах
    
    // UI элементы
    int origin_x = 20;
    int origin_y = 80;

    MyButton newGameButton;
    MyButton menuButton; 
    
    Text* timeText;
    Text* mineText;
    Text* statusText;
    
    Vector_ref<Rectangle> cellRects;
    Vector_ref<Text> cellTexts;
    
    // Методы
    void setupUI();
    void setupCallbacks();
    void drawBoard();
    void updateCell(int r, int c);
    void clickCellAtIndex(int r, int c, bool rightClick);
    bool coordsToIndex(int x, int y, int& out_r, int& out_c);
    
    // Обработчики
    void onNewGame();
    void onMenu();
    void onTimer();
    
    // Таймер
    static void timerCallback(void* data);
    bool timerActive = false;
    
    // Запуск таймера
    void startTimer();
    void stopTimer();
    
    // Колбэки 
    static void cbNewGame(Address, Address pw);
    static void cbMenu(Address, Address pw);
    
    // Обработка событий
    int handle(int event) override;
    
    // Вспомогательные
    string formatTime(int seconds) const;
    void updateStatus();
    void endGame(bool win);
};