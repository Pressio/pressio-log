# pressio-log
Header-only logging library for Pressio repositories

## Logging Levels

There are six logging levels within the `LogLevel` enum:

- `none`
- `sparse`
- `error`
- `warning`
- `info`
- `debug`

The level at which the logger runs is determined by the user during initialization (see [Instructions For Use](instructions-for-use)).

The logger will log all messages **including and above** the specified logging level.

For example, if the logger is set to `pressiolog::LogLevel::info`, all info, warning, error, and sparse messages will be logged.

At the `debug` level, all messages are logged. At `none`, no messages are logged.

### Customized Logging

The logging macros can be customized by the application.

For example, an application using pressio-log can define a custom logging macro that forwards to `PRESSIOLOG_SPARSE()`:

```cpp
#include <pressio-log/core.h>

#define CUSTOM_LOGGING_MACRO(...) PRESSIOLOG_SPARSE(__VA_ARGS__)
```

## Instructions for Use:

1. **Clone** the repo

```sh
git clone https://github.com/Pressio/pressio-log.git
```

2. **Define** any macros

You can define any of the following macros before including the core pressio-log file:

```cpp
#define PRESSIO_SILENCE_WARNINGS 1
#define PRESSIO_ENABLE_COLORIZED_OUTPUT 1
#define PRESSIO_ENABLE_EXTERNAL_FMT 1
#define PRESSIO_ENABLE_TPL_MPI 1
```

If you enable an external fmt, make sure that your project properly includes the `fmt` library you would like to use.

By default, the above options are all turned off, and pressio-log uses the in-house `fmt` snapshotted in `include/fmt`.

> [!NOTE]
> If you are configuring pressio-log using CMake, the above options can be passed as CMake variables (e.g. `-D PRESSIO_SILENCE_WARNINGS=ON`).

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
    pressiolog::LogLevel level = sparse,
    pressiolog::LogTo dst      = console,
    std::string logfileName    = "pressio.log",
    int loggingRank            = 0,              // only when PRESSIO_ENABLE_TPL_MPI=ON
    MPI_Comm comm              = MPI_COMM_WORLD  // only when PRESSIO_ENABLE_TPL_MPI=ON
)
```

5. **Use** the library

All logging is handled via macros:

```cpp
PRESSIOLOG_SPARSE("message");
PRESSIOLOG_INFO("message");
PRESSIOLOG_DEBUG("message");
PRESSIOLOG_WARNING("message");
PRESSIOLOG_ERROR("message");
```

As discussed in [Logging Levels](logging-levels), applications can also define custom macros that correspond to the above logging levels.

6. **Finalize** the logger

At the end of your program, finalize the logger with

```cpp
PRESSIOLOG_FINALIZE();
```

## Tips

Below are **three** tips for use:

- `pressio-log` is equipped with [`fmt`](https://github.com/fmtlib/fmt), so formatted strings may be passed to any of the logging commands:

```cpp
PRESSIOLOG_INFO("Sample output: {}, {}", 1, 4.5);
```

- Warnings and errors will print at the `info` and `debug` logging levels.

- All of the initialization parameters can be overriden via macros:

```cpp
PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel level);     // see Logging Levels
PRESSIOLOG_SET_OUTPUT_STREAM(pressiolog::LogTo dst);  // console, file, both
PRESSIOLOG_SET_OUTPUT_FILENAME(std::string filename);
PRESSIOLOG_SET_LOGGING_RANK(int rank);
PRESSIOLOG_SET_COMMUNICATOR(MPI_Comm comm);
```

## Sample Program

```cpp
#define PRESSIOLOG_COLORIZE_OUTPUT 1

#include <pressio-log/core.hpp>

int main() {
    PRESSIOLOG_INITIALIZE(pressiolog::LogLevel::debug);
    PRESSIOLOG_INFO("Program information: ");
    bool success;
    for (int i; i < 10; i++) {
        PRESSIOLOG_DEBUG("Iteration {}", i)
        success = i == 9 ? true : false;
    }
    if (success) {
        PRESSIOLOG_INFO("Process completed successfully.");
    }
    PRESSIOLOG_FINALIZE();
    return 0;
}

```

## Using With Pressio

If the `PRESSIO_ENABLE_LOGGING` macro is set to `1` (or `ON`, when configuring `pressio` with CMake),
`pressio` will automatically look for the `pressio-log` library and include the `pressio-log/core.hpp` file.

This means that as long as you have pointed your app to the `pressio-log` include directory, you **do not** need to explicitly include any pressio-log files.
All macros are available, as well as the `pressiolog::LogLevel` and `pressiolog::LogTo` structs.

Further, since `Pressio` uses `pressio-log` throughout its source code, you only need to initialize and finalize the logger. The rest of the logging will be handled by `Pressio`.

The following example initializes the logger with LogLevel `info` and directs the output to a file called `pressio_output.log`.

In practice, this means that all solver output (level: `sparse`) and critical program information (level: `info`) from Pressio will be written to that file. Any warnings or errors will also be logged, while all other output (level: `debug`) will be omitted.

```cpp
#define PRESSIO_ENABLE_LOGGING 1

#include <pressio/...>

int main() {
    PRESSIOLOG_INITIALIZE(
        pressiolog::LogLevel::info,
        pressiolog::LogTo::file,
        "pressio_output.log");
    // pressio code here
    PRESSIOLOG_FINALIZE();
}

```

## Testing

To build the tests, configure `pressio-log` with the `PRESSIOLOG_ENABLE_TESTS` option turned on:

```sh
cd pressio-log
mkdir build
cd build
cmake -D PRESSIOLOG_ENABLE_TESTS=ON ..
make
```

You can also configure with any of the macros described in Step 2 above.

To run the tests:

```sh
cd pressio-log/build/tests/logging
ctest -j <np>
```

> [!NOTE]
> All MPI tests require at least three processors to run.
