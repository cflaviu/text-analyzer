#pragma once
#ifndef PCH
    #include <functional>
    #include <stdext/object_pool.hpp>
    #include <text_analyzer/base.hpp>
    #include <unordered_map>
#endif

namespace text_analyzer
{
    class factory
    {
    public:
        using creation_funcion = std::function<shared_ptr()>;

        factory(size_t pool_size, creation_funcion text_analyzer_creator, creation_funcion pdf_analyzer_creator);

        bool is_accepted(const fs::path& file_path) const noexcept;

        shared_ptr try_acquire(const fs::path& file_path);
        shared_ptr acquire(const fs::path& file_path);

        void release(const fs::path& file_path, shared_ptr analyzer);

    private:
        using pool_type = stdext::object_pool<base>;
        using map_type = std::unordered_map<extension_type, pool_type>;

        map_type map_;
        mutable std::mutex map_mutex_;
    };
}
