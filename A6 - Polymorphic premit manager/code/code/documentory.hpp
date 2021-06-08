#include <iostream>
#include <vector>
#include <string>
#include "CSV.hpp"
#include "documents.hpp"
#include "organs.hpp"

using namespace std;

#ifndef __DOCUMENTORY_HPP__
    #define __DOCUMENTORY_HPP__

class Documentory{
private:
    vector<Organ*> organs;
    vector<Form*> forms;
    vector<permit*> permits;


    Form* find_form_by_name(string goal);

    permit* find_permit_by_name(string goal);

    Document* find_requirement_by_name(string name);

    void extract_forms(string forms_sheet_address);

    void extract_permits(string permits_sheet_address);

public:
    Documentory( string forms_sheet_address, string permits_sheet_address);

    bool read_console();

    ~Documentory();

};

#endif