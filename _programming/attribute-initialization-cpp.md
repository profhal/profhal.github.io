---
layout: page
title: "Attribute Initialization (C++)"
---

Classes will often have attributes and those attributes typically need a default value when an instance is created. One way to initialize the attributes, is to assign values in the constructor. For example, if we have the class

```cpp
class Fraction {
private:

    int numerator;
    int denominator;

public:

    Fraction();

    // other good Fraction operations

};
```

A reasonable default value for a `Fraction` is simply 0, where `numerator` is 0 and `denominator` is 1. A typical implementation of the constructor would look as follows:

```cpp
Fraction::Fraction() {

    numerator = 0;
    denominator = 1;

}
```

However, in C++, there is another approach called the *member initializer list*. In this approach, you list the attributes, separated by commas, after a colon that follows the constructor declaration and place their initial values in the parentheses. The implementation of the `Fraction` constructor would then look like

```cpp
Fraction::Fraction() 
         : numerator(0), denominator(1) {}
```

Both approaches result in a Fraction being 0/1. If the result is the same, it’s natural to wonder which should be favored and if one is required in certain instances.


### Preference: Member InitializationLists
As it turns out, member initialization lists are more efficient than initializing attributes within the constructor body. When we do the initialization in the constructor body two things happen:

- the object comes into existence
- an assignment happens

This would be akin to the following statements:

```cpp
string name;

name = “Tom”;
```

The first line invokes the default `string` constructor, getting the object ready for use, and the second line invokes the `string` assignment operator, `=`. We also know that we can create a `string` using a different constructor, such as the one that takes a c-string,

string name(”Tom”);

This invokes only the constructor and uses the supplied data to initialize the underlying array of characters.

There is a difference in performance. The following program times how long each method takes by trying each method 1,000,000 times:

```cpp
#include <chrono>
#include <iostream>
#include <string>

int main() {

    const int ATTEMPTS = 1000000;

    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;

    std::chrono::nanoseconds duration;
    
    // Try the constructor that takes a c-string argument.
    //
    start = std::chrono::system_clock::now();

    for (int i = 0; i < ATTEMPTS; i++) {

        std::string name(”Tom”);

    }

    stop = std::chrono::system_clock::now();

    duration = 
        std::chrono::duration_cast<std::chrono::nanoseconds>
                                                   (stop - start);

    std::cout << “string(char*): “
              << duration.count() / ATTEMPTS
              << “ nanoseconds”
              << std::endl;


    // Try the default constructor then assignment.
    //
    start = std::chrono::system_clock::now();

    for (int i = 0; i < ATTEMPTS; i++) {

        std::string name;
        name = “Tom”;

    }

    stop = std::chrono::system_clock::now();

    duration = 
        std::chrono::duration_cast<std::chrono::nanoseconds>
                                                   (stop - start);

    std::cout << “string() plus assignment: “
              << duration.count() / ATTEMPTS
              << “ nanoseconds”
              << std::endl;

    return 0;

}
```

On a Mac with an Intel i7-3.8Ghz processor, The results are in Table 1.

<div class="table-caption">Time to initialize one instance in two different ways.</div>

| Method   | Time to Initialize one instance |
|:--------:|:------------------------------:|
| c-string constructor | 14 ns |
| default constructor + assignment | 19 ns |
{: .numbered-table}

It’s likely that `string` is not an overly complicated class. If a class has more going on with its creation and assignment, the impact could be greater.


## How Much of an Impact?

Glad you asked. Let’s look at an example where a class’s underlying attribute is large. We will begin with the class `BigBoy`,

```cpp
class BigBoy {
private:
    std::vector data;

public:

    BigBoy() : data(1000000, 0) {}

    BigBoy(int value, size_t n) : data(n, value) {}


};
```

Then we need to create classes that compose `BigBoy`. In one class, `BigBoyMIL`, we will initialize the attribute using a member initialization list (MIL); in the other, `BigBoyCB`, we will initialize the attribute in the constructor body (CB).

```cpp
class BigBoyMIL {

private:
    
    BigBoy bb;

public:

    BigBoyMIL(int value) : bb(value, 1000000) {}

};


class BigBoyCB {

private:
    
    BigBoy bb;

public:

    BigBoyCB(int value) {

        bb = BigBoy(value, 1000000);

    }
    
};
```

Lastly, we run a similar timing program to the one involving strings. Note that we are using large objects so we dial back the number of attempts to 100.

```
#include <chrono>
#include <iostream>
#include <string>

int main() {

    const int ATTEMPTS = 100;

    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;

    std::chrono::milliseconds duration;
    

    // Try the member initialization list.
    //
    start = std::chrono::system_clock::now();

    for (int i = 0; i < ATTEMPTS; i++) {

        BigBoyMIL bmil(1);

    }

    stop = std::chrono::system_clock::now();

    duration = 
        std::chrono::duration_cast<std::chrono::milliseconds>
                                                   (stop - start);

    std::cout << “Member initialization: “
              << duration.count() / ATTEMPTS
              << “ milliseconds”
              << std::endl;


    // Try the constructor body.
    //
    start = std::chrono::system_clock::now();

    for (int i = 0; i < ATTEMPTS; i++) {

        BigBoyCB bcb(1);

    }

    stop = std::chrono::system_clock::now();

    duration = 
        std::chrono::duration_cast<std::chrono::milliseconds>
                                                   (stop - start);

    std::cout << “Constructor body: “
              << duration.count() / ATTEMPTS
              << “ milliseconds”
              << std::endl;

    return 0;

}
```

Here we can see a significant difference in performance in Table 2. (Again, on a Mac with an i7-3.8GHz processor.)

<div class="table-caption">Time to initialize one instance involving `BigBoy` in two different ways.</div>

| Method   | Time to Initialize one instance |
|:--------:|:------------------------------:|
| member list initialization | 8,058 μs |
| default constructor with assignment | 16,069 μs |
{: .numbered-table}


### A Note On The Design

We had to provide a default constructor so that `BigBoyCB` would compile. Since we are not using a member initialization list in the `BigBoyCB` example, the compiler would complain.

While we did need the default constructor, things would have still worked if we didn’t initialize the vector, `data`. In other words, would could have implemented BigBoy as

```cpp
class BigBoy {
private:
    std::vector<int> data;

public:

    BigBoy() {}

    BigBoy(int value, size_t n) : data(n, value) {}


};
```

However, `data` would be trivial to create in the default constructor instance. In it’s current form, we asked for a large collection of values (a million zeroes), which simulates the other constructor’s use in both `BigBoyMIL` and `BigBoyCB`. In both classes, we leverage the non-default constructor and ask for a vector with 1,000,000 values.

In the constructor body approach, `BigBoyCB`, what is happening is `bb` comes into existence, creates a huge vector, then disposes of it, and creates another `BigBoy` object with another million values. If we changed it to

```cpp
class BigBoy {
private:
    std::vector<int> data;

public:

    BigBoy() {} // required as we provide the other

    BigBoy(int value, size_t n) : data(n, value) {}


};
```

we see the performance displayed in Table 3.

<div class="table-caption">Time to initialize one instance when `BigBoy` uses member list initialization.</div>

| Method   | Time to Initialize one instance |
|:--------:|:------------------------------:|
| member list initialization | 7,827 μs |
| default constructor with assignment | 7,831 μs |
{: .numbered-table}

The two are very close. This is because creating the object with an empty array is trivial. The times reflect the copying of the underlying attribute.


### Attribute Initialization Order
The order in which attributes are initialized is based on the order in which they are specified in the class declaration. The order in which they are listed in the member initialization list is irrelevant. For more information, see [Initialization Ordering]({% link _programming/initialization-ordering.md %}).


## Required Member Intialization Lists
There are two cases when you must use member initilialization lists:

- initializaing constant attributes
- initializeing reference attributes

### Initializing Constant Attributes
Sometimes we want attributes in a class to be constant, but we won’t know it’s value until runtime. Constant attributes need to be initialized via member initialization lists as they cannot be assigned within the constructor body. Consider the following class:

```cpp
class Student {
private:

    const std::string ID;

    // other Student attributes
    
public:

    Student(std::string initId) {}

    // Other student operations

};
```

What this class is indicating is that a `Student` requires an `ID` to be created, but that `ID` is to be constant within the class (prohibiting accidental modification). Since we can’t set it in the constructor body, we have to use the member initialization list,


```cpp
Student::Student(std::string initId) 
       : ID(initId) {}
```

### Initializaing static Constant Attributes
In the previous example, we had to take the approach we did because the constant’s value wasn’t known ahead of time. Also, while `ID` is constant within `Student`, each `Student` instance will have its own `ID`.

Sometimes an attribute is constant and always has the same value. A classic example would be π. Consider a class, `Circle`, which maintains a radius and can provide its area. To calculate area, we need π, but π doesn’t change from `Circle` to `Circle`. In this case, we can use a `static` constant and `static` constants can be set in the class declaration:

```cpp
class Circle {
private:

    static const double PI = 3.141592653589793;
    double radius;

public:

    Circle(double initRadius) : radius(initRadius) {}

    double getArea() ) return { PI * radius * radius; }

};
```

You can do the same with non-static attributes (constant or otherwise) if you happen to know an appropriate default value (for example, initializing a counter to 0.)

Now, as `static` entities have class scope and so do not require an instance to be accessed, they cannot be initialized in a member initialization list, which are placed on constructors and so involved with creating instances.

#### static Constants Set By Function Calls
There are times when a `static` constant needs to be defined via a function call. Suppose we are developing a game where we spawn dice, and all dice have the same number of sides, $n$, where $n$ is an integer in the interval [3, 20] (they all [exist in reality](https://www.diceemporium.com/shop/)). However, each run of the game results in different dice of a different size. That is, on one run we spawn 10-sided dice, but on the next run we spawn 15-sided dice. 

This would be the declaration of class `Die`:

```cpp
class Die {
public:

    static const int SIDES;

private:

    int currentFace;

public:

    Die();

    int getCurrentFace() const;

    // Updates the current face and returns the value.
    //
    int roll();

};
```

We need to set the value of `SIDES` and we handle this in implementation file:

```cpp
const int Die::SIDES = <some value>
```

Since we want the value to be random, we might keep it simple and invoke `rand()`. A first attempt might be to simply write

```cpp
const int Die::SIDES = (rand() % 18) + 3; // range 3-20
```

However, doing this doesn’t give you randomness from run to run. The problem is that the random number generator needs to be seeded. A seed gives rise to a given sequence of numbers. Using the same seed will result in those same numbers appearing in the same order. If you don’t provide a seed, the generator is seeded with the value 1. This is what is happening: Every run uses the seed 1.

Since you can’t seed the random number generator and get a random number as a single expression, we need to create a function that does. Here is what the implementation for Die might look like after adding such a function:

```cpp
#include “Die.h”

#include <cmath>
#include <ctime>

int getRandomNumber(int min, int max) {

    srand(time(0));

    return rand() % (max - min + 1) + min;

}

const int Die::SIDES = getRandomNumber(3, 20);

Die::Die() {
    roll();
}

int Die::getCurrentFace() const {
    return currentFace;
}

int Die::roll() {
    currentFace = rand() % SIDES + 1;
    return currentFace;
}
```

Now, every time you run the program, the dice will haev a different number of sides.


#### Problem: We Might be Seeding More Than Once
With this implementation, the random number generator gets seeded every time `getRandomNumber()` is called. Since it is declared in a `.cpp` file, and we almost never include `.cpp` files, the function’s use is isolated to the file. Further, in the example, the function is only ever called once.

However, this might not be the case in general. What if we need to call the function several times to randomly initialize a set of values. This would be a problem.

Suppose we were creating a video game where NPCs were clones of each other. However, like the number of sides on a die, we want the clones to look different from run to run. Let’s start with the `Clone` header,

```cpp
class Clone {
public:
    static const int EYE_COLOR_INDEX;
    static const int HAIR_COLOR_INDEX;
    static const int HAIR_LENGTH_INDEX;

    // other clone attributes, static and non-static

public:

   // Clone operations

};
```

As with setting `SIDES` for `Die`, we set the values of the various indices in the implementation file, this time with a little help:

```cpp
#include <cmath>
#include <ctime>
#include <string>
#include <vector>

const std::vector<std::string> EYE_COLOR = 
                     {”blue”, “green”, “brown”, “black”, “hazel”};
const std::vector<std::string> HAIR_COLOR = 
                     {”blonde”, “brown”, “black”, “red”, “gray”};

const std::vector<std::string> HAIR_LENGTH = 
                     {”short”, “medium”, “long”};

int getRandomNumber(int min, int max) {

    srand(time(0));

    return rand() % (max - min + 1) + min;

}

const int Clone::EYE_COLOR_INDEX = 
                 getRandomNumber(0, EYE_COLOR.size() - 1);

const int Clone::HAIR_COLOR_INDEX =
                 getRandomNumber(0, HAIR_COLOR.size() - 1);

const int Clone::HAIR_LENGTH_INDEX =
                 getRandomNumber(0, HAIR_LENGTH.size() - 1);

// 
// The rest of the Clone implementation.
//
```

This may seem reasonable at first glance, but there is a problem: we are calling `getRandomNumber()` three times and so seeding the generator three times. To make matters worse, those three calls to `getRandomNumber()` will execute in succession very quickly.

The reason we are interested in when the execution happens is that we are seeding the random number generator with the current system clock tick - `time(0)`. This value is updated every second (it represents the number of seconds since midnight, January 1, 1970). Since the execution is so quick, it could be the case that all three calls happen within the same second. This means we reseed with the same value all three times. The impact is that the sequence of numbers is restarted and all three attributes would store the first number in the sequence. As a result, things don’t seem random.


#### Solution: Use a static Local Variable
Like classes, functions can have `static` features as well - `static` local variables. Just as `static` attributes are shared across all instances of a class, a `static` local variable is shared across all invocations of a function. While `static` local variables aren’t common, here may arguably be a good use.

We only need to make a simple change to the function `getRandomValue()`:

```cpp
int getRandomNumber(int min, int max) {

    static bool firstCall = true; // <-- line of interest

    if (firstCall) {

        srand(time(0));
        firstCall = false;
    
    }

    return rand() % (max - min + 1) + min;

}
```

By declaring `firstCall` to be `static`, it is initialized to `true` on the first call. This allows the `if` statement to be entered, where we do the seeding and toggle `firstCall` to `false`, indicating a call has already happened. On subsequent calls, the `static` variable is not reinitialized so it will remain `false` forever. In this case, we only seed the generator once and things will work.

#### For the Advanced Reader
We still have a problem: This approach isn’t threadsafe. But that’s a different conversation.

### Initializing Reference Attributes

There are times when a class needs to interact with an object maintained elsewhere in the system. It would not be uncommon to use a pointer,

```cpp
class ServiceUser {
private:

    Service *service;

public:

    ServiceUser(Service *initService);

    // Other ServiceUser operations - but no setService()

}
```

In this example, the implication is that once the `Service` is set, and it has to be when the `ServiceUser` is created, it can’t be changed. Since the constructor could be passed `nullptr` for the `Service`, we have a decision to make
- Build the class to handle the case when `Service` is passed the `nullptr`. This means many operations will have to check if the pointer is the `nullptr` before trying to work with it.
- Prevent objects from being built with the `nullptr`. This would involve exceptions and users would then have to do exception handling.

Both cases require additional code to be managed just to deal with the single case involving the `nullptr`.

Another way to handle this is to use a reference attribute. A reference attribute is declared much like a regular object, but includes the `&` operator:

```cpp
class ServiceUser {
private:

    Service& service; // <-- reference attribute

public:

    ServiceUser(Service &initService);

    // Other ServiceUser operations - but no setService()

}
```

We would the implement the constructor using a member initialization list,

```cpp
ServiceUser::ServiceUser(Service &initService) 
           : service(initService) {}
```

A feature of reference attributes is that once the attribute is set, it cannot be changed. So, it acts like a constant. Further, the only way to create an instance is to supply an actual `Service` object, so we no longer have to worry about the “null service”.











































