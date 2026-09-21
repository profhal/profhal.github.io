#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;


struct BigBoy {
    int data[100];
};


vector<BigBoy> getBigBoyList(int numberOfElements, int numberOfZeros);
vector<int> getIntList(int numberOfElements, int numberOfZeros);

vector<int> doIntRebuild(const vector<int> &data);
vector<BigBoy> doBigBoyRebuild(const vector<BigBoy> &data);

void doIntErase(vector<int> &data);
void doBigBoyErase(vector<BigBoy> &data);


int main() {

    const int SIZE = 10000;
    const int REMOVE_COUNT = 1000;

    srand(time(0));
    
    vector<int> intList = getIntList(SIZE, REMOVE_COUNT);
    vector<BigBoy> bigBoyList = getBigBoyList(SIZE, REMOVE_COUNT);\


    vector<int> rebuildIntList(intList);
    vector<BigBoy> rebuildBigBoyList(bigBoyList);

    vector<int> eraseIntList(intList);
    vector<BigBoy> eraseBigBoyList(bigBoyList);

    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point stop;

    chrono::duration<double> elapsed;
    chrono::microseconds elapsedUSec;


    // INT LIST : REBUILD
    //
    cout << "INT LIST" << endl;

    start = chrono::high_resolution_clock::now();

    rebuildIntList = doIntRebuild(rebuildIntList);

    stop = chrono::high_resolution_clock::now();

    elapsed = stop - start;

    elapsedUSec = chrono::duration_cast<chrono::microseconds>(elapsed);

    cout << "Time to rebuild: " << elapsedUSec.count() << " microseconds." << endl;


    // INT LIST : ERASE
    //
    start = chrono::high_resolution_clock::now();

    doIntErase(eraseIntList);

    stop = chrono::high_resolution_clock::now();

    elapsed = stop - start;

    elapsedUSec = chrono::duration_cast<chrono::microseconds>(elapsed);

    cout << "Time to erase  : " << elapsedUSec.count() << " microseconds." << endl;




    // BIG BOY LIST : REBUILD
    //
    cout << "BIG BOY LIST" << endl;

    start = chrono::high_resolution_clock::now();

    rebuildBigBoyList = doBigBoyRebuild(rebuildBigBoyList);

    stop = chrono::high_resolution_clock::now();

    elapsed = stop - start;

    elapsedUSec = chrono::duration_cast<chrono::microseconds>(elapsed);

    cout << "Time to rebuild: " << elapsedUSec.count() << " microseconds." << endl;


    // BIG BOY LIST : ERASE
    //
    start = chrono::high_resolution_clock::now();

    doBigBoyErase(eraseBigBoyList);

    stop = chrono::high_resolution_clock::now();

    elapsed = stop - start;

    elapsedUSec = chrono::duration_cast<chrono::microseconds>(elapsed);

    cout << "Time to erase  : " << elapsedUSec.count() << " microseconds." << endl;


    return 0;

}


vector<int> getIntList(int numberOfElements, int numberOfZeros) {

    vector<int> result;

    vector<int> zeroes;

    vector<int>::iterator it;

    int newZero;


    for (int z=0; z < numberOfZeros; ++z) {

        do {

            newZero = rand() % numberOfElements;

        } while(find(zeroes.begin(), zeroes.end(), newZero) != zeroes.end());

        zeroes.push_back(newZero);

    }


    for (int e=0; e < numberOfElements; ++e) {

        if (find(zeroes.begin(), zeroes.end(), e) == zeroes.end()) {

            result.push_back(1);

        } else {

            result.push_back(0);

        }

    }

    return result;   

}



vector<BigBoy> getBigBoyList(int numberOfElements, int numberOfZeros) {

    vector<BigBoy> result;

    vector<int> zeroes;

    vector<int>::iterator it;

    int newZero;

    BigBoy bigBoy;


    for (int z=0; z < numberOfZeros; ++z) {

        do {

            newZero = rand() % numberOfElements;

        } while(find(zeroes.begin(), zeroes.end(), newZero) != zeroes.end());

        zeroes.push_back(newZero);

    }

    for (int e=0; e < numberOfElements; ++e) {

        if (find(zeroes.begin(), zeroes.end(), e) == zeroes.end()) {

            bigBoy.data[0] = 1;

        } else {

            bigBoy.data[0] = 0;

        }

        result.push_back(bigBoy);

    }

    return result;

}



vector<int> doIntRebuild(const vector<int> &data) {

    vector<int> result;

    for (auto x : data) {
 
        if (x != 0) {

            result.push_back(x);

        } 

    }

    return result;

}



void doIntErase(vector<int> &data) {

    auto it = data.begin();

    while (it != data.end()) {

        if (*it == 0)  {

            data.erase(it);

        } else {

            ++it;

        }

    }

}



vector<BigBoy> doBigBoyRebuild(const vector<BigBoy> &data) {

    vector<BigBoy> result;

    for (auto bigBoy : data) {
 
        if (bigBoy.data[0] != 0) {

            result.push_back(bigBoy);

        } 

    }

    return result;

}



void doBigBoyErase(vector<BigBoy> &data) {

    auto it = data.begin();

    while (it != data.end()) {

        if (it->data[0] == 0)  {

            data.erase(it);

        } else {

            ++it;

        }

    }

}




