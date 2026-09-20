---
layout: page
title: "Inaccuracy of double Values"
---

Representing floating point values in computer systems is problematic at best. Most languages conform to the IEEE specification called [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754). While we don’t have to be concerned with the details of the specification in order to use floating point values, we do have to understand their limitations.

Hopefully, it’s understood that types in languages utilize a finite number of bits to store values. For integers, this can lead to some interesting behavior involving [overflow](https://en.wikipedia.org/wiki/Integer_overflow) and underflow (discussed in the same link) when you reach the limits of the representation.

For floating point values, the issue is accuracy: 32-bit floating point values (e.g., typically float in C++ or Java) have about 6 digits of accuracy whereas 64-bit floating point values (e.g., typically double in C++ and Java) have about 15. Anything after the guaranteed accuracy is garbage, but it can have an influence. 

For example, try running the following C++ program:

```cpp
int main() {

    double value = 10.0;

    cout << fixed << setprecision(19);

    cout << value << endl;

    value -= 0.2;

    cout << value << endl;

    value += 0.4;

    cout << value << endl;

    value -= 0.2;

    cout << value << endl;

    cout << (10.0 == value ? "true" : "false") << endl;

    return 0;

}
```

You might expect to see

```
10.0000000000000000000  <-- initial value
9.8000000000000000000   <-- value after subtracting 0.2
10.2000000000000000000  <-- value after adding 0.4
10.0000000000000000000  <-- value after subtracting 0.2
true                    <-- does value = 10.0?
```

but the actual output will be

```
10.0000000000000000000  <-- initial value
9.8000000000000007105   <-- value after subtracting 0.2
10.2000000000000010658  <-- value after adding 0.4
10.0000000000000017764  <-- value after subtracting 0.2
false                   <-- does value = 10.0?
```

To emphasize, that last line is implying $10 - 0.2 + 0.4 - 0.2 \ne 10$.

A little wisdom:

- Don’t use doubles when what you are representing is integer values
- Don’t compare doubles for exact values, rather use a threshold - a tiny value that is close enough to zero to account for the junk.




