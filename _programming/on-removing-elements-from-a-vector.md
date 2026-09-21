---
layout: page
title: "On Removing Elements from a Vector"
---

In C++, when it comes time to remove elements from a `vector<>`, you have two choices:

- rebuild the `vector<>` and omit certain values
- use `vector<>::erase()` to remove elements

The challenge is that the two can have significantly different performance times. Here, we look at two situations. For the first, we will manage a `vector<int>` and time the removal of a set of values using both the rebuilding approach and the `vector<>::erase()` operation. In the second, we create a larger data structure and do the same thing. As we will see, when using `int`s we get one result and see another when dealing with the larger structure. The point is that you have to consider your application when choosing.

### Trying The Code
The below can be integrated into a [single file]({{ '/assets/programming/on-removing-elements-from-a-vector/eraseVersusBuild.cpp' | relative_url }}). The headers required are 

```cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
```

## Managing a Vector of `int`
We start with a `vector<>` of `int`s. The list will consist of 0’s and 1’s. A zero will indicate an item should be removed from the collection when we compare techniques. The function, `getIntList()` will generate a list of length `numberOfElements` and randomly flag `numberOfZeroes` elements to be removed.

```cpp
vector<int> getIntList(int numberOfElements, int numberOfZeros) {

    vector<int> result;

    vector<int> zeroes;

    vector<int>::iterator it;

    int newZero;


    for (int z=0; z < numberOfZeros; ++z) {

        do {

            newZero = rand() % numberOfElements;

        } while(find(zeroes.begin(), zeroes.end(), newZero) 
                                                  != zeroes.end());

        zeroes.push_back(newZero);

    }


    for (int e=0; e < numberOfElements; ++e) {

        if (find(zeroes.begin(), zeroes.end(), e) 
                                        == zeroes.end()) {

            result.push_back(1);

        } else {

            result.push_back(0);

        }

    }

    return result;   

}
```

We generate one list and make copies for each of the approaches to work with. We will time two separate functions. The first is `doIntRebuild()`, where we create a new `vector<>` by populating it with the unflagged elements (non-zeros).

```cpp
vector<int> doIntRebuild(const vector<int> &list) {

    vector<int> result;

    for (auto x : list) {
 
        if (x != 0) {

            result.push_back(x);

        } 

    }

    return result;

}
```

The second function, `doIntErase()`, we use `vector<>::erase()` to remove the elements from the source list.

```cpp
void doIntErase(vector &list) {

    auto it = list.begin();

    while (it != list.end()) {

        if (*it == 0)  {

            list.erase(it);

        } else {

            ++it;

        }

    }

}
```

Lastly, we create a `main()` that times the two functions,

```cpp
int main() {

    const int SIZE = 10000;
    const int REMOVE_COUNT = 1000;

    srand(time(0));
    
    vector intList = getIntList(SIZE, REMOVE_COUNT);

    vector rebuildIntList(intList);
    vector eraseIntList(intList);

    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point stop;

    chrono::duration elapsed;
    chrono::microseconds elapsedUSec;


    // INT LIST : REBUILD
    //
    cout << “INT LIST” << endl;

    start = chrono::high_resolution_clock::now();

    rebuildIntList = doIntRebuild(rebuildIntList);

    stop = chrono::high_resolution_clock::now();

    elapsed = stop - start;

    elapsedUSec = chrono::duration_cast(elapsed);

    cout << “Time to rebuild: “ 
         << elapsedUSec.count() 
         << “ microseconds.” 
         << endl;


    // INT LIST : ERASE
    //
    start = chrono::high_resolution_clock::now();

    doIntErase(eraseIntList);

    stop = chrono::high_resolution_clock::now();

    elapsed = stop - start;

    elapsedUSec = chrono::duration_cast(elapsed);

    cout << “Time to erase  : “ 
         << elapsedUSec.count() 
         << “ microseconds.” 
         << endl;

    return 0;

}
```

Running this program on a Mac with an Intel 3.8 GHz i7 processor, and varying the number of elements in the lists as well as the number of elements that should be removed, we get the following results:

<div class="table-caption">Time to remove k elements from a vector&lt;int&gt; of length n.</div>

| Approach         | n=1000 k=100 | n=5000 k=100 | n=5000 k=500 | n=10000 k=1000|
|:-----------------|:------------:|:------------:|:------------:|:-------------:|
|  rebuild         | 29 μs        |   137 μs     |  146 μs      |  286 μs       |
|  vector<>::erase | 24 μs        |   84 μs      |  155 μs      |  448 μs       |

 We see that in some cases ``vector<>::erase()` is more efficient, but as the list size and number of elements to be removed grows, rebuilding seems to be the better choice.


 ## Managing a Vector of Large Objects
 We now do the same thing with a larger structure, `BigBoy`, 

 ```cpp
 struct BigBoy {
    int data[100];
};
```

That is, we generate a list of `BigBoy` objects where some have the values in the underlying array, `data`, are set to 0 to indicate they should be removed and others with a 1.

```cpp
vector<BigBoy> getBigBoyList(int numberOfElements, int numberOfZeros) {

    vector<BigBoy> result;

    vector<int> zeroes;

    vector<int>::iterator it;

    int newZero;

    BigBoy bigBoy;


    for (int z=0; z < numberOfZeros; ++z) {

        do {

            newZero = rand() % numberOfElements;

        } while(find(zeroes.begin(), zeroes.end(), newZero) 
                                                  != zeroes.end());

        zeroes.push_back(newZero);

    }

    for (int e=0; e < numberOfElements; ++e) {

        if (find(zeroes.begin(), zeroes.end(), e) 
                                          == zeroes.end()) {

            bigBoy.data[0] = 1;

        } else {

            bigBoy.data[0] = 0;

        }

        result.push_back(bigBoy);

    }

    return result;

}
```

We also create two functions, `doBigBoyRebuild()` and `doBigBoyErase()`, which are analgous to what we did with the `int` vectors.

```cpp
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
```

When we updated the main() to use these functions, we observed the performance shown in Table 2.

<div class="table-caption">Time to remove k elements from a vector&lt;BigBoy&gt; of length n.</div>

| Approach         | n=1000 k=100 | n=5000 k=100 | n=5000 k=500 | n=10000 k=1000|
|:-----------------|:------------:|:------------:|:------------:|:-------------:|
|  rebuild         | 193 μs       |   1,051 μs   |  1,037 μs    |  2,299 μs     |
|  vector<>::erase | 411 μs       |   2,218 μs   |  11,249 μs   |  42,362 μs   |

Clearly, there is a need to consider how to manage removal of items from a container.

