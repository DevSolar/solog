# SoLog

*Some Ordinary Logging* / *Solar's Logger*

A lightweight, single-header (STB-style) C/C++ logging library.

All you need is solog.h -- all the rest is either advertising (this Readme)
or test code.


## Features

* **Header-only (`stb`-style):** No build configuration or compilation steps.
* **Atomic output:** Single `vfprintf()` call per log entry.
* **Short-circuiting:** Arguments are not evaluated if log level not met.
* **Configurable at runtime:** Adjust minimum log level and destination stream
  on the fly.
* **C & C++ compatible:** Works cleanly with standard C99/C11 and C++11+.
* **SOLOG_H versioning:** Include guard SOLOG_H defined to version number
* **Warning-free:** Even at strictest warning settings. A few false-positive
  warnings suppressed.


## Usage

In **every** source file needing logging:
```c
#include "solog.h"
```

In **exactly one** translation unit, define `SOLOG_IMPLEMENTATION` before
including:

```c
#define SOLOG_IMPLEMENTATION
#include "solog.h"

int main(void) {
    /* Optional runtime configuration */
    solog_config.level  = SOLOG_LVL_DEBUG; /* Default: SOLOG_LVL_INFO */
    solog_config.stream = stdout;          /* Default: stderr */

    /* Generic macro (levels: TRACE, DEBUG, INFO, WARN, ERR, FAIL) */
    SOLOG(INFO, "Application started with SoLog v%d", SOLOG_H);

    return 0;
}
```


## Feature Flags

Anything beyond the most basic functionality is optionally enabled through
the value you set SOLOG_IMPLEMENTATION to. Every feature macro is defined
to a bit mask. OR-ing them together, you can compile-time enable some, all,
or none of them. By AND-ing them out of solog_config.features, you can run-
time toggle them.

This is the result of running `./output`:

![Screenshot](/screenshot.png?raw=true)


## License

Dedicated to the public domain under the
[CC0 1.0 Universal License](https://creativecommons.org/publicdomain/zero/1.0/).
