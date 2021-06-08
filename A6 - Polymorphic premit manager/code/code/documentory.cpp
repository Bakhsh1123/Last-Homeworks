#include "documentory.hpp"

Form* Documentory::find_form_by_name(string goal){
    for(auto form : forms){
        if( form == nullptr ){
            throw runtime_error("wrong form added to form list");
        }

        if( form->is(goal) )
            return form;
    }
        

    return nullptr;
}

permit* Documentory::find_permit_by_name(string goal){
    for(auto permit : permits){

        if( permit == nullptr ){
            throw runtime_error("wrong permit added to permits list");
        }

        if( permit->is(goal) )
            return permit;
    }
        
    return nullptr;
}

Document* Documentory::find_requirement_by_name(string name){
    Document* requirement = find_form_by_name(name);

    return requirement == nullptr ? find_permit_by_name(name) : requirement;
}

void Documentory::extract_forms(string forms_sheet_address){
    CSV forms_sheet( forms_sheet_address );

    for( int i = 0; i < forms_sheet.raw_cont(); i++)
        forms.push_back( new Form(forms_sheet.at(i,0), forms_sheet.at(i,1)));

}

void Documentory::extract_permits(string permits_sheet_address){
    CSV permits_sheet( permits_sheet_address );
    
    vector< vector<string> > requirements_lists;

    for( int i = 0; i < permits_sheet.raw_cont(); i++){
        organs.push_back( new Organ(permits_sheet.at(i, 0), permits_sheet.at(i, 4), permits_sheet.at(i, 5)));
        permits.push_back( new permit(permits_sheet.at(i, 2), organs[i], find_sign_type(permits_sheet.at(i, 1)) ));
        requirements_lists.push_back( break_text_by(permits_sheet.at(i, 3), '-'));
    }

    for( int i = 0; i < permits.size(); i++){
        for(auto requirement_name : requirements_lists[i]){
            Document* requirement = find_requirement_by_name(requirement_name);
            if(requirement != nullptr){
                permits[i]->add_requirement( requirement );
            }else{
                throw runtime_error("requirement does not exists");
            }

        }
    }
}

Documentory::Documentory( string forms_sheet_address, string permits_sheet_address){
    extract_forms(forms_sheet_address);
    extract_permits(permits_sheet_address);
}


bool Documentory::read_console(){
    string new_word;

    if(cin >> new_word){
        if(new_word == "Confirm"){
            string form_name;

            cin >> form_name;

            Form* form = find_form_by_name(form_name);

            if(form != nullptr){
                form->confirm();
            }else{
                cerr << "Form does not exists!! :( try something else" << endl;
            }
        }else{
            cerr << "comannd does not exists!! :( try something else" << endl;
        }

        return true;
    }else{
        return false;
    }
}

Documentory::~Documentory(){

    for(auto permit : permits)
        delete permit;

    for(auto form : forms)
        delete form;

    for(auto organ : organs)
        delete organ;
}

