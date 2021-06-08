#include "documents.hpp"

string shift_one_tab( string text ){

    text.insert(0, TAB);

    for( int i = 0; i < text.length() - 1; i++)
        if(text[i] == '\n')
            text.insert(i + 1, TAB);

    return text;   
}

void Document::notify_requitors(){
    for( auto requitor : requitors )
        requitor->requirement_passed( this );
}

Document::Document(string name) : name(name){}

void Document::add_requitor( Document* new_requitor ){
    if(new_requitor == nullptr){
        throw runtime_error("requitor cant be null");
    } 
    requitors.push_back( new_requitor ); 
}

bool Document::is( string others_name )  {
    return name == others_name; 
}

Form::Form( string name, string content) : Document( name ), content( content ), is_confirmed(false) {}

void Form::confirm(){ 
    is_confirmed = true;
    notify_requitors();
}

void Form::requirement_passed( Document* requirement){ }

string Form::to_string(){
    ostringstream output;

    output << "Name: " << name << '\n';
    output << "Content: " << content << '\n';

    return output.str();
}

Requirement::Requirement( Document* file ) : file(file), passed( false ){
    if( file == nullptr){
        throw runtime_error("null pointer entered");
    }
}

void Requirement::pass(){ passed = true; }

bool Requirement::is_passed() const { return passed; }

bool Requirement::operator==( Document* other_file ) const { return file == other_file; }

string Requirement::to_string() { return file->to_string(); }

bool permit::is_ready() const {
    for( auto requirement : requirements)
        if( ! requirement.is_passed() )
            return false;

    return true;
}

permit::permit(string name, Organ* organ, Signature sign_type) 
    : Document(name), related_organ(organ), sign_type(sign_type){
        if(organ == nullptr){
            throw runtime_error("wrong organ added to form"); 
        }
}


void permit::add_requirement( Document* new_requirement){
    if(new_requirement == nullptr)
        throw runtime_error("new requirement cant be null");

    new_requirement->add_requitor(this);
    requirements.push_back(Requirement(new_requirement));
}
    

void permit::requirement_passed( Document* requirement){

    if(requirement == nullptr)
        throw runtime_error("passed requirement cant be null");

    for(int i = 0; i < requirements.size(); i++)
        if( requirements[i] == requirement )
            requirements[i].pass();
        

    if( is_ready() )
        requitors.empty() ? print_permit() : notify_requitors();
    
}

void permit::print_permit() { cout << to_string(); }

string permit::to_string(){
    ostringstream output;

    output << "Name: " << name << '\n';
    output << "This permit consists of: {\n";

    for( int i = 1; i <= requirements.size(); i++){
        output << TAB << "#" << i << '\n';

        output << shift_one_tab( requirements[i-1].to_string() );
    }

    output << "}\n";
    output << "Signature: " << related_organ->get_signature(sign_type) << '\n';

    return output.str();
}
