#pragma once
#ifndef PCH
    #include <text_analyzer/task/base.hpp>
#endif

namespace text_analyzer::task
{
    // Saver task to be used by a thread.
    class saver: public base
    {
    public:
        saver(result_queue& results, const std::uint32_t bulk_size = 10u);
        saver(saver&&) = delete;

        saver() = delete;
        saver(const saver&) = delete;

        void operator()() noexcept;

    private:
        using result_vector = std::vector<result_unique_ptr>;

        void execute();
        void store();

        result_queue& results_;
        result_vector bulk_results_ {};
        const std::uint32_t bulk_size_;
    };
}
