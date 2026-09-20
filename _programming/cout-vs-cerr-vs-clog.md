---
layout: page
title: "cout vs cerr vs clog"
---

You likely have regularly used `cout`, which typically causes something to appear in the terminal. However, that is only part of the story. Technically, `cout` sends output to something called `stdout`. By default, `stdout` routes to the terminal. There are two other streams like `cout` available: `cerr` and `clog`. The former is used for dumping errors and the latter for logging basic events you are interested in tracking. Both `cerr` and `clog` default to `stderr`, which also defaults to the terminal. 

Suppose we have the following program:

```cpp
#include <iostream>

using namepace std;

int main() {

    cout << "Hello, cout" << endl;

    cerr << "Hello, cerr" << endl;

    clog << "Hello, clog" << endl;

    return 0;

}
```

Assuming the executable is named `outputExamples.exe`, running the program in the terminal would result in


```
> outputExamples.exe
Hello, cout
Hello, cerr
Hello, clog
```

Now, `cout`, `cerr`, and `clog`, as they are different streams, can be redirected to other output locations. Typically, redirection means a file, but it could also be to a pipe (i.e., a way to chain programs together by feeding the output of one program into another). In practice, `cout` and `cerr` might go to the terminal, but `clog` might be redirected to file. That is, you definitely want to see output and error messages, but logging might simply be telemetry to be analyzed later.

## Buffered/Unbuffered Streams

There is something different between `cout` and `clog` as compared to `cerr`: `cout` and `clog` are *buffered* whereas `cerr` is *unbuffered*. When you `cout` something, it doesn’t immediately go to where it is supposed to show up. Rather, it goes into a buffer that gets emptied every now and then. You can see the behavior in the following example:

```cpp
int main() {

    cout << "Running: ";

    for (int i=0; i < 10; ++i) {

        cout << ".";

        for (int j=0; j < 1000000000; ++j) {
            ++j;
        }

    }

    cout << endl;

    return 0;

}
```

When you read this you might expect the following behavior:

- "Running: " appears on the console
- Then a "."
- Then there is a pause while the program counts 1,000,000,000
- Then another "."
- And so on.

However, when you run it, it will appear as though the program is not doing anything and then, after it counts to a billion 10 times, the output appears all at once. Try giving it a run.

The way to fix this is to force a buffer flush of `cout` using `flush()`:

```cpp
int main() {

    cout << "Running: ";
    cout.flush(); // <-- key line!

    for (int i=0; i < 10; ++i) {

        cout << ".";
        cout.flush();

        for (int j=0; j < 1000000000; ++j) {
            ++j;
        }

    }

    cout << endl;

    return 0;

}
```

Again, give this a try and see the behavior.

By contrast, when you stream something to cerr, as in

```cpp
cerr << "YIKES!" << endl;
```

the output always immediately appears. Try replacing `cout` with `cerr` in the example where the output was delayed.


## std::endl vs '\n'

You might have noticed that in the progress bar example there is no use of `endl`. This is because we want our progress bar output to appear on the same line. If we wanted a line feed after each dot, the code would look like

```cpp
int main() {

    cout << "Running: " << endl; // <-- notice the endl!

    for (int i=0; i < 10; ++i) {

        cout << "." << endl; // <-- notice the endl!

        for (int j=0; j < 1000000000; ++j) {
            ++j;
        }

    }

    cout << endl;

    return 0;

}
```

Give this a run and notice the output appears as the program progresses. This is because using `endl` causes a buffer flush of `cout`. 

Now, if you have been programming for a while, you might be familiar with the newline character, `'\n'`. This is a special character that tells the stream to do a linefeed (go to the next line). Let's update the example to use `'\n'`:

```cpp
int main() {

    cout << "Running: \n"; // <-- notice the endl!

    for (int i=0; i < 10; ++i) {

        cout << ".\n"; // <-- notice the endl!

        for (int j=0; j < 1000000000; ++j) {
            ++j;
        }

    }

    cout << endl;

    return 0;

}
```

In terms of how output looks, using `'\n'` seems to have the effect as endl - they both cause a line feed. But there is more to the story.

When `cout` is directed to the terminal, you will see the same behavior when using `'\n'` as when using `endl`. That is, `'\n'` will also seem to cause a buffer flush. This has to do with how terminals work. A terminal uses line buffering, which means when the terminal encouters a newline character it opts to flush the buffer. This provides a more robust user experience.

However, if you send output to a file, `'\n'` does not force a buffer flush. The behavior is more like `cout` without the flushing. This is difficult to observe.

### When to Use std::endl vs `'\n'`
Buffer flushing is time consuming. The reason a buffer is used to is better manager resources. So, in practice, if you want a new line but don’t care about flushing the buffer, `'\n'` is better. This allows the system to manage itself. When you do need a buffer flush, use `cout.flush()` (or alternatively, `cout << flush`) or `endl`, as appropriate.












