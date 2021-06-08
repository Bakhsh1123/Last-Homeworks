#include "components.hpp"
#include "game_consts.hpp"
#include <chrono>

using namespace std::chrono;

//components

Component::Component(Point pivot, int height, int width) : physical_body(pivot, width, height){}

Rectangle Component::get_drawing_rectangle(int stage_y){
    Rectangle shape =  physical_body.get_rectangle(stage_y);

    shape.y = PAGE_HEIGHT - shape.y;

    return shape;
}

//player

Player::Player(Point pivot) : Component(pivot, PLAYER_HEIGHT, PLAYER_WIDTH){
    vertical_speed = PLAYER_NORMAL_JUMP_SPEED;
    is_jumping = true;
    last_jump = high_resolution_clock::now();

    is_going_left = false;
    is_alive = true;

    highest_touched_y = 0;
}

void Player::update(){
    if( is_jumping && ( passed_time(last_jump) > JUMP_DORATION) )
        is_jumping = false;
}

void Player::draw(Window* page, int stage_y){
    if(is_jumping){
        page->draw_img(JUMPING_PLAYER_ADDRESS, get_drawing_rectangle( stage_y ), NULL_RECT, 0, is_going_left);
    }else{
        page->draw_img(PLAYER_ADDRESS, get_drawing_rectangle( stage_y ), NULL_RECT, 0, is_going_left);
    }
}

void Player::horizontal_move(int time_to_left, int time_to_right){

    if(is_died())
        return;

    int total_move_to_right = time_to_right - time_to_left;

    if( time_to_left != 0 || time_to_right != 0)
        is_going_left =  (time_to_left > time_to_right);

    physical_body.move( total_move_to_right * PLAYER_STANDARD_HORIZONTAL_SPEED,0);

    if(physical_body.passed_page_horizontal_edge()){
        physical_body.return_pivot_to_page();
    }
};

void Player::vertical_move(){

    physical_body.move(0, -(GRAVITY_ACCELERATION / 2) * POWER2_FRAME_DORATION + vertical_speed * FRAME_DORATION);

    vertical_speed -= GRAVITY_ACCELERATION * FRAME_DORATION;

    if(physical_body.get_y() > highest_touched_y){
        highest_touched_y = physical_body.get_y();
    }
}



Rectangle Player::get_legs(){ return physical_body.get_bottom(5); }
Rectangle Player::get_body(){ return physical_body.get_rectangle(0); }

void Player::die(){ is_alive = false; }

void Player::jump(){
    vertical_speed = PLAYER_NORMAL_JUMP_SPEED;
    is_jumping = true;
    last_jump = high_resolution_clock::now();
}

void Player::spring_jump(){ 
    vertical_speed = PLAYER_SPRING_JUMP_SPEED; 
    is_jumping = true;
    last_jump = high_resolution_clock::now();
}

//Platforms

//  moving platform
void Moving_Platform::update(){
    physical_body.move(horizontal_speed*FRAME_DORATION, 0);

    if(physical_body.touched_page_horizontal_edge()){
        horizontal_speed = -horizontal_speed;
    }
}

void Moving_Platform::draw(Window* page, int stage_y){
    page->draw_img(M_PLATFORM_ADDRESS, get_drawing_rectangle( stage_y ));
}

//  simple platform
void Simple_Platform::draw(Window* page, int stage_y){
    page->draw_img(S_PLATFORM_ADDRESS, get_drawing_rectangle( stage_y ));
}


//  breaking platform
Breaking_Platform::Breaking_Platform(Point pivot) : Platform( pivot ){
    is_breaked = false;
    vertical_speed = 0;
}
        
void Breaking_Platform::perform_hit(Player* player){ 
    if( ! is_breaked ){
        is_breaked = true;
        break_time = high_resolution_clock::now();      
    }
}

void Breaking_Platform::update(){
    if(is_breaked){
        physical_body.move(0, -(GRAVITY_ACCELERATION / 2) * POWER2_FRAME_DORATION + vertical_speed * FRAME_DORATION);
        vertical_speed -= GRAVITY_ACCELERATION * FRAME_DORATION;
    }
}

bool between(int number, int start, int end){ return start <= number && number < end; }

int find_state( int pased_time ){
    int state;

    for( state = 0; state < 4 && !between(pased_time, BRAEKING_PLATFORM_STATE_DORATION[state], BRAEKING_PLATFORM_STATE_DORATION[state+1]); state++);

    return state + 1;
}

void Breaking_Platform::draw(Window* page, int stage_y){
    if(is_breaked){
        int pased_time = passed_time(break_time);

        int state = find_state(pased_time);

        if(state >= 4){ return; }

        page->draw_img(Breaking_Platform_ADDRESSES[state], get_stated_rectangle(state, stage_y));

    }else{
        page->draw_img(Breaking_Platform_ADDRESSES[0], get_drawing_rectangle( stage_y ));
    }
}

Rectangle Breaking_Platform::get_stated_rectangle(int state, int stage_y){
    Rectangle result = get_drawing_rectangle( stage_y );

    result.h += ADITIONAL_HEIGHT_FOR_BRAEKING_PLATFORM[state];

    result.w += ADITIONAL_WIDTH_FOR_BREAKING_PLATFORM;

    return result;
}

//  enemy

void Enemy::draw(Window* page, int stage_y){
    page->draw_img(ENEMY_ADDRESS, get_drawing_rectangle( stage_y ));
}

void Enemy::perform_hit(Player* player){ player->die(); }

//  spring

void Spring::draw(Window* page, int stage_y){
    page->draw_img(SPRING_ADDRESS, get_drawing_rectangle( stage_y ));
}

void Spring::perform_hit(Player* player){ player->spring_jump(); }
