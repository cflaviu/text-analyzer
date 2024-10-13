#include "text_analyzer/training/application.hpp"
#include <iostream>

enum error
{
    none,
    directory_not_found,
    no_directory,
    exception,
    unknown_exception,
};

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
                text_analyzer::training::application app;
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
        return error::exception;
    }
    catch (...)
    {
        std::cout << "unknown exception occured\n";
        return error::unknown_exception;
    }

    return error::none;
}
