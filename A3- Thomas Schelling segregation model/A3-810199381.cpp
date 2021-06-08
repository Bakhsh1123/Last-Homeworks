#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <ctime>

//  defines
//      agent codes
#define RED_AGENT 1
#define BLUE_AGENT 2
#define EMPTY 0
#define OUT_OF_MAP -1

//      input keys
#define FILE_ADDRESS_KEY "-f"
#define HAPPINESS_THRESHOLD_KEY "-p"
#define ROUND_COUNT_KEY "-s"

//      pixels
#define WHITE_PIXEL "255 255 255"
#define RED_PIXEL "255 0 0"
#define BLUE_PIXEL "0 0 255"
#define BLACK_PIXEL "0 0 0"

//      other consts
#define UNLIMITED -1
#define MAX_MOVES 4
#define SATISFIED 100
#define DEFAULT_HAPPINESS_THRESHOLD 30
#define SOURCE_ADDRESS "out.ppm"
#define MAX_OF_RGB 255
#define SPACE ' '
#define SCALE 45
#define BLACK_LINE_WIDGH 2



using namespace std;

//  data structure & related tools
//      input data
typedef struct input_data{

    char* file_address;

    int total_rounds;

    int happiness_threshold;

} raw_input;

//      point data
typedef struct point_data{

    int x;
    int y;

} point;

point new_point(int y, int x){
    point new_instanse;

    new_instanse.x = x;
    new_instanse.y = y;

    return new_instanse;
}

//      point lists
typedef vector<point> point_list;

point pop_and_return(point_list& list, int index){

    point item = list[index];

    list.erase(list.begin() + index);

    return item;

}

point random_pop(point_list& list){
        
    int random_index = rand() % list.size();

    return pop_and_return(list, random_index);

}

//      agents
typedef char agent;

char agent_to_character(agent data){

    switch (data)
    {
    case EMPTY:
        return 'E';
    case RED_AGENT:
        return 'R';
    case BLUE_AGENT:
        return 'B';
    default:
        return '?';
    }
}

agent character_to_agent(char data){

    switch (data)
    {
    case 'E':
        return EMPTY;
    case 'R':
        return RED_AGENT;
    case 'B':
        return BLUE_AGENT;
    default:
        return OUT_OF_MAP;
    }

}

string agent_to_pixel(agent data){

    switch (data)
    {
    case EMPTY:
        return WHITE_PIXEL;
    case RED_AGENT:
        return RED_PIXEL;
    case BLUE_AGENT:
        return BLUE_PIXEL;
    default:
        return BLACK_PIXEL;
    }

}

//      state data
typedef vector<vector<agent>> office_Map;

typedef struct office_statous{

    office_Map map;

    int map_hight;
    int map_width;

    point_list dissatisfieds;
    point_list empties;

    int happiness_threshold;
    int remined_rounds;

} state_data;

bool is_out_of_map(point location, int hight, int width){
    
    if( 0 <= location.x && location.x < width && 0 <= location.y && location.y < hight){

        return false;

    }

    return true;
}

agent get_color(state_data& state, point location){
    
    return state.map[ location.y ][ location.x ];

}

void set_color(state_data& state, point location, agent color){

    state.map[location.y][location.x] = color;

}


point get_neighbor(point home, int move_num){

    int change_on_y[] = { 1, -1, 0,  0};
    int change_on_x[] = { 0,  0, 1, -1};

    return new_point( change_on_y[move_num] + home.y, change_on_x[move_num] + home.x);
}


agent get_neighbor_color(state_data& state, point home, int move){

    point neighbor = get_neighbor(home, move);

    if( is_out_of_map(neighbor, state.map_hight, state.map_width) ){

        return OUT_OF_MAP;

    }

    return get_color(state, neighbor);

}

int calculate_satisfiction(state_data& state, point location){

    int similar_count = 0;
    int neighbor_count = 0;

    agent location_color = get_color(state, location);
    agent neighbor_color;

    if( location_color == EMPTY){ 
        return SATISFIED;
    }

        

    for(int move = 0; move < MAX_MOVES; move++){

        neighbor_color =  get_neighbor_color(state, location, move);

        if( neighbor_color == OUT_OF_MAP ){
            continue;
        }

        neighbor_count++;

        if( neighbor_color == location_color || neighbor_color == EMPTY){
            similar_count++;
        }
    }

    if(neighbor_count == 0){
        return SATISFIED;
    }else{
        return (similar_count * 100) / neighbor_count;
    }
}


bool is_satisfied(state_data& state, int checking_y, int checking_x){

    point home = new_point(checking_y, checking_x);

    if( calculate_satisfiction(state, home) < state.happiness_threshold){
        return false;
    }

    return true;
}

void replace(state_data& state, point source, point destination){

    agent destination_color = get_color(state, destination);
    agent source_color = get_color(state, source);

    set_color(state, source, destination_color);
    set_color(state, destination, source_color);
}

void list_empties(state_data& state){

    for(int checking_y = 0; checking_y < state.map_hight; checking_y++){
        for(int checking_x = 0; checking_x < state.map_width; checking_x++){

            if( state.map[ checking_y ][ checking_x ] == EMPTY){
                state.empties.push_back( new_point(checking_y, checking_x) );
            }

        }
    }
}

void list_dissatisfieds(state_data& state){

    state.dissatisfieds.clear();

        

    for(int checking_y = 0; checking_y < state.map_hight; checking_y++){
        for(int checking_x = 0; checking_x < state.map_width; checking_x++){

            
            
            if( ! is_satisfied( state, checking_y, checking_x ) ){

                state.dissatisfieds.push_back( new_point(checking_y, checking_x) );
                
            }
        }
    }
}


//  functions

//      preform

void replace_by_empty(state_data& state){

    point dissatisfied = random_pop( state.dissatisfieds );

    point empty_point = random_pop( state.empties );

    replace(state, dissatisfied, empty_point);

    state.empties.push_back( dissatisfied );
    
}

void replace_two_dissatisfieds(state_data& state){

    point first = random_pop(state.dissatisfieds);

    point second = random_pop(state.dissatisfieds);

    replace(state, first, second);

}


bool random_decide( int desired_options, int undesired_options){

    int random_number = rand() % ( undesired_options + desired_options );

    if( random_number < desired_options){
        return true;
    }

    return false;
}

void error(){

    cerr << " INVALID INPUT ! :( " << endl <<
            " there is not any other way to replace dissatisfieds but it Was guaranteed to be  " << endl <<
            " so i kill myself X(" << endl;

    abort();

}

void replace_dissatisfieds(state_data& state){

    while( ! state.dissatisfieds.empty() ){

        if( state.dissatisfieds.size() == 1 && state.empties.empty()){

           error();

        }
        //  if m is count of dissatisfieds and n is count of empty locations
        //   each dissatisfied can get replaced by m - 1 disstisfied location or can go to n empty home
        //   so chanse of replacing by another dissatisfied is (m - 1)/( n + m - 1 )
        //   also if m < 2 then there is no chanse to replace two dissatisified

        if( random_decide( state.dissatisfieds.size() - 1, state.empties.size() )){

            replace_two_dissatisfieds(state);

        }else{

            replace_by_empty(state);

        }
    }
}

void preform(state_data& state){

    while( ! state.dissatisfieds.empty() && state.remined_rounds != 0){

        replace_dissatisfieds(state);

        list_dissatisfieds(state);

        state.remined_rounds--;
    }
}

//      input

void read_another_part(int argc, char** argv, int working_index, raw_input& data_slot){

    if( working_index + 1  >= argc){
        return;
    }

    string key = argv[working_index];
    char* value = argv[working_index + 1];

    if( key == FILE_ADDRESS_KEY ){

        data_slot.file_address = value;

    }else if( key == ROUND_COUNT_KEY ){

        data_slot.total_rounds = atoi(value);

    }else if( key == HAPPINESS_THRESHOLD_KEY){
        
        data_slot.happiness_threshold = atoi(value);

    }else{
        read_another_part(argc, argv, working_index + 1, data_slot);
        return;
    }

    read_another_part(argc, argv, working_index + 2, data_slot);
}

raw_input read_args(int argc, char** argv){

    raw_input input;

    input.total_rounds = UNLIMITED;
    input.happiness_threshold = DEFAULT_HAPPINESS_THRESHOLD;

    read_another_part(argc, argv, 0, input);

    return input;

}

vector< agent > translate_row( string row_data){

    vector<agent> row;
    agent agent_buffer;

    for(int i = 0; i < row_data.size(); i++){

        agent_buffer = character_to_agent( row_data[i] );

        if(agent_buffer != OUT_OF_MAP){

            row.push_back( agent_buffer );

        }
        
    }

    return row;

}

office_Map read_map_file(char* file_address){

    office_Map map;
    string one_row;

    ifstream source;

    source.open(file_address, ios::in);

    while( getline( source, one_row ) ){

        map.push_back( translate_row(one_row) );

    } 

    source.close();

    return map;
    
}

state_data intialize_state(office_Map& map, int total_rounds, int happiness_threshold){

    state_data state;

    state.map = map;
    state.map_hight = map.size();
    state.map_width = map[0].size();

    state.remined_rounds = total_rounds;
    state.happiness_threshold = happiness_threshold;

    list_empties(state);

    list_dissatisfieds(state);

    return state;
}


state_data get_input(int argc, char** argv){

    raw_input input = read_args(argc, argv);

    office_Map map = read_map_file( input.file_address );

    return intialize_state( map, input.total_rounds, input.happiness_threshold );

}

//      output

void write_in_console(state_data& data){

    cout << data.dissatisfieds.size() << endl;

    for( int y = 0; y < data.map_hight; y++){
        for( int x = 0; x < data.map_width; x++){
            
            cout << agent_to_character(data.map[y][x]);

        }

        cout << endl;
    }
}


void write_image_row( ofstream& source, const vector< agent >& data){

    for(int x = 0; x < data.size(); x++){

        for(int _i = 0; _i < SCALE; _i++)
            source << agent_to_pixel( data[x] ) << endl;

        for(int _i = 0; _i < BLACK_LINE_WIDGH; _i++)
            source << BLACK_PIXEL << endl;

    }

}

void write_black_row( ofstream& source, int real_width){

    for(int x = 0; x < real_width * (SCALE + BLACK_LINE_WIDGH); x++)
            source << BLACK_PIXEL << endl;

}

void write_scaled_row( ofstream& source, const vector< agent >& data){

    for(int _i = 0; _i < SCALE; _i++)
        write_image_row(source, data);

    for(int _i = 0; _i < BLACK_LINE_WIDGH; _i++)
        write_black_row(source, data.size());

}

void write_ppm_data(ofstream& source, office_Map& map){

    for( auto& row : map)
        write_scaled_row(source, row);
        

}

void write_ppm_header(ofstream& source, int width, int hight){

    source << "P3" << endl << width << SPACE << hight << endl << MAX_OF_RGB << endl;

}

void write_image(state_data& data){
    
    ofstream source;

    source.open(SOURCE_ADDRESS, ios::out);

    write_ppm_header( source, (SCALE + BLACK_LINE_WIDGH) * data.map_width, (SCALE + BLACK_LINE_WIDGH) * data.map_hight);

    write_ppm_data( source, data.map);

    source.close();
}

void output(state_data& data){

    write_in_console(data);

    write_image(data);

}

//      main
int main(int argv, char** argc){

    srand( time(0) );

    state_data state = get_input(argv, argc);

    preform(state);

    output(state);

    return 0;
}