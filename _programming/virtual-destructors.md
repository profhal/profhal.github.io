---
layout: page
title: "Virtual Destructors"
---

Declaring destructors `virtual` is important in C++ when using inheritance. The reason we declare operations to be `virtual` is that when they are combined with pointers we can achieve polymorphism.

When we point to an object and invoke an operation, most often we want the object’s behavior. Let’s look at an example. We consider two classes `Student` and `GradStudent`. `Student` simply maintains a name. `GradStudent` is a subclass of `Student` and maintains a research topic. In code, we would have

```cpp
class Student {
private:

    std::string name;

public:  

    Student(const std::string &name) 
        : name(name) {}

    virtual std::string getName() const {
        return name;
    }

    virtual std::string toString() const {
        return “I am a student named “ + name;
    }

};

class GradStudent : public Student {
private:

    std::string researchTopic;

public:  

    GradStudent(const std::string &name) 
        : Student(name), researchTopic("undeclared") {}

    virtual void setResearchTopic(const std::string &topic) {
        researchTopic = topic;
    }

    virtual std::string toString() const {
        return “I am a grad student named “ + getName() 
               + “ researching “ + researchTopic;
    }

};
```

We then can direct a `Student` pointer to reference a `GradStudent` object. Since we declared `toString()` as virtual in `Student`, when we invoke `toString()` through the `Student` pointer, we will get `GradStudent` behavior. Thats is, if we execute

```cpp
GradStudent tom("Tom");

tom.setResearchTopic("Organic Programming");

Student *someStudent = &tom;

std::cout << someStudent->toString() << std::endl;
```

the output would be the result of `GradSchool`‘s `toString()` method executing,

```
I am a grad student named Tom researching Organic Programming
```

Had we not declared `toString()` as `virtual` in `Student`, the output would have been the result of `Student`‘s `toString()` method executing. It comes down to *binding*.

## Static vs Dynamic Binding
There is a distinction between an operation and a method. An *operation* is what is available to call. So, `Student` has an operation named `toString()`. `Student` also implements `toString()`. That is the *method*. `GradStudent` also has the operation `toString()` and it has it’s own method since we overrode the inherited behavior.

Wen we have a pointer to a subclass an invoke an operation, such as

```cpp
GradStudent tom("Tom");

tom.setResearchTopic("Organic Programming");

Student *someStudent = &tom;

std::cout << someStudent->toString() << std::endl;
```

a decision has to be made: which `toString()` method should execute?

When an operation is *not* declared `virtual` in a base class, the binding of the operation to a method happens at compile time. This is called *static binding*. Since the compiler can’t discern what type of object will be referenced (in general, it’s not known until the runtime), the only option is to bind the `toString()` call to `Student`‘s `toString()` method.

If we declare an operation as `virtual`, the decision is delayed until runtime. This is called *dynamic binding*. Since the runtime can determine the type of the object being referenced, it binds to that object’s method. In our case, since `someStudent` references a `GradStudent`, and `toString()` is virtual, the call to `toString()` will be bound to `GradStudent`‘s `toString()` method. That was what we saw earlier.

## Virtual Destructors
In general, we want destructors to be `virtual`. When we need to delete an instance of a subclass through a pointer to a base class, we want the subclass’s destructor to execute. If we don’t declare a destructor as `virtual`, the deletion will only execute the base class destructor. This means any clean up that needs to happen in the subclass will not occur.

Let’s look at a simple example to see the mechanics of what is happening. Let’s start with two unrelated classes `X` and `Y`, 

```cpp
class X {
private:
    int *someData;
public:
    X() {
        someData = new int[100];
    }

    ~X() {
        delete [] someData;
    }

};


class Y {

private:
    int *someOtherData;
public:
    Y() {
        someOtherData = new int[200];
    }

    ~Y() {
        delete [] someOtherData;
    }

};
```

Both classes manage their data correctly - the key is the deletion of the allocated arrays in the destructors. 

Now, suppose we decide these classes should be related by inheritance. Specifically,

```cpp
class Y : public X {  // Y is now a subclass of X
private:

    int *someOtherData;

public:

    Y() {
        someOtherData = new int[200];
    }

    ~Y() {
        delete [] someOtherData;
    }

};
```

On the surface, it seems like this should work. It will when using object variables. However, when leveraging polymorphism through pointers, we have a problem. Consider the following,

```cpp
X *x = new Y();

delete x;
```

The result of the first line is that we have two arrays in memory (`someData` referencing 100 elements and `someOtherData` referencing 200 elements). Being good stewards, we cleaned up when we were done and deletd the allocated `Y` object referenced by `x`.

Since we did not declare the destructor in `X` to be `virtual`, `X`‘s destructor method is statically bound to the `delete` call. This means `X`‘s destructor will execute and clean up `someData`, but `Y`‘s destructor does not execute. The result is a memory leak. To solve this, we want to be in the habit of declaring destructors as `virtual`. 

Updating our example, we now have,

```cpp
class X {
private:

    int *someData;

public:

    X() {
        someData = new int[100];
    }

    virtual ~X() {  // X's destructor is now virutal
        delete [] someData;
    }

};


class Y : public X {
private:

    int *someOtherData;

public:

    Y() {
        someOtherData = new int[200];
    }

    virtual ~Y() {  // Y's destructor is now virutal
        delete [] someOtherData;
    }

};
```

When write

```cpp
X *x = new Y();

delete x;
```

the binding will be dynamic and the call will be bound to `Y`‘s destructor method. Keep in mind that when classes are related through inheritance, invoking the destructor of a subclass will in turn invoke the base class’s destructor. In our example, after `Y`‘s destructor runs, `X`‘s destructor is invoked. All of the allocated memory is returned to the system.

















