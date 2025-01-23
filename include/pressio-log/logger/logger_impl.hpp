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

#include <iostream>

#include "logger.hpp"

namespace pressiolog {

///////////////////////////////////////////////////////////////////////////////
// Public logging functions

void Logger::log(LogLevel level, const std::string& message, int target_rank) {
    if (level == LogLevel::none) return;

    else if (current_rank_ == target_rank) {
        switch (level) {
            case LogLevel::basic:   basic_(message);   break;
            case LogLevel::info:    info_(message);    break;
            case LogLevel::debug:   debug_(message);   break;
            case LogLevel::warning: warning_(message); break;
            case LogLevel::error:   error_(message);   break;
        }
    }
}

#if PRESSIOLOG_ENABLE_MPI
void Logger::log(LogLevel level, const std::string& message, int target_rank, MPI_Comm comm) {
    std::cout << " Calling log 2" << std::endl;
    if (mpi_initialized_) {
        setComm_(comm);
        updateCurrentRank_();
    }
    log(level, message, target_rank);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Set or reset current logging level

void Logger::setCurrentLevel(LogLevel level) {
    current_level_ = level;
}

void Logger::resetCurrentLevel() {
    #ifdef PRESSIOLOG_LOG_LEVEL
    current_level_ = static_cast<LogLevel>(PRESSIOLOG_LOG_LEVEL);
    #else
    current_level_ = LogLevel::basic;
    #endif
}

///////////////////////////////////////////////////////////////////////////////
// Private Constructor

Logger::Logger() : current_rank_(0) {
    #if PRESSIOLOG_ENABLE_MPI
    int flag = 0; MPI_Initialized( &flag );
    mpi_initialized_ = flag == 1 ? true : false;
    if (mpi_initialized_) {
        updateCurrentRank_();
    }
    #endif
    resetCurrentLevel();
}

///////////////////////////////////////////////////////////////////////////////
// MPI Helpers

#if PRESSIOLOG_ENABLE_MPI
void Logger::setComm_(MPI_Comm comm) {
    comm_ = comm;
}

void Logger::updateCurrentRank_() {
    if (mpi_initialized_) {
        MPI_Comm_rank(comm_, &current_rank_);
    }
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Formatting

std::string Logger::formatRank_() const {
    auto rank_str = std::to_string(current_rank_);
    return "[" + rank_str + "] ";
}
std::string Logger::formatWarning_(const std::string& message) const {
    return "WARNING: " + message;
}
std::string Logger::formatError_(const std::string& message) const {
    return "ERROR: " + message;
}

///////////////////////////////////////////////////////////////////////////////
// Internal logging functions

void Logger::basic_(const std::string& message) {
    if (current_level_ >= LogLevel::basic) {
        log_(message);
    }
}
void Logger::info_(const std::string& message) {
    if (current_level_ >= LogLevel::info) {
        log_(message);
    }
}
void Logger::debug_(const std::string& message) {
    std::cout << " about to debug print: " << (current_level_ >= LogLevel::debug) << std::endl;
    if (current_level_ >= LogLevel::debug) {
        log_(message);
    }
}
void Logger::warning_(const std::string& message) {
    #if not PRESSIOLOG_SILENCE_WARNINGS
    if (current_level_ >= LogLevel::basic) {
        log_(formatWarning_(message));
    }
    #endif
}
void Logger::error_(const std::string& message) {
    log_(formatError_(message));
}
void Logger::log_(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << formatRank_() << message << std::endl;
}

} // end namespace pressiolog


#endif // PRESSIOLOG_LOGGER_LOGGERIMPL__HPP_