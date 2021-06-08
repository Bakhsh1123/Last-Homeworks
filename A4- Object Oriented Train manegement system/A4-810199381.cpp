#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Station;
class Person;
class Train;

class Station {
public:

    Station(string _name, int _ID, int _toffy_price);

    void mosafer_begir();

    void train_exited(Train* exiting_train);

    void train_arrived(Train* arriving_train);

    void new_passenger_arrived(Person* new_passenger);

    string to_string();

    int get_toffy_price() { return toffy_price; }

    string get_name() { return name; }

private:
    string name;
    int ID;

    int toffy_price;

    vector< Person* > passengers;
    vector< Train* > available_trains;

    Train* find_proper_train(Person* passenger);
    Train* choose_one_train(vector< Train* > chooses);
};

class Person {
public:
    Person(int _ID, int _addiction_rate, const vector< Station* >& _destinations);

    void first_move();

    bool will_go_to(Station* destination);

    void arrived_to_next_destination();

    void mounted_up(Train* new_train);

    void eat_toffy();

    bool is_arrived();

    string to_string();

    int get_ID() { return ID; }

private:
    int ID;

    int addiction_rate;
    int spent;

    vector< Station* > destinations;

    bool is_on_train;

    Station* last_station;
    Train* last_train;

    bool travel_started;


};

class Train {
public:
    Train(int _ID, Station* _departure, Station* _destination, int _rest_time, int _travel_time);

    void first_move();

    void move();

    void mount_one(Person* passenger);

    string to_string();

    Station* get_destination() { return destination; };

    int get_ID() { return ID; }

private:
    int ID;

    Station* resting_place;
    Station* destination;

    int rest_time;
    int travel_time;

    int remined_time;
    bool is_traveling;

    vector< Person* > passengers;

    bool travel_started;

    void reverse_direction();
    void change_status();
    void dismount_all();
};

class World {
public:
    World();

    void advance_time(int steps);

    void finish();

    void add_station(string name, int toffy_price);

    void add_train(string start_station, string end_station, int travel_time, int rest_time);

    void add_passenger(int addiction_rate, vector<string> destination_names);

    string show_passenger_info(int ID);

    string show_train_info(int ID);

    void check_for_first_move();

private:
    int time;

    vector< Person > people;

    vector< Train > trains;

    vector< Station > stations;

    void update();

    bool are_all_arrived();

    Station* find_station(string name);

};
//      station methods


Station::Station(string _name, int _ID, int _toffy_price) {
    name = _name;
    ID = _ID;

    toffy_price = _toffy_price;
}

void Station::mosafer_begir() {

    for (int i = 0; i < passengers.size(); i++) {

        Train* train = find_proper_train(passengers[i]);

        if (train != nullptr) {
            train->mount_one(passengers[i]);
            passengers[i]->mounted_up(train);
            passengers.erase(passengers.begin() + i);
            i--;
        }
    }
}

Train* Station::find_proper_train(Person* passenger) {

    vector< Train* > chooses;

    for (auto train : available_trains) {
        if (passenger->will_go_to(train->get_destination())) {
            chooses.push_back(train);
        }
    }

    return choose_one_train(chooses);
}

Train* Station::choose_one_train(vector< Train* > chooses) {

    Train* choosed_train = nullptr;

    for (auto train : chooses) {
        if (choosed_train == nullptr || train->get_ID() < choosed_train->get_ID()) {
            choosed_train = train;
        }
    }

    return choosed_train;
}

void Station::train_exited(Train* exiting_train) {


    for (int i = 0; i < available_trains.size(); i++) {
        if (available_trains[i] == exiting_train) {
            available_trains.erase(available_trains.begin() + i);
            return; 
        }
    }
}

void Station::train_arrived(Train* arriving_train) {
    available_trains.push_back(arriving_train);
}

void Station::new_passenger_arrived(Person* new_passenger) {
    passengers.push_back(new_passenger);
    new_passenger->arrived_to_next_destination();
}

string Station::to_string() {

    return name;

}

//      person methods
Person::Person(int _ID, int _addiction_rate, const vector< Station* >& _destinations) {
    ID = _ID;

    addiction_rate = _addiction_rate;
    spent = 0;

    destinations = _destinations;

    is_on_train = false;
    last_train = nullptr;
    last_station = nullptr;

    travel_started = false;

}

void Person::first_move(){

    if (!travel_started) {
        destinations[0]->new_passenger_arrived(this);
        travel_started = true;
    }
}

bool Person::will_go_to(Station* destination) {
    return  !is_arrived() && destination == destinations[0];
}

void Person::arrived_to_next_destination() {
    is_on_train = false;
    last_station = destinations[0];
    destinations.erase(destinations.begin());
}

void Person::mounted_up(Train* new_train) {
    last_train = new_train;
    is_on_train = true;
}

void Person::eat_toffy() {
    if (!is_arrived() && !is_on_train)
        spent += addiction_rate * last_station->get_toffy_price();
}

bool Person::is_arrived() {
    return destinations.empty();
}

string Person::to_string() {
    ostringstream text;

    text << "status : " << (is_arrived() ? "arrived" : (is_on_train ? "on train" : "in station")) << '\n';

    text << "spent : " << spent << '\n';

    text << "current ";

    if (is_on_train)
        text << "train " << last_train->get_ID() << '\n';
    else
        text << "station " << last_station->get_name() << '\n';

    return text.str();
}

//      train methods

Train::Train(int _ID, Station* _departure, Station* _destination, int _rest_time, int _travel_time) {
    ID = _ID;

    resting_place = _departure;
    destination = _destination;

    rest_time = _rest_time;
    travel_time = _travel_time;

    remined_time = _rest_time;

    is_traveling = false;
    travel_started = false;
}

void Train::first_move(){
    if (!travel_started) {
        resting_place->train_arrived(this);
        travel_started = true;
    }
}

void Train::move() {

    remined_time--;

    if (remined_time == 0) {
        if (is_traveling) {
            destination->train_arrived(this);
            dismount_all();
            reverse_direction();
        }
        else {
            resting_place->train_exited(this);
        }

        change_status();
    }
}

void Train::mount_one(Person* passenger) {
    passengers.push_back(passenger);
}

void Train::dismount_all() {

    for (auto passenger : passengers)
        destination->new_passenger_arrived(passenger);

    passengers.clear();
}

void Train::reverse_direction() {

    Station* swap_buffer = resting_place;
    resting_place = destination;
    destination = swap_buffer;
}

void Train::change_status() {

    is_traveling = !is_traveling;
    remined_time = is_traveling ? travel_time : rest_time;
}


string Train::to_string() {
    ostringstream text;

    text << "status : " << (is_traveling ? "moving" : "resting") << '\n';
    text << "estimated remainig time : " << remined_time << '\n';
    text << "next station : " << destination->get_name() << '\n';
    text << "passengers count : " << passengers.size() << '\n';

    return text.str();
}

//      world methods

World::World() {
    time = 0;
}

void World::advance_time(int steps) {
    check_for_first_move();
    for (int i = 0; i < steps; i++) {
        update();
    }
}

void World::finish() {
    check_for_first_move();
    while (!are_all_arrived()) {
        update();
    }
}

void World::add_station(string name, int toffy_price) {

    stations.push_back(Station(name, stations.size() + 1, toffy_price));

}

void World::add_train(string start_station, string end_station, int travel_time, int rest_time) {

    Station* diparture = find_station(start_station);
    Station* destination = find_station(end_station);

    trains.push_back(Train(trains.size() + 1, diparture, destination, rest_time, travel_time));
}

Station* World::find_station(string name) {

    for (int i = 0; i < stations.size(); i++) {
        if (stations[i].get_name() == name) {
            return &stations[i];
        }
    }
    return nullptr;
}

void World::add_passenger(int addiction_rate, vector<string> destination_names) {

    vector< Station* > destinations;

    for (int i = 0; i < destination_names.size(); i++) {
        destinations.push_back(find_station(destination_names[i]));
    }

    people.push_back(Person(people.size() + 1, addiction_rate, destinations)); 
}

void World::update() {

    time++;

    for (auto& station : stations)
        station.mosafer_begir();

    for (auto& train : trains)
        train.move();

    for (auto& person : people)
        person.eat_toffy();

}

void World::check_for_first_move() {

    for (auto& train : trains)
        train.first_move();

    for (auto& person : people)
        person.first_move();
}

bool World::are_all_arrived() {

    for (auto& person : people) {
        if (!person.is_arrived()) {
            return false;
        }
    }
    return true;
}


string World::show_train_info(int ID) {

    for (auto& train : trains) {
        if (train.get_ID() == ID) {
            train.first_move();
            return train.to_string();
        }
    }
    return "\ttrain not found\n";
}

string World::show_passenger_info(int ID) {

    for (auto& person : people) {
        if (person.get_ID() == ID) {
            person.first_move();
            return person.to_string();
        }
    }
    return "\tpassenger not found\n";
}


class IO_manager {
public:

    bool read_command();

    void preform_command();

private:
    stringstream input;
    World world;
    string command;

    void add_passenger_command();

    void add_station_command();

    void add_train_command();

    void advance_time_command();

    void finish_command();

    void show_passenger_info_command();

    void show_train_info_command();

};


void IO_manager::add_passenger_command() {
    int addiction;
    string next_destination;
    vector< string > destination_list;

    input >> addiction;
    while (input >> next_destination) {
        destination_list.push_back(next_destination);
    }

    world.add_passenger(addiction, destination_list);

    cout << "OK" << endl;
}

void IO_manager::add_station_command() {
    string name;
    int toffy_price;

    input >> name >> toffy_price;

    world.add_station(name, toffy_price);

    cout << "OK" << endl;
}

void IO_manager::add_train_command() {
    string start_point;
    string end_point;
    int rest_time;
    int travel_time;

    input >> start_point >> end_point >> travel_time >> rest_time;

    world.add_train(start_point, end_point, travel_time, rest_time);

    cout << "OK" << endl;
}

void IO_manager::advance_time_command() {
    int steps;

    input >> steps;

    world.advance_time(steps);

    cout << "OK" << endl;
}

void IO_manager::finish_command() {
    world.finish();

    cout << "OK" << endl;
}

void IO_manager::show_passenger_info_command() {
    int ID;

    input >> ID;

    cout << world.show_passenger_info(ID);
}

void IO_manager::show_train_info_command() {
    int ID;

    input >> ID;

    cout << world.show_train_info(ID);
}


bool IO_manager::read_command() {
    string text;
    input.clear();
    command.clear();

    if (getline(cin, text)) {
        input.str(text);
        input >> command;
        return true;
    }


    return false;
}

void IO_manager::preform_command() {

  

    if (command == "add_passenger") {
        add_passenger_command();
    }
    else if (command == "add_station") {
        add_station_command();
    }
    else if (command == "add_train") {
        add_train_command();
    }
    else if (command == "advance_time") {
        advance_time_command();
    }
    else if (command == "finish") {
        finish_command();
    }
    else if (command == "show_passenger_info") {
        show_passenger_info_command();
    }
    else if (command == "show_train_info") {
        show_train_info_command();
    }
    else {
        cerr << "INVALID COMMAND" << endl;
    }

}

int main(int argc, char** argv) {

    IO_manager console;

    while (console.read_command())
        console.preform_command();

    return 0;
}
