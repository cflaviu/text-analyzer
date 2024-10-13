#include "text_analyzer/factory.hpp"
#include "text_analyzer/pdf.hpp"
#include "text_analyzer/text.hpp"
#include <iostream>
#include <syncstream>
#include <thread>

namespace text_analyzer
{
    using lock_guard_t = std::lock_guard<std::mutex>;

    std::ostream& log()
    {
        static std::osyncstream out {std::cout};
        const auto id = std::hash<std::thread::id> {}(std::this_thread::get_id());
        return out; // << '[' << std::setfill('0') << std::setw(3) << (id % 1000) << "] ";
    }

    factory::factory(size_t pool_size, creation_funcion text_analyzer_creator, creation_funcion pdf_analyzer_creator)
    {
        map_[text::base::ext] = {pool_size, [&text_analyzer_creator] { return text_analyzer_creator(); }};
        map_[pdf::base::ext] = {pool_size, [&pdf_analyzer_creator] { return pdf_analyzer_creator(); }};
    }

    bool factory::is_accepted(const fs::path& file_path) const noexcept
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_path);
        return pos != map_.end();
    }

    shared_ptr factory::try_acquire(const fs::path& file_path)
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_path.extension());
        if (pos != map_.end())
            return pos->second.try_acquire();
        return {}; // Unsupported file_path type
    }

    shared_ptr factory::acquire(const fs::path& file_path)
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_path.extension());
        if (pos == map_.end())
            throw std::runtime_error("failed to find associated analyzer");
        return pos->second.acquire();
    }

    void factory::release(const fs::path& file_path, shared_ptr analyzer)
    {
        lock_guard_t lock(map_mutex_);
        auto pos = map_.find(file_path.extension());
        if (pos != map_.end())
            return pos->second.release(analyzer);
    }
}
