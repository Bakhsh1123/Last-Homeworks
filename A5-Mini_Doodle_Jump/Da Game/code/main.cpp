#include "stage.hpp"
#include "../liberaries/rsdl.hpp"

using namespace std;


int main(int argc, char** argv){

    Stage master(MAP_ADDRESS);

    master.start_game();

    return 0;
}