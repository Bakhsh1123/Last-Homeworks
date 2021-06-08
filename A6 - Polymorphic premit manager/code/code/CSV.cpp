#include "CSV.hpp"

vector<string> break_text_by(string text, char separator ){

    vector<string> result;
    int last_break = 0;

    for( int i = 0; i < text.length(); i++)
        if(text[i] == separator){
            result.push_back(text.substr(last_break, i - last_break));
            last_break = i+1;
        }

    if( last_break < text.length()){
        result.push_back(text.substr(last_break));
    }

    return result;
}


CSV::CSV( string file_address ){

    ifstream file(file_address);

    string line;

    getline(file, line);
    header_raw = break_text_by(line, ',');
    
    while(getline(file, line))
        sheet.push_back( break_text_by(line, ',') );
    
    
    file.close();
}

string CSV::at(int raw, int column){ return sheet[raw][column]; }

vector<string> CSV::get_raw(int raw_num){ return sheet[raw_num]; }

int CSV::raw_cont(){ return sheet.size(); }
