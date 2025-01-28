/*
//@HEADER
// ************************************************************************
//
// core.hpp
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

#ifndef PRESSIOLOG_CORE_HPP_
#define PRESSIOLOG_CORE_HPP_

#include "./logger/logger_impl.hpp"

// Standard logging macros

#define LOG(level, ...) \
   pressiolog::Logger::PressioLogger()->log(level, __VA_ARGS__);

#define PRESSIOLOG_BASIC(...)   LOG(pressiolog::LogLevel::basic, __VA_ARGS__)
#define PRESSIOLOG_INFO(...)    LOG(pressiolog::LogLevel::info, __VA_ARGS__)
#define PRESSIOLOG_DEBUG(...)   LOG(pressiolog::LogLevel::debug, __VA_ARGS__)
#define PRESSIOLOG_WARNING(...) LOG(pressiolog::LogLevel::warning, __VA_ARGS__)
#define PRESSIOLOG_ERROR(...)   LOG(pressiolog::LogLevel::error, __VA_ARGS__)

// Initialization/Finalization

#if PRESSIOLOG_ENABLE_MPI
    #define PRESSIOLOG_INITIALIZE(...) \
        pressiolog::Logger::PressioLogger()->initializeWithMPI(__VA_ARGS__)
#else
    #define PRESSIOLOG_INITIALIZE(...) \
        pressiolog::Logger::PressioLogger()->initialize(__VA_ARGS__)
#endif


#define PRESSIOLOG_FINALIZE(...) \
    pressiolog::Logger::PressioLogger()->finalize(__VA_ARGS__);

// Setters

#define PRESSIOLOG_SET_LEVEL(...) \
    pressiolog::Logger::PressioLogger()->setLoggingLevel(__VA_ARGS__);

#define PRESSIOLOG_SET_OUTPUT_STREAM(...) \
    pressiolog::Logger::PressioLogger()->setOutputStream(__VA_ARGS__);

#define PRESSIOLOG_SET_OUTPUT_FILENAME(...) \
    pressiolog::Logger::PressioLogger()->setOutputFilename(__VA_ARGS__);

#if PRESSIOLOG_ENABLE_MPI
#define PRESSIOLOG_SET_COMMUNICATOR(...) \
    pressiolog::Logger::PressioLogger()->setCommunicator(__VA_ARGS__);
#endif

#endif // PRESSIOLOG_CORE_HPP
