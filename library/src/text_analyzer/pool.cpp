#include "text_analyzer/pool.hpp"
#include "text_analyzer/pdf.hpp"
#include "text_analyzer/text.hpp"
#include <iostream>
#include <syncstream>
#include <thread>

namespace text_analyzer
{
    using lock_guard_t = std::lock_guard<std::mutex>;

    std::osyncstream log()
    {
        std::osyncstream out {std::cout};
        const auto id = std::hash<std::thread::id> {}(std::this_thread::get_id());
        out << '[' << std::setfill('0') << std::setw(3) << (id % 1000) << ']' << ' ';
        return out;
    }

    pool::pool(map_type map): map_(std::move(map))
    {
    }

    bool pool::is_accepted(const fs::path& file_extension) const noexcept
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_extension);
        return pos != map_.end();
    }

    shared_ptr pool::try_acquire(const fs::path& file_extension)
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_extension);
        if (pos != map_.end())
            return pos->second.try_acquire();
        return {}; // Unsupported file_path type
    }

    shared_ptr pool::acquire(const fs::path& file_extension)
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_extension);
        if (pos == map_.end())
            throw std::runtime_error("failed to find associated analyzer");
        return pos->second.acquire();
    }

    void pool::release(const fs::path& file_path, shared_ptr analyzer)
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_path.extension());
        if (pos != map_.end())
            return pos->second.release(analyzer);
    }
}
