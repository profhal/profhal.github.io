---
layout: page
title: "Unions"
---

Structured data is commonly used in programming languages. It enables us to model real-world concepts effectively by gathering various pieces of information into a common container. In C++, one option is to use a `struct`. For example, we might define a `Image` (`pixels` is a pointer to an array of length `width` * `height`) as

```cpp
struct Image {
    int  * pixels;
    int    height;
    int    width;
};
```

When you create an instance on a 64-bit machine (pointers are 64-bit addresses whereas an `int` is often 32-bits), in memory it would be laid out as in FIgure 1.

<figure class="numbered">
  <img src="/assets/images/programming/imageInMemory.png"
       alt="The memory layout of the struct Image.">
  <figcaption>
    The memory layout of the `struct` `Image`.
  </figcaption>
</figure>

What we notice is each field has it’s own memory space. In fact, you can see this by displaying the addresses of each field. For example, running

```cpp
int main() {

    Image i;

    std::cout << "Address of pixels: " << &i.pixels << std::endl;
    std::cout << "Address of height: " << &i.height << std::endl;
    std::cout << "Address of width : " << &i.width  << std::endl;


    return 0;

}
```

the output would be

```
> ./a.out        
Address of pixels: 0x7ff7bb99ee08
Address of width : 0x7ff7bb99ee10
Address of height: 0x7ff7bb99ee14
```

You can see that the space between `pixels` and `width` is 8 bytes (in hexadecimal 0x08 + 0x08 = 0x10) and there is 4 bytes between `width` and `height`.

A `union`, by contrast, places all fields on top of each other and the memory it requires is the memory required by the largest field. Suppose we have the following `union`,

```cpp
union PaySource {
    int    hours;
    double sales;
};
```

If we create an instance of `PaySource`, where `hours` is a 32-bit `int` and `sales` is a 64-bit `double`, in memory it would be laid out as in Figure 2.

<figure class="numbered">
  <img src="/assets/images/programming/paysourceInMemory.png"
       alt="The memory layout of the union PaySource.">
  <figcaption>
    The memory layout of the `union` `Paysource`.
  </figcaption>
</figure>

In this case, `sales` and `hours` are mapped to the same address. Since `sales` is a `double`, which is 64 bits, and `hours` is an an `int`, which is 32 bits, to store any field, the `union` requires 64 bits. This means, in memory, an instance will be allocated 64 bits.

We can see that the two fields are overlapped by looking at their addresses. Running the program

```cpp
int main() {

    PaySource ps;

    std::cout << "Address of hours : " << &ps.hours << std::endl;
    std::cout << "Address of sales : " << &ps.sales << std::endl;


    return 0;

}
```

would show that both fields live at the same address:

```
> ./a.out        
Address of hours : 0x7ff7beddae18
Address of sales : 0x7ff7beddae18
```

When you interact with a `union`, you can access any field and the memory will be manipulated based on the type of the field. In this example, if we do

```cpp
PaySource p;

p.sales = 12503.87
```

all 64 bits would be utilized to store the value since `sales` is a `double`. On the otherhand,

```cpp
p.hours = 35;
```

involves only the first 32-bits.

The issue is if we use one field to update the `union` and the other field to read it. Remember that the way `int` values are stored and manipulated is conisderably different than the way `double` values are stored and manipulated. If you run the following program,

```cpp
#include <iostream>

using namespace std;

union PaySource {
    double sales;
    int hours;
};

int main() {

    PaySource p;

    p.sales = 1;

    cout << p.sales << endl;
    cout << p.hours << endl;

    return 0;

}
```

You will likely see

```
> ./a.out
1
0
```

The point is that the `int` and `double` live at the same location and have different representations. You must be consistent when using a union.












