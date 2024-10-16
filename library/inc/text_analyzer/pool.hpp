#pragma once
#ifndef PCH
    #include <stdext/object_pool.hpp>
    #include <text_analyzer/base.hpp>
    #include <unordered_map>
#endif

namespace text_analyzer
{
    class pool
    {
    public:
        using pool_type = stdext::object_pool<base>;
        using map_type = std::unordered_map<extension_type, pool_type>;

        pool(map_type map);

        bool is_accepted(const fs::path& file_path) const noexcept;

        shared_ptr try_acquire(const fs::path& file_path);
        shared_ptr acquire(const fs::path& file_path);

        void release(const fs::path& file_path, shared_ptr analyzer);

    private:
        map_type map_;
        mutable std::mutex map_mutex_;
    };
}
