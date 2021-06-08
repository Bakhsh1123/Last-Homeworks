#include <iostream>
#include <vector>

#define BASE 10
#define FAILURE "-1"
#define SPACE " "

using namespace std;

void addOneDigit(int reminedDigits, int distanseOfTwoDigit, int baseNumber, vector<int>& answers){

    if(reminedDigits == 0){

        answers.push_back(baseNumber);

    }else{
        
        int lastDigit = baseNumber % BASE;

        int lessNewDigit = lastDigit - distanseOfTwoDigit;
        int moreNewDigit = lastDigit + distanseOfTwoDigit;

        if ( distanseOfTwoDigit == 0){

            addOneDigit(reminedDigits - 1, distanseOfTwoDigit, baseNumber * BASE + lastDigit, answers);
            return;
        }

        if(0 <= lessNewDigit && lessNewDigit < BASE){
            addOneDigit(reminedDigits - 1, distanseOfTwoDigit, baseNumber * BASE + lessNewDigit, answers);
        }

        if(0 <= moreNewDigit && moreNewDigit < BASE){
            addOneDigit(reminedDigits - 1, distanseOfTwoDigit, baseNumber * BASE + moreNewDigit, answers);
        }
    }
}


void findCode(int reminedDigits, int distanseOfTwoDigit, vector<int>& answers, int firstDigit = 1){

    if(firstDigit < BASE){
        addOneDigit(reminedDigits, distanseOfTwoDigit, firstDigit, answers);
        findCode(reminedDigits, distanseOfTwoDigit, answers, firstDigit + 1);
    }

}

void printList(vector<int>& list, int currentIndex = 0);

int main(){

    int digitsCount; 
    int distanseOfTwoDigit;

    cin >> digitsCount >> distanseOfTwoDigit;

    vector<int> answers;

    findCode(digitsCount - 1, distanseOfTwoDigit, answers);

    if(answers.empty()){
        cout << FAILURE;
    }else{
        printList(answers);
    }

    return 0;
}

void printList(vector<int>& list, int currentIndex){

    if(currentIndex < list.size()){
        cout << list[currentIndex] << SPACE;
        printList(list, currentIndex + 1);
    }

}