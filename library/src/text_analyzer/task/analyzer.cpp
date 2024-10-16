#include "text_analyzer/task/analyzer.hpp"
#include "text_analyzer/pool.hpp"
#include "text_analyzer/wrapper.hpp"
#include <syncstream>

namespace text_analyzer::task
{
    analyzer::analyzer(pool& pool, file_path_queue& file_paths, result_queue& results):
        extended(pool, file_paths),
        results_(results)
    {
    }

    analyzer::analyzer(analyzer&& item): extended(std::move(item)), results_(item.results_)
    {
    }

    result_unique_ptr analyzer::analyze_file(const fs::path& file_path) const
    {
        try
        {
            wrapper actual_analyzer(pool_, file_path);
            auto result = actual_analyzer();

            log() << "analysis finished: " << file_path << std::endl;

            return result;
        }
        catch (const std::exception& ex)
        {
            log() << "failed to process " << file_path << ": " << ex.what() << std::endl;
        }
        catch (...)
        {
            log() << "failed to process " << file_path << ": uknown error" << std::endl;
        }

        return {};
    }

    bool analyzer::execute()
    {
        auto opt_file = file_paths_.try_pop();
        if (opt_file)
        {
            auto& file = *opt_file;
            auto result = analyze_file(file);
            if (result != nullptr)
            {
                results_.push(std::move(result));
                ++item_count_;
            }

            return true;
        }

        return false;
    }

    void analyzer::operator()() noexcept
    {
        results_.begin_enqueue();
        while (true)
        {
            try
            {
                log() << "trying analyzing file\n";
                if (!execute())
                {
                    log() << "exit - no more files\n";
                    break;
                }
            }
            catch (...)
            {
                // break;
            }
        }

        try
        {
            results_.end_enqueue();
        }
        catch (const std::exception& ex)
        {
        }
        catch (...)
        {
        }
    }
}
