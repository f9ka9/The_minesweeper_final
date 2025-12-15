#include "The_minesweeper_heder/MenuWindow.h"
#include "The_minesweeper_heder/MainWindow.h"
#include <iostream>
#include <memory>
using namespace std;

int main() {
    try {
        // Инициализация случайных чисел
        srand(static_cast<unsigned>(time(nullptr)));
        
        // Создаем меню
        unique_ptr<MenuWindow> menu = make_unique<MenuWindow>(
            Point(550, 200), 300, 550, "Сапер - Меню"  
        );
        
        // Показываем меню
        menu->show();
        
        // Главный цикл FLTK
        while (menu->shown()) {
            Fl::wait();
            
            if (menu->isGameStarted()) {
                // Получаем настройки
                MenuWindow::GameSettings settings = menu->getSettings();
                
                // Скрываем меню
                menu->hide();
                
                // Вычисляем размеры окна
                const int CELL_SIZE = 30;
                const int MARGIN_X = 40;
                const int MARGIN_Y_TOP = 140;    // Увеличен отступ сверху для кнопок и текстов
                const int MARGIN_Y_BOTTOM = 40;  // Отступ снизу
                
                int boardWidth = settings.cols * CELL_SIZE;
                int boardHeight = settings.rows * CELL_SIZE;
                int windowWidth = MARGIN_X * 2 + boardWidth;
                int windowHeight = MARGIN_Y_TOP + boardHeight + MARGIN_Y_BOTTOM;
                
                // Ограничиваем максимальный размер
                if (windowWidth > 1200) windowWidth = 1200;
                if (windowHeight > 800) windowHeight = 800;
                
                // Создаем игровое окно
                unique_ptr<MainWindow> game = make_unique<MainWindow>(
                    Point(150, 100), 
                    windowWidth, 
                    windowHeight, 
                    "Сапер",
                    settings.rows,
                    settings.cols,
                    settings.mines,
                    settings.countdownMode,
                    settings.countdownTime
                );
                
                // Показываем игровое окно
                game->show();
                
                // Ожидаем закрытия игрового окна
                while (game->shown()) {
                    Fl::wait();
                }
                
                // После закрытия игрового окна показываем меню снова
                menu->resetGameStarted();
                menu->show();
            }
        }
        
        return 0;
        
    } catch (exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Неизвестная ошибка!" << endl;
        return 2;
    }
}