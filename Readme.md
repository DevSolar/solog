# SoLog

*Some Ordinary Logging* / *Solar's Logger*

A lightweight, single-header (STB-style) C/C++ logging library.


## Features

* **Header-only (`stb`-style):** No build configuration or compilation steps required.
* **Atomic output:** Single `vfprintf()` call per log entry.
* **Short-circuiting:** Arguments are not evaluated if the log level is disabled.
* **Configurable at runtime:** Adjust minimum log level and destination stream on the fly.
* **C & C++ compatible:** Works cleanly with standard C99/C11 and C++11+.
* Check SOLOG_H for versioning (future versions will increment its value).
* Compiles without warnings even under the strictest settings (false positives suppressed).


## Usage

In **every** source file needing logging:
```c
#include "solog.h"
```

In **exactly one** translation unit (`.c` or `.cpp` file), define `SOLOG_IMPLEMENTATION` before including:
```c
#define SOLOG_IMPLEMENTATION
#include "solog.h"

int main(void) {
    /* Optional runtime configuration */
    solog_config.level  = SOLOG_LVL_DEBUG; /* Default: SOLOG_LVL_INFO */
    solog_config.stream = stdout;          /* Default: stderr */

    /* Generic macro (levels: TRACE, DEBUG, INFO, WARN, ERR, FAIL) */
    SOLOG(INFO, "Application started: %d", 42);

    return 0;
}
```


## License

Dedicated to the public domain under the [CC0 1.0 Universal License](https://creativecommons.org/publicdomain/zero/1.0/).
