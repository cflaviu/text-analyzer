#include "text_analyzer/task/file_path_generator.hpp"
#include "text_analyzer/pool.hpp"
#include <syncstream>

namespace text_analyzer::task
{
    file_path_generator::file_path_generator(pool& pool, file_path_queue& file_paths, fs::path directory):
        extended(pool, file_paths),
        directory_(std::move(directory))
    {
    }

    file_path_generator::file_path_generator(file_path_generator&& item): extended(std::move(item)), directory_(std::move(item.directory_))
    {
    }

    void file_path_generator::execute()
    {
        for (const auto& entry: fs::recursive_directory_iterator(directory_))
            if (entry.is_regular_file())
            {
                const auto& file_path = entry.path();
                if (pool_.is_accepted(file_path.extension()))
                {
                    file_paths_.push(file_path);
                    log() << "file found: " << file_path << std::endl;
                    ++item_count_; // Increment the count of files being processed
                }
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
            log() << "file_path_generator exception: " << ex.what() << std::endl;
        }
        catch (...)
        {
            log() << "file_path_generator unknown exception\n";
        }

        try
        {
            file_paths_.end_enqueue();
            log() << "file path generation done\n";
        }
        catch (const std::exception& ex)
        {
            log() << "file_path_generator ending-equeue exception: " << ex.what() << std::endl;
        }
        catch (...)
        {
            log() << "file_path_generator ending-equeue unknown exception\n";
        }
    }
}
