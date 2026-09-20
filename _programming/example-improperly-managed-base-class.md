---
layout: page
title: "Example: Improperly Managed Base Class"
---

Making attributes of a base class protected could lead to problems in a subclass. Consider the classes `Vehicle` and `Car`. Suppose `Vehicle` is defined as

```cpp
class Vehicle {

protected:

    // This is the vehicle id number. It must be a string 
    // of 10 digits or "UNKNOWN"
    //

    string vin;

    bool isValidVIN(string vin) {

        // returns true if vin is a string of 10 digits
        // and false otherwise

    }

public:

    Vehicle(string initVin) {

        vin = (isValidVIN(initVin) ? initVIN : "UNKNOWN";

    }

    // Other great Vehicle stuff

};
```

The point is `Vehicle` always manages `vin` so that is either a `string` of 10 digits or `"UNKNOWN"`. This enables us to detect unknown VINs easily.

Now, suppose we subclass `Vehicle` by `Car` with the following operation, `setVIN()`:

```cpp
class Car : public Vehicle {

public:

    void setVIN(string newVIN) {

        vin = newVIN;

    }

    // Other great Car stuff

};
```
Here, `Car` just sets the vin without any checks. So, `vin` could be updated to something it shouldn’t (i.e., other than 10 digits or `"UNKNOWN"`). This could lead to problems later.

