/*
//@HEADER
// ************************************************************************
//
// logger_impl.hpp
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

#ifndef PRESSIOLOG_LOGGER_LOGGER_IMPL_HPP_
#define PRESSIOLOG_LOGGER_LOGGER_IMPL_HPP_

#include <fstream>
#include <iostream>

#include "utils.hpp"
#include "colors.hpp"
#include "logger.hpp"

namespace pressiolog {

///////////////////////////////////////////////////////////////////////////////
// Initialization and finalization

inline void Logger::initialize(
    LogLevel level, LogTo destination, const std::string& filename) {
    std::call_once(init_flag_, [&]() {
        setLoggingLevel(level);
        setOutputStream(destination);
        setOutputFilename(filename);
        formatRankString_();
        logger_is_initialized_.store(true, std::memory_order_release);
    });
    if (logger_is_initialized_) {
        log(LogLevel::info, colors::green("pressio-log initialized."));
    }
}

#if PRESSIO_ENABLE_TPL_MPI
inline void Logger::initializeWithMPI(
    LogLevel level, LogTo destination, const std::string& filename,
    int logging_rank, MPI_Comm comm) {
    // Check if MPI is initialized
    int flag = 0; MPI_Initialized( &flag );
    mpi_initialized_ = flag == 1 ? true : false;
    if (mpi_initialized_) {
        updateCurrentRank_();
    }
    // Set member variables and initialize
    setLoggingRank(logging_rank);
    setCommunicator(comm);
    initialize(level, destination, filename);
}
#endif

inline void Logger::finalize() {
    log(LogLevel::info, colors::green("pressio-log finalized."));
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Public logging functions

inline void Logger::log(LogLevel level, const std::string& message) {
    if (!logger_is_initialized_) {
        std::call_once(init_warning_flag_, [&]() {
            print_(formatWarning_(
                "You are trying to use pressio-log, but it has not been initialized. "
                "Initialize with PRESSIOLOG_INITIALIZE()."));
        });
        return;
    };

    if (current_rank_ == logging_rank_) {
        switch (level) {
            case LogLevel::none:    return;
            case LogLevel::basic:   basic_(message);   break;
            case LogLevel::info:    info_(message);    break;
            case LogLevel::debug:   debug_(message);   break;
            case LogLevel::warning: warning_(message); break;
            case LogLevel::error:   error_(message);   break;
        }
    }
}

template <typename... Args>
inline void Logger::log(LogLevel level, const std::string& fmt_str, Args&&... args) {
    try {
        std::string message = fmt::format(fmt::runtime(fmt_str), utils::prep_for_fmt(std::forward<Args>(args))...);
        log(level, message);
    } catch(const fmt::v11::format_error&) {
        std::ostringstream oss;
        oss << "fmt could not format given string: " << fmt_str << " with args:\n";
        ((oss << "  " << std::to_string(args) << "\n"), ...);
        throw std::runtime_error(oss.str());
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public setters

inline void Logger::setLoggingLevel(LogLevel level) {
    logging_level_ = level;
}

inline void Logger::setOutputStream(LogTo destination) {
    dst_ = destination;
    setDestinationBools_();
}

inline void Logger::setOutputFilename(const std::string& log_file_name) {
    log_file_ = log_file_name;
}

#if PRESSIO_ENABLE_TPL_MPI
inline void Logger::setLoggingRank(int rank) {
    if (mpi_initialized_) {
        int size;
        MPI_Comm_size(comm_, &size);
        if (rank > size - 1 and logger_is_initialized_) {
            log(LogLevel::warning, "Cannot target rank {} (current comm size is {})", rank, size);
        } else {
            logging_rank_ = rank;
        }
    } else if (logger_is_initialized_) {
        log(LogLevel::warning, "MPI is enabled but not initialized. Using serial logger.");
    }
}
inline void Logger::setCommunicator(MPI_Comm comm) {
    comm_ = comm;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Private constructor

inline Logger::Logger() {}

///////////////////////////////////////////////////////////////////////////////
// MPI helpers

#if PRESSIO_ENABLE_TPL_MPI
inline void Logger::updateCurrentRank_() {
    if (mpi_initialized_) {
        MPI_Comm_rank(comm_, &current_rank_);
    }
    formatRankString_();
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Private setters

inline void Logger::setInitialized_() {
    logger_is_initialized_ = true;
}

inline void Logger::setDestinationBools_() {
    should_write_ = (dst_ >= LogTo::file) ? true : false;
    should_log_   = (dst_ == LogTo::console or dst_ == LogTo::both) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// Formatting

inline void Logger::formatRankString_() {
    rank_str_ = colors::green(fmt::format("[{}] ", current_rank_));
}

inline std::string Logger::formatWarning_(const std::string& message) const {
    return colors::yellow(fmt::format("WARNING: {}", message));
}

inline std::string Logger::formatError_(const std::string& message) const {
    return colors::red(fmt::format("ERROR: {}", message));
}

///////////////////////////////////////////////////////////////////////////////
// Internal logging functions

inline void Logger::basic_(const std::string& message) {
    if (logging_level_ >= LogLevel::basic) {
        log_(message);
    }
}
inline void Logger::info_(const std::string& message) {
    if (logging_level_ >= LogLevel::info) {
        log_(message);
    }
}
inline void Logger::debug_(const std::string& message) {
    if (logging_level_ >= LogLevel::debug) {
        log_(message);
    }
}
inline void Logger::warning_(const std::string& message) {
    #if not PRESSIO_SILENCE_WARNINGS
    if (logging_level_ >= LogLevel::info) {
        log_(formatWarning_(message));
    }
    #endif
}
inline void Logger::error_(const std::string& message) {
    if (logging_level_ >= LogLevel::info) {
        log_(formatError_(message));
    }
}

///////////////////////////////////////////////////////////////////////////////
// Output Functions

inline void Logger::log_(const std::string& message) {
    auto out_str = rank_str_ + message;
    std::lock_guard<std::mutex> lock(mutex_);
    if (should_log_)   print_(out_str);
    if (should_write_) write_(out_str);
}

inline void Logger::print_(const std::string& message) {
    std::cout << message << std::endl;
}

// TO DO: Opening/closing every time is inefficient.
//        We could open on INITIALIZE() and close on
//        FINALIZE(), but we would also want to flush
//        periodically in case the run terminates early.
inline void Logger::write_(const std::string& message) {
    std::ofstream file;
    file.open(log_file_, std::ios::app);
    file << message << std::endl;
    file.close();
}

} // end namespace pressiolog

#endif // PRESSIOLOG_LOGGER_LOGGERIMPL__HPP_
