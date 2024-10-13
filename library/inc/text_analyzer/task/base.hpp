#pragma once
#ifndef PCH
    #include <stdext/producer_consumer_queue.hpp>
    #include <text_analyzer/base.hpp>
#endif

namespace text_analyzer
{
    namespace fs = ::std::filesystem;

    using lock_guard_t = std::lock_guard<std::mutex>;

    class factory;
}

namespace text_analyzer::task
{
    using file_path_queue = stdext::producer_consumer_queue<fs::path>;

    using result_queue = stdext::producer_consumer_queue<result_unique_ptr>;

    // base class for analysis task to be used by a thread.
    class base
    {
    public:
        base() = default;
        base(base&& a): item_count_(a.item_count_.load()) {}

        std::uint32_t item_count() const noexcept { return item_count_; }

    protected:
        std::atomic_uint32_t item_count_ {};
    };

    // extended class for analysis task to be used by a thread.
    class extended: public base
    {
    public:
        extended(factory& factory, file_path_queue& file_paths): factory_(factory), file_paths_(file_paths) {}
        extended(extended&& item): extended(item.factory_, item.file_paths_) {}

    protected:
        factory& factory_;
        file_path_queue& file_paths_;
    };
}
