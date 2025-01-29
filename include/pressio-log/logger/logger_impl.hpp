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

#include "colors.hpp"
#include "logger.hpp"

namespace pressiolog {

///////////////////////////////////////////////////////////////////////////////
// Initialization and finalization

void Logger::initialize(
    LogLevel level, LogTo destination, const std::string& filename) {
    std::call_once(init_flag_, [&]() {
        setLoggingLevel(level);
        setOutputStream(destination);
        setOutputFilename(filename);
        formatRankString_();
        logger_is_initialized_.store(true, std::memory_order_release);
        info_("pressio-log initialized.");
    });
}

#if PRESSIOLOG_ENABLE_MPI
void Logger::initializeWithMPI(
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

void Logger::finalize() {
    assertLoggerIsInitialized_();
    info_("pressio-log finalized.");
    return;
}

///////////////////////////////////////////////////////////////////////////////
// Public logging functions

void Logger::log(LogLevel level, const std::string& message) {
    assertLoggerIsInitialized_();
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

#if PRESSIOLOG_ENABLE_MPI
void Logger::log(LogLevel level, const std::string& message, int logging_rank) {
    assertLoggerIsInitialized_();
    if (logging_rank != logging_rank_) setLoggingRank(logging_rank);
    log(level, message);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Public setters

void Logger::setLoggingLevel(LogLevel level) {
    logging_level_ = level;
}

void Logger::setOutputStream(LogTo destination) {
    dst_ = destination;
    setDestinationBools_();
}

void Logger::setOutputFilename(const std::string& log_file_name) {
    log_file_ = log_file_name;
}

#if PRESSIOLOG_ENABLE_MPI
void Logger::setLoggingRank(int rank) {
    if (mpi_initialized_) {
        int size;
        MPI_Comm_size(comm_, &size);
        if (rank > size - 1) {
            warning_("Cannot target rank " + std::to_string(rank) + \
                     " (current communicator size is " + std::to_string(size) + ").");
        } else {
            logging_rank_ = rank;
        }
    } else {
        warning_("Cannot set target rank (MPI is not initialized).");
    }
}
void Logger::setCommunicator(MPI_Comm comm) {
    comm_ = comm;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Private constructor

Logger::Logger() : logger_is_initialized_(false) {
    #if !PRESSIOLOG_ENABLED
    initialize(LogLevel::none);
    #endif
}

///////////////////////////////////////////////////////////////////////////////
// Check initialization

void Logger::assertLoggerIsInitialized_() {
    if (!logger_is_initialized_) {
        throw std::runtime_error(
            "pressio-log has not been initialized. "
            "Call PRESSIOLOG_INITIALIZE() before using.");
    }
}

///////////////////////////////////////////////////////////////////////////////
// MPI helpers

#if PRESSIOLOG_ENABLE_MPI
void Logger::updateCurrentRank_() {
    if (mpi_initialized_) {
        MPI_Comm_rank(comm_, &current_rank_);
    }
    formatRankString_();
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Private setters

void Logger::setInitialized_() {
    logger_is_initialized_ = true;
}

void Logger::setDestinationBools_() {
    should_write_ = (dst_ >= LogTo::file) ? true : false;
    should_log_   = (dst_ == LogTo::console or dst_ == LogTo::both) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// Formatting

void Logger::formatRankString_() {
    rank_str_ = "[" + std::to_string(current_rank_) + "] ";
}
std::string Logger::formatWarning_(const std::string& message) const {
    // Colors only if PRESSIOLOG_COLORIZED_OUTPUT is enabled
    return colors::yellow("WARNING: " + message);
}
std::string Logger::formatError_(const std::string& message) const {
    // Colors only if PRESSIOLOG_COLORIZED_OUTPUT is enabled
    return colors::red("ERROR: " + message);
}

///////////////////////////////////////////////////////////////////////////////
// Internal logging functions

void Logger::basic_(const std::string& message) {
    if (logging_level_ >= LogLevel::basic) {
        log_(message);
    }
}
void Logger::info_(const std::string& message) {
    if (logging_level_ >= LogLevel::info) {
        log_(message);
    }
}
void Logger::debug_(const std::string& message) {
    if (logging_level_ >= LogLevel::debug) {
        log_(message);
    }
}
void Logger::warning_(const std::string& message) {
    #if not PRESSIOLOG_SILENCE_WARNINGS
    if (logging_level_ >= LogLevel::info) {
        log_(formatWarning_(message));
    }
    #endif
}
void Logger::error_(const std::string& message) {
    if (logging_level_ >= LogLevel::info) {
        log_(formatError_(message));
    }
}

///////////////////////////////////////////////////////////////////////////////
// Output Functions

void Logger::log_(const std::string& message) {
    auto out_str = rank_str_ + message;
    std::lock_guard<std::mutex> lock(mutex_);
    if (should_log_)   print_(out_str);
    if (should_write_) write_(out_str);
}

void Logger::print_(const std::string& message) {
    std::cout << message << std::endl;
}

// TO DO: Opening/closing every time is inefficient,
//        but we need to determine an exit signal if
//        if we want to only close at the end.
void Logger::write_(const std::string& message) {
    std::ofstream file;
    file.open(log_file_, std::ios::app);
    file << message << std::endl;
    file.close();
}

} // end namespace pressiolog

#endif // PRESSIOLOG_LOGGER_LOGGERIMPL__HPP_
