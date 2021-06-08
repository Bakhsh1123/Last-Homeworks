#include <iostream>
#include <vector>

using namespace std;

int findMaxHight(int buildingCount, vector<int>& hightList, int checkingIndex){

    if(checkingIndex + 1 >= buildingCount){
        return checkingIndex;
    }else if(hightList[checkingIndex] > hightList[checkingIndex + 1]){
        return checkingIndex;
    }else{
        return findMaxHight(buildingCount, hightList, checkingIndex + 1);
    }

}

int main(){

    int buildingCount;

    cin >> buildingCount;

    vector<int> hightList(buildingCount);

    for(int i = 0; i < buildingCount; i++){
        cin >> hightList[i];
    }

    cout << findMaxHight(buildingCount, hightList, 0);

    return 0;
}