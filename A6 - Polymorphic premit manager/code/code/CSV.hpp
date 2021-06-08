#include <fstream>
#include <vector>
#include <string>

using namespace std;

#ifndef __CSV_HPP__
    #define __CSV_HPP__

vector<string> break_text_by(string text, char separator );

class CSV{
private:
    vector< vector<string> > sheet;
    vector<string> header_raw;

public:
    CSV( string file_address );

    string at(int raw, int column);

    vector<string> get_raw(int raw_num);
    
    int raw_cont();
};


#endif