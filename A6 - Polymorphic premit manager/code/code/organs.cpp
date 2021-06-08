#include "organs.hpp"

Signature find_sign_type(string type){
    if(type == "o")
        return Signature::ORGAN_SIGN;
    else if(type == "m")
        return Signature::MANAGER_SIGN;
    else if(type == "c")
        return Signature::CEO_SIGN;
    else
        throw runtime_error("undefined sign type");
}

Organ::Organ(string organ_name, string manager_name, string CEO_name) 
        : organ_name(organ_name), manager_name(manager_name), CEO_name(CEO_name){}

string Organ::get_signature( Signature sign_type ) const {
    switch (sign_type)
    {
    case Signature::ORGAN_SIGN:
        return organ_name;
    case Signature::MANAGER_SIGN:
        return manager_name;
    case Signature::CEO_SIGN:
        return CEO_name;
    default:
        throw runtime_error("Worg signature type entered");
    }
}

string Organ::get_name(){ return organ_name; }
