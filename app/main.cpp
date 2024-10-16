#include "text_analyzer/application.hpp"
#include <iostream>

enum error
{
    none,
    directory_not_found,
    no_directory,
    exception,
    unknown_exception,
};

namespace text_analyzer
{
    pool::map_type create_factory_map(const std::size_t pool_size);
}

int main(const int argc, const char* const argv[]) noexcept
{
    try
    {
        if (argc == 2)
        {
            namespace fs = ::std::filesystem;

            const fs::path directory = argv[1u];
            if (fs::exists(directory))
            {
                using namespace text_analyzer;

                application::config config {};
                config.analyzer_thread_count = 16u;

                application app(config, text_analyzer::create_factory_map(config.analyzer_thread_count));
                app.run(directory);
            }
            else
            {
                std::cout << directory << " not found\n";
                return error::directory_not_found;
            }
        }
        else
        {
            std::cout << "<directory> parameter expected\n";
            return error::no_directory;
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << "exception occured\n";
        return error::exception;
    }
    catch (...)
    {
        std::cout << "unknown exception occured\n";
        return error::unknown_exception;
    }

    return error::none;
}
