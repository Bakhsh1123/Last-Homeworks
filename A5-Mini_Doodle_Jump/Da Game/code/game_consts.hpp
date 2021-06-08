#include "brsdl.hpp"
#include <vector>


#ifndef __GAME_CONESTS_HPP__
    #define __GAME_CONESTS_HPP__

    //moving object's data

    //animation
    const int JUMP_DORATION = 500;

    //player
    const int SINGLE_JUMP_HEIGHT = 260; // unit:(pixel)
    const int SINGLE_JUMP_TIME = 1300;// unit:(milisecond)
    const double PLAYER_NORMAL_JUMP_SPEED = 0.8;// SINGLE_JUMP_HEIGHT / SINGLE_JUMP_TIME     unit:(pixel per milisecond)
    const double PLAYER_SPRING_JUMP_SPEED = PLAYER_NORMAL_JUMP_SPEED * 2;
    const double GRAVITY_ACCELERATION = 0.001230769;// PLAYER_NORMAL_JUMP_SPEED / SINGLE_JUMP_TIME   unit:(pixel per (milisecond)^2)
    const double PLAYER_STANDARD_HORIZONTAL_SPEED = 0.4;
    

    //moving platform
    const double MOVING_PLATFORM_HORIZONTAL_SPEED  = 0.15;

    //framong data
    const int FRAME_DORATION = 40;//25 fps
    const int POWER2_FRAME_DORATION = FRAME_DORATION * FRAME_DORATION;

    const int PAGE_WIDTH = 509;
    const int PAGE_HEIGHT = 814;
    
    const int PLATFORM_WIDTH = 94;
    const int PLATFORM_HEIGHT = 28;
    
    const int ENEMY_WIDTH = 139;
    const int ENEMY_HEIGHT = 87;

    const int SPRING_WIDTH = 34;
    const int SPRING_HEIGHT = 24;

    const int PLAYER_WIDTH = 74;
    const int PLAYER_HEIGHT = 72;
    
    //drawing data
    const string BACKGROUND_ADDRESS = "./media/background.png";
    const string S_PLATFORM_ADDRESS = "./media/splat.png";
    const string M_PLATFORM_ADDRESS = "./media/mplat.png";
    const string ENEMY_ADDRESS = "./media/enemy.png";
    const string SPRING_ADDRESS = "./media/spring.png";
    
    const string JUMPING_PLAYER_ADDRESS = "./media/jumping_player.png";
    const string PLAYER_ADDRESS =  "./media/player.png";

    const string MAP_ADDRESS = "./map/map.txt";

    const string WINDOW_NAME = "Baxayesh Jump ... ";

    const std::vector<string> Breaking_Platform_ADDRESSES{"./media/bplat0.png", "./media/bplat1.png",
                                                          "./media/bplat2.png", "./media/bplat3.png"};

    const std::vector<int> ADITIONAL_HEIGHT_FOR_BRAEKING_PLATFORM{0, 7, 20, 24};

    const std::vector<int> BRAEKING_PLATFORM_STATE_DORATION{0, 65, 130, 2100};

    const int ADITIONAL_WIDTH_FOR_BREAKING_PLATFORM = 3;

    const string FONT_ADDRESS = "./media/font.ttf";

#endif