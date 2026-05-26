/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// compatibility for Boost 1.74 (no boost/process/v1/) and 1.88+ (no boost/process/)
#if __has_include(<boost/process/v1/args.hpp>)
#define BOOST_PROCESS_V1_HEADER(header) <boost/process/v1/header>
#define BOOST_PROCESS_VERSION 1
#else
#define BOOST_PROCESS_V1_HEADER(header) <boost/process/header>
#endif

#include "StartProcess.h"
#include "Errors.h"
#include "Log.h"
#include "Optional.h"

#include <boost/algorithm/string/join.hpp>
#include BOOST_PROCESS_V1_HEADER(args.hpp)
#include BOOST_PROCESS_V1_HEADER(child.hpp)
#include BOOST_PROCESS_V1_HEADER(env.hpp)
#include BOOST_PROCESS_V1_HEADER(error.hpp)
#include BOOST_PROCESS_V1_HEADER(exe.hpp)
#include BOOST_PROCESS_V1_HEADER(io.hpp)
#include BOOST_PROCESS_V1_HEADER(pipe.hpp)
#include BOOST_PROCESS_V1_HEADER(search_path.hpp)

#include <atomic>
#include <cstdio>
#include <future>
#include <memory>
#include <system_error>

namespace bp = boost::process;

namespace Trinity
{
class AsyncProcessResultImplementation
    : public AsyncProcessResult
{
    std::string const executable;
    std::vector<std::string> const args;
    std::string const logger;
    std::string const input_file;
    bool const is_secure;

    std::atomic<bool> was_terminated;

    Optional<std::future<int>> futureResult;
    Optional<bp::child> my_child;

public:
    explicit AsyncProcessResultImplementation(std::string executable_, std::vector<std::string> args_,
                                     std::string logger_, std::string input_file_,
                                     bool secure)
        : executable(std::move(executable_)), args(std::move(args_)),
          logger(std::move(logger_)), input_file(std::move(input_file_)),
          is_secure(secure), was_terminated(false) { }

    AsyncProcessResultImplementation(AsyncProcessResultImplementation const&) = delete;
    AsyncProcessResultImplementation& operator= (AsyncProcessResultImplementation const&) = delete;
    AsyncProcessResultImplementation(AsyncProcessResultImplementation&&) = delete;
    AsyncProcessResultImplementation& operator= (AsyncProcessResultImplementation&&) = delete;

    ~AsyncProcessResultImplementation() = default;

    int StartProcess()
    {
        ASSERT(!my_child, "Process started already!");

        bp::ipstream outStream;
        bp::ipstream errStream;

        if (is_secure)
        {
            TC_LOG_TRACE(logger, "Starting process \"%s\".", executable.c_str());
        }
        else
        {
            TC_LOG_TRACE(logger, "Starting process \"%s\" with arguments: \"%s\".",
                executable.c_str(), boost::algorithm::join(args, " ").c_str());
        }

        // prepare file with only read permission (boost process opens with read_write)
        std::unique_ptr<FILE, decltype(&::fclose)> inputFile(
            !input_file.empty() ? fopen(input_file.c_str(), "rb") : nullptr,
            &::fclose);

        std::error_code ec;

        // Start the child process
        if (inputFile)
        {
            my_child.emplace(
                bp::exe = boost::filesystem::absolute(executable).string(),
                bp::args = args,
                bp::env = bp::environment(boost::this_process::environment()),
                bp::std_in = inputFile.get(),
                bp::std_out = outStream,
                bp::std_err = errStream,
                bp::error = ec
            );
        }
        else
        {
            my_child.emplace(
                bp::exe = boost::filesystem::absolute(executable).string(),
                bp::args = args,
                bp::env = bp::environment(boost::this_process::environment()),
                bp::std_in = bp::close,
                bp::std_out = outStream,
                bp::std_err = errStream,
                bp::error = ec
            );
        }

        if (ec)
        {
            TC_LOG_ERROR(logger, ">> Failed to start process \"%s\": %s", executable.c_str(), ec.message().c_str());
            return EXIT_FAILURE;
        }

        std::future<void> stdOutReader = std::async(std::launch::async, [&]
        {
            std::string line;
            while (std::getline(outStream, line, '\n'))
            {
                std::erase(line, '\r');
                if (!line.empty())
                    TC_LOG_INFO(logger, "%s", line.c_str());
            }
        });

        std::future<void> stdErrReader = std::async(std::launch::async, [&]
        {
            std::string line;
            while (std::getline(errStream, line, '\n'))
            {
                std::erase(line, '\r');
                if (!line.empty())
                    TC_LOG_ERROR(logger, "%s", line.c_str());
            }
        });

        my_child->wait(ec);
        int const result = !ec && !was_terminated ? my_child->exit_code() : EXIT_FAILURE;
        my_child.reset();

        stdOutReader.wait();
        stdErrReader.wait();

        TC_LOG_TRACE(logger, ">> Process \"%s\" finished with return value %i.",
            executable.c_str(), result);

        return result;
    }

    void SetFuture(std::future<int> result_)
    {
        futureResult.emplace(std::move(result_));
    }

    /// Returns the future which contains the result of the process
    /// as soon it is finished.
    std::future<int>& GetFutureResult() override
    {
        ASSERT(futureResult.has_value(), "The process wasn't started!");
        return *futureResult;
    }

    /// Tries to terminate the process
    void Terminate() override
    {
        if (my_child)
        {
            was_terminated = true;
            std::error_code ec;
            my_child->terminate(ec);
        }
    }
};

int StartProcess(std::string const& executable, std::vector<std::string> const& args,
    std::string const& logger, std::string input_file, bool secure)
{
    AsyncProcessResultImplementation handle(
        executable, args, logger, std::move(input_file), secure);

    return handle.StartProcess();
}

std::shared_ptr<AsyncProcessResult> StartAsyncProcess(std::string executable, std::vector<std::string> args,
    std::string logger, std::string input_file, bool secure)
{
    std::shared_ptr<AsyncProcessResultImplementation> handle = std::make_shared<AsyncProcessResultImplementation>(
        std::move(executable), std::move(args), std::move(logger), std::move(input_file), secure);

    handle->SetFuture(std::async(std::launch::async, [handle] { return handle->StartProcess(); }));
    return handle;
}

std::string SearchExecutableInPath(std::string const& filename)
{
    try
    {
        return bp::search_path(filename).string();
    }
    catch (...)
    {
        return "";
    }
}

} // namespace Trinity
