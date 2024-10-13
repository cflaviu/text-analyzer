#include "text_analyzer/task/saver.hpp"

namespace text_analyzer::task
{
    saver::saver(result_queue& results, const std::uint32_t bulk_size): results_(results), bulk_size_(bulk_size)
    {
    }

    void saver::store()
    {
        log() << "saved " << bulk_results_.size() << " results into db\n";

        item_count_ += bulk_results_.size();
        bulk_results_.clear();
    }

#if 1
    void saver::execute()
    {
        for (bool ok = true; ok;)
        {
            log() << "trying collecting results\n";

            const auto [enqueueing_active_, queue_empty] = results_.dequeue(bulk_results_, bulk_size_);
            if (!bulk_results_.empty())
                store();
            ok = enqueueing_active_ || !queue_empty;
        }

        log() << "saving done\n";
    }
#else
    void saver::execute()
    {
        while (true)
        {
            {
                lock_guard_t cout_lock(cout_mutex);
                log() << "trying collect results\n";
            }

            auto opt_result = results_.try_pop();
            if (opt_result)
            {
                auto& result = *opt_result;
                bulk_results_.push_back(std::move(result));

                {
                    lock_guard_t cout_lock(cout_mutex);
                    log() << "result collected\n";
                }

                // Save in batches
                if (bulk_results_.size() >= bulk_size_)
                    store();
            }
            else
            {
                {
                    lock_guard_t cout_lock(cout_mutex);
                    log() << "exit - no more results\n";
                }

                break;
            }
        }

        // Save any remaining results
        if (!bulk_results_.empty())
            store();
    }
#endif

    void saver::operator()() noexcept
    {
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
    }
}
