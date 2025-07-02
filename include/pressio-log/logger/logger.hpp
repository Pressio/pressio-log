
#ifndef PRESSIOLOG_LOGGER_LOGGER_HPP_
#define PRESSIOLOG_LOGGER_LOGGER_HPP_

#include <mutex>
#include <atomic>
#include <vector>
#include <memory>
#include <filesystem>

#include <pressio-log/fmt/fmt.h>
#ifdef PRESSIO_ENABLE_TPL_MPI
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
         * For more info on "magic statics":
         *     https://blog.mbedded.ninja/programming/languages/c-plus-plus/magic-statics/
         */
        static std::shared_ptr<Logger> PressioLogger() {
            static std::shared_ptr<Logger> instance(new Logger());
            return instance;
        }

        // Initialization and finalization
        void initialize(
            LogLevel level = LogLevel::sparse,
            LogTo destination = LogTo::console,
            const std::string& filename = "pressio.log"
        );
        #ifdef PRESSIO_ENABLE_TPL_MPI
        void initializeWithMPI(
            LogLevel level = LogLevel::sparse,
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
        void log(LogLevel level, const std::string& fmt_str, Args&&... args);

        // Public setters
        void setLoggingLevel(LogLevel level);
        void setOutputStream(LogTo destination);
        void setOutputFilename(const std::string& log_file_name);
        #ifdef PRESSIO_ENABLE_TPL_MPI
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
        #ifdef PRESSIO_ENABLE_TPL_MPI
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
        void sparse_(const std::string& message);
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
        std::atomic<bool> logger_is_initialized_{false};
        std::unique_ptr<std::once_flag> init_flag_ = std::make_unique<std::once_flag>();
        std::unique_ptr<std::once_flag> init_warning_flag_ = std::make_unique<std::once_flag>();
        std::mutex init_mutex_;

        // Configuration
        LogLevel logging_level_{LogLevel::none};
        int logging_rank_{0};

        // Output
        LogTo dst_{LogTo::console};
        bool should_write_{false};
        bool should_log_{true};
        std::string log_file_{"pressio.log"};

        // MPI
        #ifdef PRESSIO_ENABLE_TPL_MPI
        bool mpi_initialized_{false};
        MPI_Comm comm_{MPI_COMM_WORLD};
        #endif
};

} // end namespace pressiolog

#endif  // PRESSIOLOG_LOGGER_LOGGER_HPP_
