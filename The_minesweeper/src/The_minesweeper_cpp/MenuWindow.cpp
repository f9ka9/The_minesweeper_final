#include "The_minesweeper_heder/MenuWindow.h"
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

MenuWindow::MenuWindow(Point xy, int w, int h, const string& title)
    : Window(xy, w, h, title),
      windowWidth(w), windowHeight(h),
      centerX((w - BTN_WIDTH) / 2),
      // Заголовок 
    
      titleText(Point(centerX, MARGIN_TOP), "САПЕР"),
      // Текст выбора сложности 
      difficultyText(Point(centerX, MARGIN_TOP + 50), "Выберите сложность:"),
      // Кнопки сложности 
      beginnerBtn(Point(centerX, MARGIN_TOP + 80), BTN_WIDTH, BTN_HEIGHT, "Новичок (9x9, 10 мин)", cbBeginner),
      intermediateBtn(Point(centerX, MARGIN_TOP + 80 + BTN_HEIGHT + SPACING), BTN_WIDTH, BTN_HEIGHT, "Любитель (16x16, 40 мин)", cbIntermediate),
      expertBtn(Point(centerX, MARGIN_TOP + 80 + 2*(BTN_HEIGHT + SPACING)), BTN_WIDTH, BTN_HEIGHT, "Эксперт (16x30, 99 мин)", cbExpert),
      // Текст режима игры 
      modeText(Point(centerX, MARGIN_TOP + 80 + 3*(BTN_HEIGHT + SPACING) + SECTION_SPACING), "Режим игры:"),
      // Кнопки режимов 
      normalModeBtn(Point(centerX, MARGIN_TOP + 80 + 3*(BTN_HEIGHT + SPACING) + SECTION_SPACING + 30), BTN_WIDTH, BTN_HEIGHT, "Стандартный режим", cbNormalMode),
      countdownModeBtn(Point(centerX, MARGIN_TOP + 80 + 3*(BTN_HEIGHT + SPACING) + SECTION_SPACING + 30 + BTN_HEIGHT + SPACING), BTN_WIDTH, BTN_HEIGHT, "Обратный отсчет", cbCountdownMode),
      // Поле времени 
      countdownText(Point(centerX, MARGIN_TOP + 80 + 3*(BTN_HEIGHT + SPACING) + 2*SECTION_SPACING + 30 + 2*BTN_HEIGHT), "Время (секунды):"),
      timeBox(Point(centerX, MARGIN_TOP + 80 + 3*(BTN_HEIGHT + SPACING) + 2*SECTION_SPACING + 30 + 2*BTN_HEIGHT + SPACING), BTN_WIDTH, BTN_HEIGHT - 10, ""),
      // Кнопки внизу 
      startBtn(Point(centerX, MARGIN_TOP + 80 + 3*(BTN_HEIGHT + SPACING) + 2*SECTION_SPACING + 30 + 3*BTN_HEIGHT + SPACING), BTN_WIDTH, BTN_HEIGHT, "Начать игру", cbStart)
{
    //Настройка логики кнопок
    setupCallbacks();
    //Размещение элементов
    setupLayout();
    //Стили
    setupStyles();
    
    //Значение по умолчанию
    //Сложность: новичок
    onBeginner();
    //Режим: обычный
    onNormalMode();

    //Запрещаем изменение размера окна
    FltkInterface::setFixedSize(this, windowWidth, windowHeight);
}

MenuWindow::~MenuWindow() {}

void MenuWindow::setupCallbacks() {
    //Лямбды вызывают метод класса MyButton
    beginnerBtn.on_click = [this]() { onBeginner(); };
    intermediateBtn.on_click = [this]() { onIntermediate(); };
    expertBtn.on_click = [this]() { onExpert(); };
    normalModeBtn.on_click = [this]() { onNormalMode(); };
    countdownModeBtn.on_click = [this]() { onCountdownMode(); };
    startBtn.on_click = [this]() { onStart(); };
}


//Добавляем элементы в окно
void MenuWindow::setupLayout() {
    attach(titleText);
    attach(difficultyText);
    attach(beginnerBtn);
    attach(intermediateBtn);
    attach(expertBtn);
    
    attach(modeText);
    attach(normalModeBtn);
    attach(countdownModeBtn);
    
    attach(countdownText);
    attach(timeBox); 
    
    attach(startBtn);
    
    // Скрываем поле времени по умолчанию
    showCountdownInput(false);
}

//Стиль текста 
void MenuWindow::setupStyles() {
    titleText.set_font_size(24);
    titleText.set_color(Color::dark_blue);
    
    difficultyText.set_font_size(16);
    difficultyText.set_color(Color::dark_red);
    
    modeText.set_font_size(16);
    modeText.set_color(Color::dark_red);
    
    countdownText.set_font_size(14);
    countdownText.set_color(Color::invisible);
    

    timeBox.set_color(Color::black);
}

// Колбэки
void MenuWindow::cbBeginner(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

void MenuWindow::cbIntermediate(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

void MenuWindow::cbExpert(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

void MenuWindow::cbNormalMode(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

void MenuWindow::cbCountdownMode(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

void MenuWindow::cbStart(Address, Address pw) {
    MyButton& btn = reference_to<MyButton>(pw);
    if (btn.on_click) btn.on_click();
}

// Обработчики
void MenuWindow::onBeginner() {
    settings.rows = 9;
    settings.cols = 9;
    settings.mines = 10;
    //Подсвечиваем выбранную кнопку
    updateDifficultyButtons(&beginnerBtn);
}

void MenuWindow::onIntermediate() {
    settings.rows = 16;
    settings.cols = 16;
    settings.mines = 40;
    updateDifficultyButtons(&intermediateBtn);
}

void MenuWindow::onExpert() {
    settings.rows = 16;
    settings.cols = 30;
    settings.mines = 99;
    updateDifficultyButtons(&expertBtn);
}


//Выбор режима

void MenuWindow::onNormalMode() {
    settings.countdownMode = false;
    updateModeButtons(&normalModeBtn);
    showCountdownInput(false); // Скрываем поле ввода
}

void MenuWindow::onCountdownMode() {
    settings.countdownMode = true;
    updateModeButtons(&countdownModeBtn);
    showCountdownInput(true); // Показываем поле ввода
}

//Старт игры
void MenuWindow::onStart() {
    if (settings.countdownMode) {
        //Получаем введённое время
        int time = timeBox.get_int();
        // Проверка времени (от 30 секунд до 30 минут)
        if (time < 30 || time > 1800) {
            FltkInterface::showMessage("Время должно быть от 30 до 1800 секунд (30 минут)!");
            return;
        }
        settings.countdownTime = time;
    }
    
    gameStarted = true;
    hide();
}

//Подсвечивает выбранную кнопку сложности
void MenuWindow::updateDifficultyButtons(MyButton* selected) {
    beginnerBtn.set_fill_color(Color::darker_gray);
    intermediateBtn.set_fill_color(Color::darker_gray);
    expertBtn.set_fill_color(Color::darker_gray);
    if (selected) {
        selected->set_fill_color(Color::yellow);
        selectedDifficulty = selected;
    }
    redraw();
}

//Подсвечивает выбранный режим игры
void MenuWindow::updateModeButtons(MyButton* selected) {
    normalModeBtn.set_fill_color(Color::darker_gray);
    countdownModeBtn.set_fill_color(Color::darker_gray);
    
    if (selected) {
        selected->set_fill_color(Color::cyan);
        selectedMode = selected;
    }
    redraw();
}

//Показывает или скрывает поле ввода времени
void MenuWindow::showCountdownInput(bool show) {
    if (show) {
        attach(countdownText);
        attach(timeBox);
        countdownText.set_color(Color::black); 
        timeBox.show();
    } else {
        detach(countdownText);
        timeBox.hide();
        detach(timeBox);
    }
    redraw();
}