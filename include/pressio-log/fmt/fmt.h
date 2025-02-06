// This file was created specifically for use with pressio-log.
// The full fmt license can be found in include/pressio-log/fmt/format.h

#ifndef FMT_FMT_H_
#define FMT_FMT_H_

#define FMT_HEADER_ONLY

#if PRESSIO_EXTERNAL_FMT
    #include <fmt/format.h>
#else
    #include <pressio-log/fmt/format.h>
#endif

#endif // FMT_FMT_H_
