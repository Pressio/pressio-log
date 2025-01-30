# pressio-log
Logging functionality for Pressio repositories

_This is a work in progress, and all subsequent information is subject to change._

## Instructions for Use:

1. **Clone** the repo

```sh
git clone https://github.com/Pressio/pressio-log.git
```

2. **Configure** the logger via CMake

| CMake Variable                       | Default Value |
| :----------------------------------- | ------------: |
| `-D PRESSIO_ENABLE_LOGGING`          |           `ON`|
| `-D PRESSIO_SILENCE_WARNINGS`        |          `OFF`|
| `-D PRESSIO_ENABLE_TPL_MPI`          |          `OFF`|
| `-D PRESSIO_ENABLE_COLORIZED_OUTPUT` |          `OFF`|

Sample build command:

```sh
cd pressio-log
mkdir build
cd build
cmake -D PRESSIO_ENABLE_TPL_MPI=ON .. && make
```

3. **Include** the library

Add `pressio-log/include` to your project's include directories, and include the core file:

```cpp
#include <pressio-log/core.hpp>
```

4. **Initialize** the logger

The `PressioLogger` must be initialized before it can be used:

```cpp
PRESSIOLOG_INITIALIZE();
```

The initialization function takes several optional arguments. The arguments, and their default values, are provided below:

```cpp
PRESSIOLOG_INITIALIZE(
    pressiolog::LogLevel level = basic,
    pressiolog::LogTo dst      = console,
    std::string logfileName    = "pressio.log",
    int loggingRank            = 0,              // only when PRESSIO_ENABLE_TPL_MPI=ON
    MPI_Comm comm              = MPI_COMM_WORLD  // only when PRESSIO_ENABLE_TPL_MPI=ON
)
```

5. **Use** the library

All logging is handled via macros:

```cpp
PRESSIOLOG_BASIC("message");
PRESSIOLOG_INFO("message");
PRESSIOLOG_DEBUG("message");
PRESSIOLOG_WARNING("message");
PRESSIOLOG_ERROR("message");
```

Warnings and errors will print at the `info` and `debug` logging levels.

All of the initialization parameters can also be overriden via macros:

```cpp
PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel level);     // none, basic, info, debug
PRESSIOLOG_SET_OUTPUT_STREAM(pressiolog::LogTo dst);  // console, file, both
PRESSIOLOG_SET_OUTPUT_FILENAME(std::string filename);
PRESSIOLOG_SET_LOGGING_RANK(int rank);
PRESSIOLOG_SET_COMMUNICATOR(MPI_Comm comm);
```

6. **Finalize** the logger

At the end of your program, finalize the logger with

```cpp
PRESSIOLOGGER_FINALIZE();
```

## Sample Program

```cpp
#include <pressio-log/core.hpp>

int main() {
    PRESSIOLOG_INITIALIZE(pressiolog::LogLevel::debug);
    PRESSIOLOG_INFO("Program information: ");
    bool success;
    for (int i; i < 10; i++) {
        PRESSIOLOG_DEBUG("Iteration " + std::to_string(i))
        success = i == 9 ? true : false;
    }
    if (success) {
        PRESSIOLOG_INFO("Process completed successfully");
    }
    PRESSIOLOG_FINALIZE();
    return 0;
}

```

## Testing

After you have built the tests (see step 2), run:

```sh
cd pressio-log/build/tests/logging
ctest -j <np>
```

> [!NOTE]
> All MPI tests require at least three ranks to run.
