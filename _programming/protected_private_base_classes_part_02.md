---
layout: page
title: "Protected/Private Base Classes - Part 2"
---

This is a continuation of the discussion, [Protected/Private Base Classes - Part 1]({% link _programming/protected_private_base_classes_part_01.md %}), where we asserted that using protected or private inheritance to leverage behavior of class is not a good practice. Rather, we argued for composition (having an attribute or an object local to a method).

The context of Part 1 did not include polymorphism. So, the interaction was only one direction. The subclass, or composing class, interacted with features of the base class, but the base class didn’t leverage anything about the subclass. Now, let’s introduce polymorphism. In the example, the base class will use its pure virtual operation in the implementation of its concrete operation.

* **Note:** The example we explore doesn’t require subclassing. We could refactor the design so that it’s not needed. What it does is illustrate a situation where it could be leveraged. Again, as discussed in Part 1, we normally prioritize composition over inheritance. However, there are two advanced cases where private subclassing might be preferable and both are discussed below: Empty Base Class Optimization and Initialization Ordering.*

This example evolves the `Logger` and `NetworkManager` example from [Part 1]({% link _programming/protected_private_base_classes_part_01.md %}). In this case, `Logger` will now implement a `log()` operation that needs to include the state of the class making the call. To do this, `Logger::log()` depends on `Logger::getClassState()`.

```cpp
class Logger {
private: 

    string filepath;

protected:

    virtual string getClassState() const = 0;

public:

    Logger(string initFilepath) {

        filepath = initFilepath;

    }

    virtual void log(string message) const {

        ofstream file;

        // Open file for appending.
        file.open(filepath, ios_base::app);

        file << message 
             << "\n"
             << "   " << getClassState() 
             << std::endl;

        file.close();

    }
    
};
```

Notice that `Logger::getClassState()` is pure virtual. This means, for one, that `Logger` cannot be instantiated directly and in turn can’t be used in a composition in this design. If another class wants to leverage `Logger`, it must be a subclass and implement `getClassState()`. This is what `NetworkManager` does.

```cpp
class NetworkManager : private Logger {
private:

    bool isConnected;

    int eventCount;

protected:

    virtual string getClassState() const {

        return "NetworkManager : Connected? " 
               + (isConnected ? string("Yes") : string("No"))
               + " || "
               + "Event count: " + std::to_string(eventCount);

    }

public:

    NetworkManager(string logFilepath) : Logger(logFilepath) {

        isConnected = false;

        eventCount = 0;

    }

    virtual void start() {

        isConnected = true;

    }

    virtual void stop() {

        isConnected = false;

    }

    virtual void logNetworkEvent(string event) {
    
        ++eventCount;

        log("Network Event: " + event);

    }

};
```

The fact that `Logger` is a `private` base class means other classes can’t leverage `NetworkManager` as a `Logger`. Logging through the `Logger` only happens through the `NetworkManager`.

Now, when `NetworkManager::logNetworkEvent()` is called `Logger::log()` is called, and in turn invokes `NetworkManager::getClassState()` via polymorphism. That is, in `Logger`‘s `log()` implementation,

```cpp
virtual void log(string message) const {

    ofstream file;

    // Open file for appending.
    file.open(filepath, ios_base::app);

    file << message 
         << "\n"
         << "   " << getClassState() 
         << std::endl;

    file.close();

}
```

the call to `getClassState()` gets bound to `NetworkManager::getClassState()`‘s method. This means if we had a `main()` such as

int main() {

    NetworkManager nm("networkLog.txt");

    nm.logNetworkEvent("Attempting to start network...");

    nm.start();

    nm.logNetworkEvent("Network started successfully.");

    nm.logNetworkEvent("Some random update.");

    nm.stop();

    nm.logNetworkEvent("Network successfull shut down.");


    return 0;

}
```

executing the program would result in the file `"networkLog.txt"` with the following contents

```
Network Event: Attempting to start network...
   NetworkManager : Connected? No || Event count: 1
Network Event: Network started successfully.
   NetworkManager : Connected? Yes || Event count: 2
Network Event: Some random update.
   NetworkManager : Connected? Yes || Event count: 3
Network Event: Network successfull shut down.
   NetworkManager : Connected? No || Event count: 4
```

You can see that the class state that is reported in from `NetworkManager`.

