# pressio-log
Logging functionality for Pressio repositories

_This is a work in progress, and all subsequent information is subject to change._

## Instructions for Use:

1. **Clone** the repo

```sh
git clone https://github.com/Pressio/pressio-log.git
```

2. **Configure** the logger via CMake

| CMake Variable                | Default Value |                     All Options |
| :---------------------------- | ------------: | ------------------------------: |
| `-D PRESSIO_LOG_LEVEL`        |        `basic`| `none`, `basic`, `info`, `debug`|
| `-D PRESSIO_LOG_OUTPUT`       |      `console`|        `console`, `file`, `both`|
| `-D PRESSIO_SILENCE_WARNINGS` |          `OFF`|                       `ON`/`OFF`|
| `-D PRESSIO_ENABLE_TPL_MPI`   |          `OFF`|                       `ON`/`OFF`|

> [!NOTE]
> If `PRESSIO_LOG_OUTPUT` is set to `file` or `both`, the output will be written to `pressio.log`.

Sample build command:

```sh
cd pressio-log
mkdir build
cd build
cmake -D PRESSIO_LOG_LEVEL=DEBUG .. && make
```

3. **Include** the library

Add `pressio-log/include` to your project's include directories, and include the core file:

```cpp
#include <pressio-log/core.hpp>
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

Errors will print regardless of the logging level.
Warnings will print for any level besides `none`.

To override the configured logging level, use:

```cpp
PRESSIOLOG_SET_LEVEL(pressiolog::LogLevel::<none/basic/info/debug>)
```

## Testing

After you have built the tests (see step 2), run:

```sh
cd pressio-log/build/tests/logging
ctest -j <np>
```

> [!NOTE]
> All MPI tests require at least three ranks to run properly.
