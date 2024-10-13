#include "text_analyzer/task/file_path_generator.hpp"
#include "text_analyzer/factory.hpp"
#include <syncstream>

namespace text_analyzer::task
{
    file_path_generator::file_path_generator(factory& factory, file_path_queue& file_paths, fs::path directory):
        extended(factory, file_paths),
        directory_(std::move(directory))
    {
    }

    file_path_generator::file_path_generator(file_path_generator&& item): extended(std::move(item)), directory_(std::move(item.directory_))
    {
    }

    void file_path_generator::execute()
    {
        for (const auto& entry: fs::recursive_directory_iterator(directory_))
            if (entry.is_regular_file() && factory_.is_accepted(entry.path().extension()))
            {
                file_paths_.push(entry.path());
                log() << "file found: " << entry.path() << '\n';
                ++item_count_; // Increment the count of files being processed
            }
    }

    void file_path_generator::operator()() noexcept
    {
        file_paths_.begin_enqueue();

        try
        {
            execute();
        }
        catch (const std::exception& ex)
        {
        }
        catch (...)
        {
        }

        try
        {
            file_paths_.end_enqueue();
            log() << "file path generation done\n";
        }
        catch (const std::exception& ex)
        {
        }
        catch (...)
        {
        }
    }
}
