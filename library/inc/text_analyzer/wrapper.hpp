#pragma once
#ifndef PCH
    #include <text_analyzer/base.hpp>
#endif

namespace text_analyzer
{
    class pool;

    class wrapper
    {
    public:
        // Constructor: acquire the appropriate analyzer from the pool
        wrapper(pool& pool, const fs::path& file_path);

        // Destructor: release the analyzer back to the pool
        ~wrapper();

        // Overload operator() to behave like the wrapped analyzer
        result_unique_ptr operator()() const;

    private:
        shared_ptr analyzer_;
        fs::path file_path_;
        pool& pool_;
    };
}
