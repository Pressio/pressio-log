/*
//@HEADER
// ************************************************************************
//
// logger.hpp
//                     		     pressio-log
//                             Copyright 2025
//    National Technology & Engineering Solutions of Sandia, LLC (NTESS)
//
// Under the terms of Contract DE-NA0003525 with NTESS, the
// U.S. Government retains certain rights in this software.
//
// Pressio is licensed under BSD-3-Clause terms of use:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Francesco Rizzi (fnrizzi@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef PRESSIOLOG_LOGGER_LOGGER_HPP_
#define PRESSIOLOG_LOGGER_LOGGER_HPP_

#include <mutex>
#include <atomic>
#include <vector>
#include <memory>
#include <filesystem>

#include <pressio-log/fmt/fmt.h>
#if PRESSIO_ENABLE_TPL_MPI
#include <mpi.h>
#endif

#include "loglevel.hpp"
#include "logto.hpp"

namespace pressiolog {

class Logger {
    public:
        /*
         * Returns a shared pointer to the singleton instance of PressioLogger.
         *
         * The static variable `instance` is initialized only once (when it is first accessed).
         * Since C++11, the initialization of "magic statics" is guaranteed to be thread safe.
         *
         * For more info on magic statics:
         *     https://blog.mbedded.ninja/programming/languages/c-plus-plus/magic-statics/
         */
        static std::shared_ptr<Logger> PressioLogger() {
            static std::shared_ptr<Logger> instance(new Logger());
            return instance;
        }

        // Initialization and finalization
        void initialize(
            LogLevel level = LogLevel::basic,
            LogTo destination = LogTo::console,
            const std::string& filename = "pressio.log"
        );
        #if PRESSIO_ENABLE_TPL_MPI
        void initializeWithMPI(
            LogLevel level = LogLevel::basic,
            LogTo destination = LogTo::console,
            const std::string& filename = "pressio.log",
            int logging_rank = 0,
            MPI_Comm comm = MPI_COMM_WORLD
        );
        #endif
        void finalize();

        // Public logging functions
        void log(LogLevel level, const std::string& message);
        template <typename... Args>
        void log(LogLevel level, fmt::format_string<Args...> fmt_str, Args&&... args);

        // Public setters
        void setLoggingLevel(LogLevel level);
        void setOutputStream(LogTo destination);
        void setOutputFilename(const std::string& log_file_name);
        #if PRESSIO_ENABLE_TPL_MPI
        void setLoggingRank(int rank);
        void setCommunicator(MPI_Comm comm);
        #endif

    private:
        // Private constructor
        Logger();

        // Delete copy and move constructors
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator=(Logger&&) = delete;

        // Check initialization
        void assertLoggerIsInitialized_();

        // MPI helpers
        #if PRESSIO_ENABLE_TPL_MPI
        void updateCurrentRank_();
        #endif

        // Private setters
        void setInitialized_();
        void setDestinationBools_();

        // Formatting
        void formatRankString_();
        std::string formatWarning_(const std::string& message) const;
        std::string formatError_(const std::string& message) const;

        // Internal logging functions
        void basic_(const std::string& message);
        void info_(const std::string& message);
        void debug_(const std::string& message);
        void warning_(const std::string& message);
        void error_(const std::string& message);

        // Output functions
        void log_(const std::string& message);
        void print_(const std::string& message);
        void write_(const std::string& message);

        ///////////////////////////////////////////////////////////////////////
        // Member variables

        // General
        std::mutex mutex_;
        int current_rank_{0};
        std::string rank_str_;

        // Initialization
        std::atomic<bool> logger_is_initialized_;
        std::once_flag init_flag_;

        // Configuration
        LogLevel logging_level_{LogLevel::basic};
        int logging_rank_{0};

        // Output
        LogTo dst_{LogTo::console};
        bool should_write_{false};
        bool should_log_{true};
        std::string log_file_{"pressio.log"};

        // MPI
        #if PRESSIO_ENABLE_TPL_MPI
        bool mpi_initialized_{false};
        MPI_Comm comm_{MPI_COMM_WORLD};
        #endif
};

} // end namespace pressiolog

#endif // PRESSIOLOG_LOGGER_LOGGER_HPP_
