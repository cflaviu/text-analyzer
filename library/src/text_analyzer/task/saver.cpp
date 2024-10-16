#include "text_analyzer/task/saver.hpp"
#include <syncstream>

namespace text_analyzer::task
{
    saver::saver(result_queue& results, const std::uint32_t bulk_size): results_(results), bulk_size_(bulk_size)
    {
    }

    void saver::store()
    {
        log() << "saved " << bulk_results_.size() << " results into db" << std::endl;

        item_count_ += bulk_results_.size();
        bulk_results_.clear();
    }

    void saver::execute()
    {
        for (bool ok = true; ok;)
        {
            log() << "trying collecting results" << std::endl;

            const auto [enqueueing_active_, queue_empty] = results_.dequeue(bulk_results_, bulk_size_);
            if (!bulk_results_.empty())
                store();
            ok = enqueueing_active_ || !queue_empty;
        }

        log() << "saving done" << std::endl;
    }

    void saver::operator()() noexcept
    {
        try
        {
            execute();
        }
        catch (const std::exception& ex)
        {
            log() << "saver exception: " << ex.what() << std::endl;
        }
        catch (...)
        {
            log() << "saver unknown exception" << std::endl;
        }
    }
}
