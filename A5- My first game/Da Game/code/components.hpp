#include "brsdl.hpp"
#include "game_consts.hpp"
#include <string>
#include <vector>

using namespace std::chrono;

#ifndef __COMPONENTS_HPP__
    #define __COMPONENTS_HPP__

    class Component{
    public:
        Component(Point pivot, int height, int width);

        virtual void update() = 0;

        virtual void draw(Window* page, int stage_y) = 0;

        int get_partial_height(int stage_bottom_edge_height){ return physical_body.get_y() - stage_bottom_edge_height; }

    protected:

        Physical_body physical_body;

        Rectangle get_drawing_rectangle(int stage_y);
       
    };

    class Player : public Component{
    public:
        Player(Point pivot);

        void update();

        void draw(Window* page, int stage_y);

        void horizontal_move(int time_to_left, int time_to_right);

        void vertical_move();

        Rectangle get_legs();
        Rectangle get_body();

        void die();
        void jump();
        void spring_jump();

        int get_highest_touched_y(){ return highest_touched_y + PLAYER_HEIGHT/2 ; }
        bool is_died(){ return ! is_alive; }
        bool is_falling(){ return vertical_speed < 0; }

    protected:
        double vertical_speed;

        bool is_jumping;
        high_resolution_clock::time_point last_jump;

        bool is_going_left;
        bool is_alive;

        int highest_touched_y;
    };

    class hittable_Component : public Component{
    public:
        hittable_Component(Point mid_down, int height, int width) :  Component(mid_down + Point(0, height/2), height, width){}

        void hit_check(Player* player){
            if(hitted(player))
                perform_hit(player);
        }

    protected:
        
        virtual bool hitted(Player* player){ return player->is_falling() && physical_body.is_hitting( player->get_legs() ); };
        virtual void perform_hit(Player* player) = 0;
    };
    

    class Platform : public hittable_Component{
    public:
        Platform(Point mid_down) : hittable_Component(mid_down, PLATFORM_HEIGHT, PLATFORM_WIDTH){}

        virtual void perform_hit(Player* player){ player->jump(); }
 
    };


    class Moving_Platform : public Platform{
    public:
        Moving_Platform(Point mid_down) : Platform( mid_down ){ horizontal_speed = MOVING_PLATFORM_HORIZONTAL_SPEED; }

        void update();

        void draw(Window* page, int stage_y);

    protected:
        float horizontal_speed;

    };

    class Simple_Platform : public Platform{
        public:
            Simple_Platform(Point mid_down): Platform( mid_down ){}

            void update(){}

            void draw(Window* page, int stage_y);
        
    };

    class Breaking_Platform : public Platform{
    public:
        Breaking_Platform(Point mid_down);

        void update();

        void draw(Window* page, int stage_y);

        void perform_hit(Player* player);

    protected:
        bool is_breaked;
        high_resolution_clock::time_point break_time;

        float vertical_speed;

        Rectangle get_stated_rectangle(int state, int stage_y);

    };

    class Enemy : public hittable_Component{
    public:
        Enemy(Point mid_down) : hittable_Component(mid_down, ENEMY_HEIGHT, ENEMY_WIDTH) {}

        void update(){}

        void draw(Window* page, int stage_y);

        bool hitted(Player* player){ return physical_body.is_hitting( player->get_body() ); }

        void perform_hit(Player* player);

    };

    class Spring : public hittable_Component{
    public:
        Spring(Point mid_down) : hittable_Component(mid_down, SPRING_HEIGHT, SPRING_WIDTH) {}

        void update(){}

        void draw(Window* page, int stage_y);

        void perform_hit(Player* player);

    };
    
    




#endif
