#include "brsdl.hpp"
#include "game_consts.hpp"
#include "components.hpp"
#include "stage.hpp"
#include <string>
#include <vector>
#include <ctime>
#include <fstream>

Stage::Stage( string map_address ){

    page = new Window(PAGE_WIDTH, PAGE_HEIGHT, WINDOW_NAME);
    player = new Player(Point(PAGE_WIDTH/2, PLAYER_HEIGHT/2));
    stage_bottom_edge_height = 0;

    read_map(map_address);

    first_visible_component_index = 0;
    last_visible_component_index = 0;

    update_indexes();
}

void Stage::start_game(){
    while(gameplay_control()){
        draw();
        delay(FRAME_DORATION);    
    }
}

void Stage::game_update(){
    update_stage_height();
    update_indexes();
    update_components();
    perform_hits();
    check_player_fall();
}

bool Stage::gameplay_control(){

    while(page->has_pending_event()) {
        Event event = page->poll_for_event();
        
        switch(event.get_type()) {
            case Event::KEY_PRESS:
                
                if(player->is_died())
                    return false;

                if(event.get_pressed_key() == 'd'){
                    right_key.key_pressed();
                }
                else if(event.get_pressed_key() == 'a'){
                    left_key.key_pressed();
                }
                
                break;

            case Event::KEY_RELEASE:
                if(event.get_pressed_key() == 'd'){
                    right_key.key_released();
                }
                else if(event.get_pressed_key() == 'a'){
                    left_key.key_released();
                }

                break;
                
            case Event::QUIT:
                return false;
                break;
        }
    }

    move_player(left_key.get_total_time(), right_key.get_total_time());     
    game_update();
    return true;
}

void Stage::move_player(int time_to_left, int time_to_right){
    player->horizontal_move(time_to_left, time_to_right);
    player->vertical_move();
}


void Stage::update_stage_height(){
    if((player->get_highest_touched_y() - stage_bottom_edge_height) > (PAGE_HEIGHT/2) )
        stage_bottom_edge_height = (player->get_highest_touched_y() - (PAGE_HEIGHT/2) );
}


void Stage::update_first_visible_index(){
    for(int i = first_visible_component_index; i < game_map.size(); i++){
        if( game_map[i]->get_partial_height( stage_bottom_edge_height ) > -PLAYER_HEIGHT ){
            first_visible_component_index = i;
            return;
        }
    }
    first_visible_component_index = game_map.size() - 1;
}

void Stage::update_last_visible_index(){
    for(int i = last_visible_component_index; i < game_map.size(); i++){
        if( game_map[i]->get_partial_height( stage_bottom_edge_height ) > PAGE_HEIGHT ){
            last_visible_component_index = i;
            return;
        }
    }
    last_visible_component_index = game_map.size() - 1;
}

void Stage::update_indexes(){
    update_first_visible_index();
    update_last_visible_index();
}

void Stage::update_components(){

    for(int i = first_visible_component_index; i <= last_visible_component_index; i++)
        game_map[i]->update();

    player->update();

}

void Stage::perform_hits(){
    if(player->is_died())
        return;

    for(int i = first_visible_component_index; i <= last_visible_component_index; i++)
        game_map[i]->hit_check(player);
}

void Stage::check_player_fall(){
    if(player->get_partial_height(stage_bottom_edge_height) < -PLAYER_HEIGHT)
        player->die();
}


void Stage::draw(){
    page->clear();

    page->draw_img(BACKGROUND_ADDRESS);
    
    for(auto component : game_map)
        component->draw(page, stage_bottom_edge_height);

    player->draw(page, stage_bottom_edge_height);

    for(int i = 0; i < 3; i++)
        page->show_text("score : " + to_string(get_score()), Point(15, 15), BLACK, FONT_ADDRESS, 35);

    if(player->is_died())
        for(int i = 0; i < 3; i++)
            gameover_message();
       

    page->update_screen();
}

void Stage::gameover_message(){
    page->show_text("Game Over!!!", Point(25, PAGE_HEIGHT/2), BLACK, FONT_ADDRESS, 70);
    page->show_text("Your score is : " + to_string(get_score()), Point(25, PAGE_HEIGHT/2 + 70), BLACK, FONT_ADDRESS, 45);
    page->show_text("press any key to exit ... ", Point(25, PAGE_HEIGHT/2 + 115), BLACK, FONT_ADDRESS, 30);
}

int scale(int input ){ return (input*8)/10; }

void Stage::read_map( string map_address ){

    int cont;
    int x, y;
    string type;

    ifstream map_file( map_address );

    map_file >> cont;

    for( int i = 0; i < cont; i++){
        map_file >> x >> y >> type;

        game_map.push_back( read_Component(scale(x), scale(y), type) );
        
    }

    map_file.close();
}

hittable_Component* Stage::read_Component(int x, int y, string type){

    if(type == "platform"){ return new Simple_Platform( Point(x, y) ); }
    else if(type == "mplatform"){ return new Moving_Platform( Point(x, y) ); }
    else if(type == "bplatform"){ return new Breaking_Platform( Point(x, y) ); }
    else if(type == "spring"){ return new Spring( Point(x, y) ); }
    else if(type == "enemy"){ return new Enemy( Point(x, y) ); }
    else{ throw runtime_error("unknoun map component"); }
}