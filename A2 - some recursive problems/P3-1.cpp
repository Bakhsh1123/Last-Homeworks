#include <iostream>
#include <vector>

#define SPACE " "
#define FAILURE -1
#define MAX_MOVES 4
#define PATH_LENGH 3

using namespace std;

//***********************       data structurs
typedef vector<vector<int>> cityMap;
typedef struct pathRoad node;
typedef struct visitedPointsData visitedPoint;
typedef struct pointData point;

//**********       path data
/*      note:
*
*   i will use linked lists to store data of visited points becauese
*   of better memory management 
*   also working by linked lists is realy similar to finding paths
*   note: last elements of lists will be used more frequently 
*   so having a link to pre element will make storing data and other 
*   operations (like searching) lot faster
*
*/
struct pathRoad{
    int x;
    int y;
    
    node* Previous;
};

node* setNode(int x, int y, node* previous){

    node* result = (node*)malloc(sizeof(node));        
                                             
    result->x = x;
    result->y = y;
    result->Previous = previous;

    return result;
}

//**********       visited points data
struct visitedPointsData{
    int x;
    int y;

    visitedPoint* previous;
};

void addNewVisitedPoint(visitedPoint** beginning, int newPointX, int newPointY){

    visitedPoint* newVisited = (visitedPoint*)malloc(sizeof(visitedPoint));

    newVisited->x = newPointX;
    newVisited->y = newPointY;

    newVisited->previous = *beginning;

    *beginning = newVisited;
}

bool hasPoint(int searchingX, int searchingY, visitedPoint* lastVisitedPoint){

    visitedPoint* curr = lastVisitedPoint;

    while( curr->previous != nullptr ){

        if(curr->x == searchingX && curr->y == searchingY){
            return true;
        }

        curr = curr->previous;
    }
    
    return false;
}

//**********       visited points data
struct pointData{
    int x;
    int y;
};

point newPoint(node* data){
    point result;
    result.x = data->x;
    result.y = data->y;
    return result;
}

//***********************       perminary tools
int abs(int number){
    if(number < 0){
        return -number;
    }
    return number;
}

bool isPosibleMove(cityMap& map, node* currentPosition, int new_X, int new_Y,
                   visitedPoint* lastVisitedNode){

    if(new_X < 0 || new_X >= map[0].size() || new_Y < 0 || new_Y >= map.size()){
        return false;
    }

    if(abs(map[new_Y][new_X] - map[currentPosition->y][currentPosition->x]) != 1){
        return false;
    }

    if(hasPoint(new_X, new_Y, lastVisitedNode)){
        return false;
    }
    
    return true;
}

void calculateNewDestination(node* currentPosition, int& new_X, int& new_Y, int moveNumber){
    const int MOVE_ON_X[] = {1, 0, -1,  0};
    const int MOVE_ON_Y[] = {0, 1,  0, -1};
   
    new_X = currentPosition->x + MOVE_ON_X[moveNumber];
    new_Y = currentPosition->y + MOVE_ON_Y[moveNumber];
}

void saveAnswer(node* newPath, int pathLengh, vector<point>& pathSlot){

    pathSlot.clear();

    node* currentNode = newPath;

    for( int i = 0; i < pathLengh; i++){

        pathSlot.push_back( newPoint(currentNode) );
        currentNode = currentNode->Previous;
    }
}

//***********************       main recursive functions
bool pathFound(cityMap& map, node* currentPosition, visitedPoint* lastVisitedNode,
               vector<point>& foundPath, int pathLengh){
    
    addNewVisitedPoint( &lastVisitedNode, currentPosition->x, currentPosition->y );
    
    pathLengh++;

    if(  pathLengh == PATH_LENGH ){

        saveAnswer(currentPosition, pathLengh, foundPath);
        free(currentPosition);
        return true;
    }

    int new_X, new_Y;

    for( int move = 0; move < MAX_MOVES; move++ ){

        calculateNewDestination(currentPosition, new_X, new_Y, move); 
        
        if(isPosibleMove(map, currentPosition, new_X, new_Y, lastVisitedNode)){

            node* nextPoint = setNode(new_X, new_Y, currentPosition);
   
            if(pathFound(map, nextPoint, lastVisitedNode, foundPath, pathLengh)){
                return true;
            }
        }
    }

    free(lastVisitedNode);
    free(currentPosition);

    return false;
}

vector<point> findPath(cityMap& map, const int MAX_X, const int MAX_Y){

    vector<point> largestPath;

    for(int i = 0; i < MAX_Y; i++){
        for(int j = 0; j < MAX_X; j++){
            
            node* newNode = setNode( j, i, nullptr);

            if(pathFound(map, newNode, nullptr, largestPath, 0)){
                return largestPath;
            }
        }
    }

    return largestPath;
}

void printPath(cityMap map, vector<point> path){

    if( path.size() <= 0 ){
        cout << FAILURE;
    }

    for( point position : path ){
        cout << map[position.y][position.x] << SPACE;
    }
}

int main(){

    int MAX_X, MAX_Y;

    cin >> MAX_X >> MAX_Y;

    cityMap map(MAX_Y, vector<int> (MAX_X));

    for(int i = 0; i < MAX_Y; i++){
        for(int j = 0; j < MAX_X; j++){
            cin >> map[i][j];
        }
    }

    vector<point> answer = findPath(map, MAX_X, MAX_Y);
    
    printPath(map, answer);

    return 0;
}