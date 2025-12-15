#pragma once
#include "Graph_lib/GUI.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/Window.h"
#include "Update_Graph_lib.h"
#include <string>
using namespace Graph_lib;
using namespace std;

struct MenuWindow : public Window {
public:
    struct GameSettings {
        int rows = 9;
        int cols = 9;
        int mines = 10;
        bool countdownMode = false;
        int countdownTime = 300; 
    };
    
    MenuWindow(Point xy, int w, int h, const string& title);
    ~MenuWindow();
    
    GameSettings getSettings() const { return settings; }
    bool isGameStarted() const { return gameStarted; }
    void resetGameStarted() { gameStarted = false; }
    
private:
    static constexpr int BTN_WIDTH = 220;
    static constexpr int BTN_HEIGHT = 40;
    static constexpr int SPACING = 10;
    static constexpr int MARGIN_TOP = 30;
    static constexpr int SECTION_SPACING = 30;

    int windowWidth, windowHeight, centerX;
    GameSettings settings;
    bool gameStarted = false;
    
    // Текстовые элементы
    Text titleText;
    Text difficultyText;
    Text modeText;
    Text countdownText;  
    
    // Кнопки сложности 
    MyButton beginnerBtn;
    MyButton intermediateBtn;
    MyButton expertBtn;
    
    // Поле ввода времени 
    In_box timeBox;
    
    // Кнопки режима
    MyButton normalModeBtn;
    MyButton countdownModeBtn;
    
    // Основные кнопки
    MyButton startBtn;

    // Выбранные кнопки
    MyButton* selectedDifficulty = nullptr;
    MyButton* selectedMode = nullptr;
    
    // Методы инициализации
    void setupCallbacks();
    void setupLayout();
    void setupStyles();
    
    // Обработчики (без кастомных настроек поля)
    void onBeginner();
    void onIntermediate();
    void onExpert();
    void onNormalMode();
    void onCountdownMode();
    void onStart();
    void onExit();
    
    // Вспомогательные методы (убираем showCustomInputs, validateInputs)
    void updateDifficultyButtons(MyButton* selected);
    void updateModeButtons(MyButton* selected);
    void showCountdownInput(bool show);  // Оставляем только для времени
    
    // Колбэки (без кастомных настроек поля)
    static void cbBeginner(Address, Address pw);
    static void cbIntermediate(Address, Address pw);
    static void cbExpert(Address, Address pw);
    static void cbNormalMode(Address, Address pw);
    static void cbCountdownMode(Address, Address pw);
    static void cbStart(Address, Address pw);
    static void cbExit(Address, Address pw);
};