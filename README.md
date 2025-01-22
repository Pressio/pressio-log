# pressio-log
Logging functionality for Pressio repositories

This is a work in progress, and all subsequent information is subject to change.

## Instructions for Use:

1. Clone the repo

```sh
git clone https://github.com/Pressio/pressio-log.git
```

2. Set logging level in CMake and build tests

```sh
cd pressio-log
mkdir build
cd build
cmake -D PRESSIO_LOG_LEVEL=DEBUG .. && make
```

All logging levels:

- 0 - "None"
- 1 - "Basic"
- 2 - "Info"
- 3 - "Debug"

You can pass either the number or the corresponding string as the argument.

3. Include the library

Point to `pressio-log/include` to include the relevant files.

4. Include the core file

```cpp
#include <pressio-log/core.hpp>
```

5. Use the library

All logging is handled via macros:

```cpp
PRESSIOLOG_BASIC("message");
PRESSIOLOG_INFO("message");
PRESSIOLOG_DEBUG("message");
PRESSIOLOG_WARNING("message");
PRESSIOLOG_ERROR("message");
```

Errors will print regardless of the logging level.
Warnings will print for any level above "none".

## Testing

After you have built (see step 2), run:

```sh
cd pressio-log/build/tests/logging
ctest
```
