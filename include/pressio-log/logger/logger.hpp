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
#include <vector>
#include <memory>
#include <filesystem>
#if PRESSIOLOG_ENABLE_MPI
#include <mpi.h>
#endif

#include "loglevel.hpp"
#include "logto.hpp"

/*
 * TO DO:
 * - logging destinations (console, file, both)
 * - colorized output with fmt?
 */

namespace pressiolog {

class Logger {
    public:
        // Delete copy and assignment ops
        Logger(Logger &other) = delete;
        void operator=(const Logger &) = delete;

        // Return singleton instance of PressioLogger
        static std::shared_ptr<Logger> PressioLogger() {
            static std::shared_ptr<Logger> instance(new Logger());
            return instance;
        }

        // Public logging functions
        void log(LogLevel level, const std::string& message, int target_rank=0);
        #if PRESSIOLOG_ENABLE_MPI
        void log(LogLevel level, const std::string& message, int target_rank, MPI_Comm comm);
        #endif

        // Public setters (for testing)
        void setLoggingLevel(LogLevel level);
        void setOutputStream(LogTo destination);

    private:
        // Private constructor
        Logger();

        // MPI helpers
        #if PRESSIOLOG_ENABLE_MPI
        void setComm_(MPI_Comm comm);
        void updateCurrentRank_();
        #endif

        // Private setters
        void resetLoggingLevel_();
        void setDestination_();
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

        // Member variables
        std::mutex mutex_;
        LogLevel current_level_;
        int current_rank_;
        std::string rank_str_;

        LogTo dst_;
        bool should_write_, should_log_;
        const std::string log_file_ = "pressio.log";

        #if PRESSIOLOG_ENABLE_MPI
        bool mpi_initialized_;
        MPI_Comm comm_ = MPI_COMM_WORLD;
        #endif
};

} // end namespace pressiolog

#endif // PRESSIOLOG_LOGGER_LOGGER_HPP_
