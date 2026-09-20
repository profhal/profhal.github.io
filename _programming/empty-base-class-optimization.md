---
layout: page
title: "Empty Base Class Optimization"
---

There are times when we create classes that don’t have any internals and might simply look like

```cpp
class SomeClass {};
```

Such entities are named types and have various purposes. Here are a couple examples:

**Example:** Exception classes don’t necessarily have internals. The classes give rise to types which can be used in catch blocks for refined error handling.

**Example:** A top-level class in a hierarchy might be empty and only exist to provide a common parent for family of classes.

Per C++, an empty class can be a little sturdier. Specifically, a class is empty if it has

- no non-static data members
- no virtual functions
- no virtual base classes

When a C++ class is empty, it can be optimized into the storage of the subclass, effectively saving memory which leads to potential increased cache utilization and efficient object creation/destruction.

We can see this happening by creating a set of classes

- `NonemptyClass` : a nonempty class with no base class
- `EmptyBase` : an empty base class
- `NonemptySubClass` : a nonempty subclasss of `EmptyBase`
- `Base` : a nonempty base class
- `SubClass` : a nonempty subclass of Base

and then outputting how many bytes each takes.

Let's start with `NonEmptyClass`:

```cpp
class NonemptyClass {
private:    

    int data;

public:

    NonemptyClass(int initData) {
        data = initData;
    }

    int getData() const {
        return data;
    }

    void setData(int newData) {
        data = newData;
    }

};
```

and then write a `main()` to output its size:

```cpp
int main() {

    std::cout << “NonemptyClass size: “ << sizeof(NonemptyClass) << std::endl;

    return 0;

}
```

The result would be

```
> ./a.out
NonemptyClass size: 4 bytes
```

Now, consider the nonempty class, `Base`, and derive the class `SubClass`:

```cpp

class Base {
private:
    int data;
public:
    Base(int initData) {
        data = initData;
    }

    int getData() const {
        return data;
    }

    void setData(int newData) {
        data = newData;
    }
};



class SubClass : public Base {
private:    

    int data;

public:

    SubClass(int initData) : Base(initData) {
        data = initData;
    }

    int getData() const {
        return data;
    }

    void setData(int newData) {
        data = newData;
    }

};
```

As with `NonemptyClass`, we write a `main()` that outputs the size of each. The result would be

```
> ./a.out
Base size : 4 bytes
SubClass size: 8 bytes
```

This shouldn’t be surprising. `Base` looked like `NonemptyClass` and should have the same size. `SubClass` also looked like `Base` in terms of structure and it was derived from `Base` - so it required 4 (for Base) + 4 (for SubClass) = 8 bytes.


Lastly, let’s look at the empty base case situation. We start with a class, `EmptyBase`, with no attributes but a non-virtual operation,

```cpp
class EmptyBase {
public:
    void doSomething() const {
        int i = 42; // Just a placeholder action
        ++i;
    }
};
```

and we subclass with a non-empty class,

```cpp
class NonemptySubClass : public EmptyBase {
private:    

    int data;

public:

    NonemptySubClass(int initData) {
        data = initData;
    }

    int getData() const {
        return data;
    }

    void setData(int newData) {
        data = newData;
    }

};
```

Now, when we run a program that outputs the size, we get

```
> ./a.out
EmptyBase size : 1 byte
NonemptySubClass size: 4 bytes
```

The class `EmptyBase`, while empty, can still be instantiated. Though it stores no data, it still has identity, which is stored in its 1 byte. The class `NonemptySubClass`, which looks like the class `NonemptyClass` but is also subclass of `EmptyBase`, also consumes 4 bytes.

**The Upshot:** `NonemptyClass` had no base class and took 4 bytes and `NonemptySubClass` had a base class and also only used 4 bytes. So, the `EmptyBase` part of `NonemptySubClass` is folded into its underlying storage.


## Impact of Composition

Lastly, let’s look at what happens when we use composition with an empty class. Notice that `ComposedClass` has an attribute of type `EmptyBase`.

```cpp
class ComposedClass {
private:
    EmptyBase emptyBaseMember;
    int data;
public:
    ComposedClass(int initData) {
        data = initData;
    }

    int getData() const {
        return data;
    }

    void setData(int newData) {
        data = newData;
    }
};
```

When we output the respective sizes, we get

```
> ./a.out
EmptyBase size : 1 byte
ComposedClass size: 8 bytes
```

Recalling that when using inheritance, the subclass was only 4 bytes, we see that composition results in a total of 8 bytes.









