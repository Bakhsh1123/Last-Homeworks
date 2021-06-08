#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>

#define UNFILLED -1
#define GROUP_MEMBER_AMOUNT 3
#define STUDENTID_UNIT_SEPARATOR 4
#define ANY_STUDENT 3
#define EMPTY_LIST -1
#define NOT_AVAILBLE -1

#define GROUP_INDEX(STU_ID)   (STU_ID / STUDENTID_UNIT_SEPARATOR)
#define STUDENT_INDEX(STU_ID) (STU_ID % STUDENTID_UNIT_SEPARATOR)

using namespace std;

//studentID : number of group*4 + number of person in group (0 to 2) 
//studentID % STUDENTID_UNIT_SEPARATOR --> gives number of person in group | code 3 (ANY_STUDENT) main any student from that group
//studentID / STUDENTID_UNIT_SEPARATOR --> gives number of group
typedef int studentID;
typedef vector<vector <studentID>> studentIDTable;
typedef vector<vector <string>> studentNameTable;

//  **********  declearing functions
void input(int k, studentNameTable& students);
void output(studentNameTable names,studentIDTable  pattern,int k,int a,int s);

vector<int> shuffle(int to);
vector<int> organizedShuffle(int to, int outputLen);
void shuffleList(vector<int>& list);

int chooseAssignment(vector<int>& notUsed, vector<int>& used, studentIDTable assignmentTable);
studentID getStudentID(int groupIndex, int studentIndex);
bool hasItem(vector<int> list, int item);
bool isFilled (vector<studentID> row);
void add(vector<int>& list,int element);
int popFirst(vector<int>& list);
void removeItem(vector<int> list, int removing);

studentIDTable setGroupsLeastNessesaryPresents(int k, int a, int s);
studentIDTable LeastNessesaryPresentTable(studentIDTable groups, int k, int a, int s);
void fillTableUnfilleds(studentIDTable& assignmentTable, int k);
void fillUnfilledRow(vector<studentID>& row, int k);

int main(){
    //random number seed
    srand((int)time(0));
    //perminary data init
    int k,a,s;
    cin >> k >> a >> s;
    //          students[group num.][student num.]
    studentNameTable students(k, vector<string> (GROUP_MEMBER_AMOUNT)); 
    
    input(k, students);

    vector<vector<studentID>> groups = setGroupsLeastNessesaryPresents(k,a,s);
    
    vector<vector<studentID>> assignmentTable; 
    
    assignmentTable = LeastNessesaryPresentTable(groups, k, a, s);

    fillTableUnfilleds(assignmentTable, k);

    output(students, assignmentTable, k, a, s);

    return 0;
}

//determines each group's squense of presentor student for least needed presents 
studentIDTable setGroupsLeastNessesaryPresents(int k, int a, int s){

    int leastPresents = s*a/k;  //any group has at least [s*a/k] presents

    studentIDTable groups(k, vector<studentID>(leastPresents));
    vector<int> shuffledOrderOfStudent;


    //least present intialize
    for(int i = 0; i < k; i++){

       shuffledOrderOfStudent = organizedShuffle(GROUP_MEMBER_AMOUNT - 1,leastPresents);

       for(int j = 0; j < leastPresents; j++){
          groups[i][j] = getStudentID(i, shuffledOrderOfStudent[j]);
       }

    }

    return groups;
}


//putting groups in table
studentIDTable LeastNessesaryPresentTable(studentIDTable groups, int k, int a, int s){

    //          assignment table[assignment num.][present num. per assignment]
    vector<vector<studentID>>  assignmentTable(a, vector<studentID> (s,UNFILLED));

    vector<int> notUsedAssignments; //will store index of asignment that not used for last group presents
    vector<int> usedAssignments; //will store index of asignment that used for last group presents
    vector<int> isUsingAssignments; //will store index of asignment that is using by current group

    int usingIndex;

    for(vector<studentID> singleGroup : groups){
        
        //any item that is used must not be in not used list
        for(int assignmentIndex = 0; assignmentIndex < a; assignmentIndex++){
            if( !hasItem(usedAssignments, assignmentIndex) ){
                notUsedAssignments.push_back(assignmentIndex);
            }
        }
        
        shuffleList(notUsedAssignments);
        shuffleList(usedAssignments);

        for (int i = 0; i < singleGroup.size(); i++) {

            usingIndex = chooseAssignment(notUsedAssignments, usedAssignments, assignmentTable);

            if (usingIndex == NOT_AVAILBLE) {
                continue;
            }

            isUsingAssignments.push_back(usingIndex); //adding used index to used list
            add(assignmentTable[usingIndex], singleGroup[i]);

        }
        
        notUsedAssignments.clear();
        usedAssignments = isUsingAssignments;//now its time to change this two
        isUsingAssignments.clear();
 
    }

    return assignmentTable;
}

//will allocate proper student to empty assignment slots
void fillTableUnfilleds(studentIDTable& assignmentTable, int k) {

    for ( int i = 0; i < assignmentTable.size(); i++) {
        fillUnfilledRow(assignmentTable[i], k);
    }

}


void fillUnfilledRow(vector<studentID>& row, int k) {

    vector<int> notPresentedGroups = shuffle( k - 1);
    vector<int> unfilledIndexes;

    for (int i = 0; i < row.size(); i++) {
        if (row[i] == UNFILLED) {
            unfilledIndexes.push_back(i);
        }
        else {
            removeItem(notPresentedGroups, GROUP_INDEX(row[i]));
        }
    }

    for (int freeIndex : unfilledIndexes) {
        int randomStudent = rand() % GROUP_MEMBER_AMOUNT;
        int group = popFirst(notPresentedGroups);

        row[freeIndex] = getStudentID(group, randomStudent);
    }
}

int chooseAssignment(vector<int>& notUsed, vector<int>& used, studentIDTable assignmentTable) {
    int index;
    do {
        index = popFirst(notUsed);
        if (index == EMPTY_LIST) {
        
            index = popFirst(used);
            if (index == EMPTY_LIST) {
                return NOT_AVAILBLE;
            }
        }
    } while (isFilled(assignmentTable[index]));
    return index;
}


//make a shuffled list by 0 to 'to' values (containing 'to' )
vector<int> shuffle(int to){

    //if to has the 0 value this may cause some errors
    if(to < 1){
        return vector<int>(1,0);
    }

    vector<int> shuffledList(to + 1, UNFILLED);

    for(int i = 0; i <= to; i++){

        int  randIndex = rand()%(to + 1);

        while(shuffledList[randIndex] != UNFILLED){
            randIndex = (randIndex + 1)%(to + 1);
        }

        shuffledList[randIndex] = i;
    }
    
    return shuffledList;
}

//adds shuffled forms of numbers from 0 to "to" in end of squense to reach the size of goal vector and shuffle result at the end
vector<int> organizedShuffle(int to, int outputLen){

    vector<studentID> shuffledTemp = shuffle(to);
    int shuffledTempIndex = 0;

    vector<studentID> shuffledList(outputLen);
    
    for(int i = 0; i<outputLen; shuffledTempIndex++, i++){

        if( shuffledTempIndex == shuffledTemp.size() ){
            shuffledTempIndex = 0;
            shuffleList(shuffledTemp);
        }

        shuffledList[i] = shuffledTemp[shuffledTempIndex];
    }

    shuffleList(shuffledList);

    return shuffledList;
}

//shuffles given list
void shuffleList(vector<int>& list){

    int len = list.size();
    vector<int> shuffledList(len);
    vector<int> shuffledIndexes = shuffle(len-1);

    for(int i = 0; i<len; i++){
        shuffledList[i] = list[shuffledIndexes[i]];
    }

    list = shuffledList;
}

//calculate student ID by given data
studentID getStudentID(int groupIndex, int studentIndex){
    return groupIndex * STUDENTID_UNIT_SEPARATOR + studentIndex;
}

//checks if given item exists in given list or not
bool hasItem(vector<int> list, int item){

    for(int i = 0; i < list.size(); i++){
        if(list[i] == item){
            return true;
        }
    }

    return false;

}


//checks if a assignment is filled or not
bool isFilled (vector<studentID> row){

    for(int i = 0; i < row.size(); i++){
        if(row[i] == UNFILLED){
            return false;
        }
    }

    return true;
}

// will add element at first non -1 place in list
void add(vector<int>& list,int element){

    for(int i = 0; i < list.size(); i++){
        if(list[i] == UNFILLED){
            list[i] = element;
            return;
        }
    }
}

int popFirst(vector<int>& list) {

    if (list.size() != 0) {
        int item = list[0];
        list.erase(list.begin());
        return item;
    }
    else {
        return EMPTY_LIST;
    }

}

void removeItem(vector<int> list, int removing) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == removing) {
            list.erase(list.begin() + i);
        }
    }
}

// IO funcs
void input(int k, studentNameTable& students){
    for(int i = 0; i < k; i++){
        for(int j = 0; j < GROUP_MEMBER_AMOUNT; j++){
             cin >> students[i][j];
        }
    }
}
  
void output(studentNameTable names,studentIDTable pattern, int k, int a, int s){
    for(int i = 0; i < a; i++){
        for(int j = 0; j < s; j++){

            int groupID = GROUP_INDEX(pattern[i][j]);
            int studentNum = STUDENT_INDEX(pattern[i][j]);
            cout << names[groupID][studentNum] << " ";

        }
        cout << endl;
    }
}
