// a RSDL wrapper
#include "../liberaries/rsdl.hpp"
#include <string>
#include <chrono>

using namespace std;

#ifndef __BRSDL_HPP__
    #define __BRSDL_HPP__

    class Physical_body{
    public:
        Physical_body(Point _pivot, int _width, int _height);

        Point top_left_point(int stage_y);

        Rectangle get_rectangle(int stage_y);
        Rectangle get_bottom(int width);
        
        void move(int dx, int dy);


        int get_width(){ return width; }
        int get_height(){ return height; }

        bool touched_page_horizontal_edge();
        bool passed_page_horizontal_edge();

        bool is_hitting(Rectangle other_object);

        void return_pivot_to_page();
        int get_y(){ return pivot.y; }
        
    private:

        bool is_hitting_1D(int first_mid, int second_mid, int total_widths);

        Point pivot;
        int width;
        int height;
    };


    int passed_time( chrono::high_resolution_clock::time_point from);

    class Key_timer{
    public:
        Key_timer();

        void key_pressed();

        void key_released();

        int get_total_time();

        void clear();

    private:
        int total_press_time;
        chrono::high_resolution_clock::time_point  last_press;
        bool key_is_released;

    };


    

#endif