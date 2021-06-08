#include "brsdl.hpp"
#include "game_consts.hpp"
#include <chrono>

using namespace std::chrono;

//physical_body
Physical_body::Physical_body(Point _pivot, int _width, int _height){
    pivot = _pivot;
    height = _height;
    width = _width;
}

Point Physical_body::top_left_point(int stage_view_height){ return pivot + Point(-width/2, height/2 - stage_view_height);}


Rectangle Physical_body::get_rectangle(int stage_view_height){ return Rectangle(top_left_point(stage_view_height), width, height); }

Rectangle Physical_body::get_bottom(int block_width){
    return Rectangle(pivot.x - width/2, pivot.y - height/2 + block_width, width, block_width);
}

bool Physical_body::touched_page_horizontal_edge(){ return pivot.x + width/2 > PAGE_WIDTH || pivot.x - width/2 < 0; }
bool Physical_body::passed_page_horizontal_edge(){ return pivot.x > PAGE_WIDTH || pivot.x < 0; }

void Physical_body::move(int dx, int dy){ 
    pivot.x += dx; 
    pivot.y += dy;
}

void Physical_body::return_pivot_to_page(){
    while( pivot.x < 0 || pivot.x > PAGE_WIDTH){
        if( pivot.x < 0){
            pivot.x += PAGE_WIDTH;
        }else if( pivot.x > PAGE_WIDTH ){
            pivot.x -= PAGE_WIDTH;
        }
    }
}

bool Physical_body::is_hitting(Rectangle other_object){
    return is_hitting_1D(other_object.x + (other_object.w/2), pivot.x, width + other_object.w) &&    
           is_hitting_1D(other_object.y - (other_object.h/2), pivot.y, height + other_object.h);
}

bool Physical_body::is_hitting_1D(int first_mid, int second_mid, int total_widths){
    return abs( first_mid - second_mid ) < (total_widths/2);
}

//key_timer

int passed_time(high_resolution_clock::time_point from){
    return duration_cast<milliseconds>(high_resolution_clock::now() - from).count();
}

Key_timer::Key_timer(){
    key_is_released = true;
    total_press_time = 0;
}

void Key_timer::key_pressed(){
    if(key_is_released){
        last_press = high_resolution_clock::now();
        key_is_released = false;
    }else{
        cerr << "key must be realised befor next press" << endl;
        abort();
    }
}

void Key_timer::key_released(){   
    total_press_time = passed_time(last_press);
    key_is_released = true;
}


int Key_timer::get_total_time(){ 
    if(! key_is_released){
        key_released();
        key_pressed();

        return total_press_time;
    }else{
        int result = total_press_time;
        clear();
        return result;
    }
}

void Key_timer::clear(){
    key_is_released = true;
    total_press_time = 0;
}
