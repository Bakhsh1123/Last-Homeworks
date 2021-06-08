#include <string>
#include <iostream>

using namespace std;

#ifndef __ORGANS_HPP__
    #define __ORGANS_HPP__

enum class Signature {ORGAN_SIGN, MANAGER_SIGN, CEO_SIGN};

Signature find_sign_type(string type);

class Organ{
private:
    string organ_name;
    string manager_name;
    string CEO_name;

public:
    Organ(string organ_name, string manager_name, string CEO_name);

    string get_signature( Signature sign_type ) const ;
    string get_name();

};

#endif