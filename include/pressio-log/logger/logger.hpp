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

#include <memory>
#include <iostream>

#ifdef PRESSIO_ENABLE_TPL_MPI
#include <mpi.h>
#endif

#include "levels.hpp"

/*
 * TO DO:
 * - support for MPI
 * - logging destinations (console, file, both)
 * - colorized output with fmt?
 */

namespace pressiolog {

class Logger {
    public:

        // Delete copy and assignment ops
        Logger(Logger &other) = delete;
        void operator=(const Logger &) = delete;

        // Define getter ftn
        static std::shared_ptr<Logger> pressio_logger() {
            static std::shared_ptr<Logger> instance(new Logger());
            return instance;
        }

        // Public logging function
        void log(level level, const std::string& message) {
            switch (level) {
                case none:
                    break;
                case basic:
                    basic_(message);
                    break;
                case info:
                    info_(message);
                    break;
                case debug:
                    debug_(message);
                    break;
                case warning:
                    warning_(message);
                    break;
                case error:
                    error_(message);
                    break;
                default:
                    info_(message);
                    break;
            }
        }

        void setCurrentLevel(level level) {
            current_level_ = level;
        }

        void resetCurrentLevel() {
            #ifdef PRESSIOLOG_LOG_LEVEL
            current_level_ = static_cast<level>(PRESSIOLOG_LOG_LEVEL);
            #else
            current_level_ = level::basic;
            #endif
        }

    private:
        Logger() {
            resetCurrentLevel();
        }

        // Internal logging functions
        void basic_(const std::string& message) {
            if (current_level_ >= level::basic) {
                log_(message);
            }
        }
        void info_(const std::string& message) {
            if (current_level_ >= level::info) {
                log_(message);
            }
        }
        void debug_(const std::string& message) {
            if (current_level_ >= level::debug) {
                log_(message);
            }
        }
        void warning_(const std::string& message) {
            #if not PRESSIOLOG_SILENCE_WARNINGS
            if (current_level_ > level::none) {
                log_("[WARNING] " + message);
            }
            #endif
        }
        void error_(const std::string& message) {
            log_("[ERROR]   " + message);
        }

        // Low level logger
        void log_(const std::string& message) {
            std::cout << message << std::endl;
        }

        // Member variables
        level current_level_;
};

} // end namespace pressiolog

#endif // PRESSIOLOG_LOGGER_LOGGER_HPP_
