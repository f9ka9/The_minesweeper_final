#pragma once
#include "Graph_lib/Graph.h"
#include <FL/fl_ask.H>
#include <functional>

// Универсальная кнопка с поддержкой lambda-колбэков
struct MyButton : public Graph_lib::Button {
    using Graph_lib::Button::Button;
    
    std::function<void()> on_click;
    
    void set_fill_color(Graph_lib::Color color) {
        if (pw) {
            pw->color(color.as_int());
        }
    }
};

// Интерфейс для FLTK функций
class FltkInterface {
public:
    static int event_x() { return Fl::event_x(); }
    static int event_y() { return Fl::event_y(); }
    static int event_button() { return Fl::event_button(); }

    static const int LEFT_MOUSE = FL_LEFT_MOUSE;
    static const int RIGHT_MOUSE = FL_RIGHT_MOUSE;
    static const int PUSH = FL_PUSH;
    
    static bool isRightMouse() { return event_button() == RIGHT_MOUSE; }
    static bool isLeftMouse() { return event_button() == LEFT_MOUSE; }
    
    static void showMessage(const char* message) { fl_message("%s", message); }
    
    static void setFixedSize(Fl_Window* window, int width, int height) {
        if (window) {
            window->size_range(width, height, width, height);
            window->resizable(nullptr);
        }
    }
    
    // Обертка для timeout
    static void add_timeout(double t, Fl_Timeout_Handler cb, void* arg) {
        Fl::add_timeout(t, cb, arg);
    }
    
    static void repeat_timeout(double t, Fl_Timeout_Handler cb, void* arg) {
        Fl::repeat_timeout(t, cb, arg);
    }
    
    static void remove_timeout(Fl_Timeout_Handler cb, void* arg) {
        Fl::remove_timeout(cb, arg);
    }
};

