---
layout: page
title: "Initialization Ordering"
---

A benefit of inheriting from a base class is that the base class will be initialized before anything in the subclass. So, if the subclass has attributes that depend on the base class, it’s guaranteed that the base class is ready for use.

Consider the following example: We have `Logger` which manages a log file. We create the subclass, `Service`, which wants to do some logging while initializing. However, the `Service` doesn’t want to present itself as a `Logger`. We can utilize a private base class to handle this.

Let's start by defining `Logger`,

```cpp
class Logger {
private:

    ofile outputFile;

public:

    Logger(string fp) : outputFile (fp) {
        
        fp << “[Logger] Logger initialized.\n";
    
    }

    void log(const string& msg) const {

        fp << “[Logger] “ << msg << ‘\n’;

    }

};
```

and then define `Service`, which utilizes `Logger` during construction,

```cpp
class Service : private Logger {
private:
    string config;
    int connection;

    string loadConfig() { 
        log("Loading configuration..."); 
        return “the config data"; // imagine its a JSON string
    }

    int connect() { 
        log("Establishing connection..."); 
        return 1; // think socket handle
    } 

public:

    Service(string logfilepath)
        : Logger(logfilepath), 
          config(loadConfig()),   // depends on Logger::log()
          connection(connect())   // depends on Logger::log()
    {
        log("Service fully constructed.");
    }

};
```

Things to observe:

- Logger is the base class and is initialized first.
- The attributes of Service, config and connection, are being initialized through calls to loadConfig() and connect(), respectively.
- The operations loadConfig() and connect() both depend on Logger::log(). So, the Logger part of Service needs to be in place before the other initialization happens.

Now, when you create an instance of `Service`, 

```cpp
Service s;
```

the following output would appear,

```
[Logger] Logger initialized.
[Logger] Loading configuration...
[Logger] Establishing connection...
[Logger] Service fully constructed.
```

The next question might be, *Can't we do this with composition?* The answer is "yes", but the solution becomes fragile. 

We begin with the same `Logger`, but now implement `Service` via composition,

```cpp
class Service {
private:
    Logger logger;
    string config;
    int connection;

    string loadConfig() { 
        logger.log("Loading configuration..."); 
        return “the config data as a string"; }

    int connect() { 
        logger.log("Establishing connection..."); 
        return 1; // just a connect id, like a socket handle
    } 

public:

    Service(string logfilepath)
        : logger(logfilepath), 
          config(loadConfig()),   // depends on Logger::log()
          connection(connect())   // depends on Logger::log()
    {
        logger.log("Service fully constructed.");
    }

};
```

Creating an instance of `Service` works here solely because of the fact that `logger` is the first attribute to be listed *in the class declaration*. The initialization list on the constructor implementation has no effect. We could also have done

```cpp
class Service {
private:
    Logger logger;
    string config;
    int connection;

    string loadConfig() { 
        logger.log("Loading configuration..."); 
        return “the config data as a string"; }

    int connect() { 
        logger.log("Establishing connection..."); 
        return 1; // just a connect id, like a socket handle
    } 

public:

    Service(string logfilepath)
        : config(loadConfig()),   // depends on Logger::log()
          connection(connect()),   // depends on Logger::log()
          logger(logfilepath)
    {
        logger.log("Service fully constructed.");
    }

};
```

where `logger` is now listed last in `Service`‘s constructor initialization list. Things would still work.

Now, let's change that order in the declaration of the attributes of `Service`, moving `logger` to be declared last, and we will also revert `Service`‘s constructor so that `logger` is listed first:

```cpp
class Service {
private:
    string config;
    int connection;
    Logger logger; // logger is listed third

    string loadConfig() { 
        logger.log("Loading configuration..."); 
        return “the config data as a string"; }

    int connect() { 
        logger.log("Establishing connection..."); 
        return 1; // just a connect id, like a socket handle
    } 

public:

    Service(string logfilepath)
        : logger(logfilepath),    // logger is listed first
          config(loadConfig()),   // depends on Logger::log()
          connection(connect())   // depends on Logger::log()
          
    {
        logger.log("Service fully constructed.");
    }

};
```

This will compile but will result in a program crash[^1]. This is because `logger` is listed after `config` and `connection`. Even though it looks like `logger` is getting initialized first, it isn’t. This means that, in this case, composition requires a specific ordering of the attributes when they are declared. This is the approach is considered fragile.

[^1]: Technically, the answer is it results in undefined behavior. Often, though, the behavior is to crash.



