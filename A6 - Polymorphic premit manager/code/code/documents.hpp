#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "organs.hpp"

using namespace std;

#ifndef __DOCUMENTS_HPP__
    #define __DOCUMENTS_HPP__

#define TAB "    "

string shift_one_tab( string text );

class Document{
protected:
    string name;

    vector<Document*> requitors;

    virtual void requirement_passed( Document* requirement) = 0;

    void notify_requitors();

public:
    Document(string name);

    void add_requitor( Document* new_requitor );

    virtual string to_string() = 0;

    bool is( string others_name );

};

class Form : public Document{
private:
    string content;
    bool is_confirmed;

public:
    Form( string name, string content);

    void confirm();
    void requirement_passed( Document* requirement);

    string to_string();

};

class Requirement{
private:
    Document* file;
    bool passed;

public:
    Requirement( Document* file );

    void pass();

    bool is_passed() const ;

    bool operator==( Document* other_file ) const ;

    string to_string();
};

class permit : public Document{
private:
    Organ* related_organ;
    Signature sign_type;

    vector<Requirement> requirements;

    bool is_ready() const ;

public:

    permit(string name, Organ* organ, Signature sign_type);


    void add_requirement( Document* new_requirement);

    void requirement_passed( Document* requirement);

    void print_permit() ;

    string to_string();

};

#endif