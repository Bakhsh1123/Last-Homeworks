#include "documentory.hpp"

int main(int argc, char** argv){

    try{

        Documentory documentory(argv[2], argv[1]);

        while(documentory.read_console());


    }catch(runtime_error ex){
        cerr << ex.what() << endl;
    }

    return 0;

}