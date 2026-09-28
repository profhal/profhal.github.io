---
layout: page
title: "Example: static Attribute"
---

There are times when all instances of a class need to share a resource. For example, suppose you were modeling a swarm of angry bees that needed to chase a bear. You might start with

```cpp
class Position { ... };

class AngryBee {
private:

    Positon bearPosition;

public:
    
    AngryBee() { ... }

    void setBearPosition(Position newPosition) {
        bearPosition = newPosition;
    }
    
    // Other AngryBee stuff

};
```

Then you would likely model the swarm as 

```cpp
const int SWARM_SIZE = 500;

vector<AngryBee> swarm(SWARM_SIZE);
```

and updates on the bear's position would be done with a loop,

```cpp
bearPosition = bear.getPosition();

for (Bee& bee : swarm) {

    bee.setBearPosition(bearPosition);

}
```
<div class="vspace"></div>

Another way to solve this would be to use a `static` attribute. A `static` attribute is shared by all instances of the class. In our swarm example, every `AngryBee` had its own copy of the location of the bear. 

We start by declaring `bearPosition` to be `static` as well as the operation `setBearPosition()`,

```cpp
class AngryBee {
private:

    static Positon bearPosition;

public:
    
    AngryBee() { ... }

    static void setBearPosition(Position newPosition) {
        bearPosition = newPosition;
    }
    
    // Other AngryBee stuff

};
```

The operation `setBearPosition()` doesn't have to be static. However, since it only works with other `static` entities, we declare it static `static`. 

<div class="rule-box" markdown="1">

**C++ Rule:** `static` members can only directly access other `static` members of the class..

</div>

Now, to update all the `AngryBee` instances requires only one call,

```cpp
AngryBee::setBearPosition(bear.getPosition());
```

This updates the `static` attribute `bearLocation` and all `AngryBee` instances can see the new `Position`. 

Notice that we called `setBearPosition()` using the class name, `AngryBee`. Although C++ allows `static` members to be accessed through an object, using the class name is preferred because the member belongs to the class, not to a particular object.


## 1. An Simple, Explicit Example

Let's look at an explicit example of the impact declaring something static has. Let's define a simple class with one `static` attribute and one non-`static` attribute,

**Simple.h**
```cpp
#ifndef SIMPLE_H
#define SIMPLE_H

class Simple {
private:

    static int staticX;
    int regularX;

public:

    Simple();

    static void setStaticX(int newX);
    static int getStaticX();

    void setRegularX(int newX);
    int getRegularX() const;

};

#endif
```

<div class="rule-box" markdown="1">

**C++ Rule:** `static` members cannot include the `const` modifier.

</div>

and then implement the class

**Simple.cpp**
```cpp
#include "Simple.h"

// static attribute initialization
//
int Simple::staticX = 0;


Simple::Simple() : regularX(0) {};


void Simple::setStaticX(int newX) {

    staticX = newX;

}


int Simple::getStaticX() {

    return staticX;

}



void Simple::setRegularX(int newX) {

    regularX = newX;

}


int Simple::getRegularX() const {

    return regularX;

}
```

For more information on initializing `static` attributes, see section 3.2, *Initializing Non-constant static Attributes*, of [Attribute Initialization]({% link _programming/attribute-initialization-cpp.md %}).

Now, let's create a couple instances and see what happens. If you execute,

```cpp
#include <iostream>

#include "Simple.h"

using namespace std;


int main() {

    Simple one;
    Simple two;

    // Output the state of each Simple object.
    //
    cout << "ONE: staticX: " 
         << one.getStaticX()
         << "  regularX: "
         << one.getRegularX()
         << endl;

    cout << "TWO: staticX: " 
         << two.getStaticX()
         << "  regularX: "
         << two.getRegularX()
         << endl;


    // Change one's staticX and regularX.
    //
    one.setStaticX(5);        
    one.setRegularX(7);        


    // Output again the state of each Simple object.
    //
    cout << "ONE: staticX: " 
         << one.getStaticX()
         << "  regularX: "
         << one.getRegularX()
         << endl;

    cout << "TWO: staticX: " 
         << two.getStaticX()
         << "  regularX: "
         << two.getRegularX()
         << endl;


    return 0;
}
```
you will see the following ouput

```
> ./a.out
ONE: staticX: 0  regularX: 0
TWO: staticX: 0  regularX: 0
ONE: staticX: 5  regularX: 7
TWO: staticX: 5  regularX: 0
```

Notice that updating `staticX` through the object `one` was observed through the object `two` - both show the value initially at 0 and then 5. By contrast, only `one`'s version the non-static attribute, `regularX`, was updated - in the second half of the output, the values for `regularX` differ.

*Note:* In this example, we accessed the `static` operation via the object, `one`, rather than accessing it through the class. We did that hear to emphasize the shared aspect of a `static` attribute. Normally, we would follow convention.