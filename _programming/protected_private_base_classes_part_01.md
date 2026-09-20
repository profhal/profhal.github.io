---
layout: page
title: "Protected/Private Base Classes - Part 1"
---

When creating a subclass in C++, most of the time the base class is declared to be `public`. For example, we might subclass `Vehicle` by `Car` and would have 

```cpp
class Vehicle { ... };

class Car : public Vehicle { ... };
```

However a base class can also be declared `protected` or `private`. In this discussion, part 1 of 2, we are focusing on inheritance. We will discuss polymorphsim in [Part 2]({% link _programming/protected_private_base_classes_part_02.md %}).

The question becomes, "Why would you declare a base class as something other than public?" The standard answer is, *You want to leverage the functionality of the base case but not expose it*. Consider the following example: 

```cpp
class Logger {
public:

    void logMessage(const string &message) {
        cout << "[LOG] " << message << endl;
    }
    
    void logError(const string &error) {
       cerr << "[ERROR] " << error << endl;
    }
};
  
  
// NOTE: We are using private inheritance!
//
class NetworkManager : private Logger { 
public:

    void connect(const string &address) {
        
        logMessage("Attempting to connect to " + address);
        
        // ... connection logic ...

        if (connectionFailed) {

            logError("Failed to connect to " + address);

        } else {
            
            logMessage("Successfully connected to " + address);
        
        }
    
    }

    void sendData(const string &data) {

        logMessage("Sending data: " + data);

        // ... data sending logic ...
    
    }

};

int main() {
         
    NetworkManager manager;

    manager.connect("192.168.1.1");
    manager.sendData("Hello Server");

    return 0;

}
```

`NetworkManager` needs to utilize the functionality of `Logger` and gains access to it via inheritance. While this works, it’s not the best approach. In general, you should favor composition over inheritance. That means we create an attribute (or a object local to a method when needed) as opposed to being a derived class. Being a derived class - "is a type of" - is a stronger relationship than just using a class.

If we use composition, the previous example might become

```cpp
class Logger {
public:

    void logMessage(const string &message) {
        cout << "[LOG] " << message << endl;
    }
    
    void logError(const string &error) {
       cerr << "[ERROR] " << error << endl;
    }
};

                                      
class NetworkManager {
private:
    
    // Composition: The network manage encapsulates an Logger object.
    //
    Logger logger;
 
public:

    void connect(const string &address) {
        
        logger.logMessage("Attempting to connect to " + address);
        
        // ... connection logic ...

        if (connectionFailed) {

            logger.logError("Failed to connect to " + address);

        } else {
            
            logger.logMessage("Successfully connected to " 
                               + address);
        
        }
    
    }

    void sendData(const string &data) {

        logger.logMessage("Sending data: " + data);

        // ... data sending logic ...
    
    }

};

int main() {
         
    NetworkManager manager;

    manager.connect("192.168.1.1");
    manager.sendData("Hello Server");

    return 0;

}
```

The thrust here is that calls to Logger functionality go through the logger object.

The behavior of the two approaches are identical. However, when a super class has `protected` features, a subclass can interact with them. This means `protected` attributes might not be managed correctly by subclasses (see [Improperly Managed Base Class]({% link _programming/example-improperly-managed-base-class.md %})). In this example, it wouldn’t be a problem as `Logger` has no internals to manage. This example just illustrates subclassing versus composition.

## Looking Ahead to Part 2
Once we introduce polymorphism, there is a rationale for using subclasses. It comes down to the need for a superclass to invoke a behavior of a subclass. Check out [Part 2]({% link _programming/protected_private_base_classes_part_02.md %}).