#include "components.hpp"              

using namespace std;

#ifndef __STAGE_HPP__
    #define __STAGE_HPP__       
     
    class Stage{
    public:

        Stage( string map_address );

        void start_game();
    
        
    private:

        Window* page;    
        
        Player* player;

        Key_timer left_key;
        Key_timer right_key;

        int stage_bottom_edge_height;

        vector<hittable_Component*> game_map;

        int first_visible_component_index;
        int last_visible_component_index;
        
        bool gameplay_control();

        void move_player(int time_to_left, int time_to_right);

        void game_update();

        void update_stage_height();

        void update_first_visible_index();

        void update_last_visible_index();

        void update_indexes();

        void update_components();

        void perform_hits();

        void check_player_fall();

        void gameover_message();

        int get_score(){ return player->get_highest_touched_y() - (PLAYER_HEIGHT/2); }

        void draw();

        void read_map( string map_address );

        hittable_Component* read_Component(int x, int y, string type);
    };

#endif
