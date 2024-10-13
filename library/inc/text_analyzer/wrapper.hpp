#pragma once
#ifndef PCH
    #include <text_analyzer/base.hpp>
#endif

namespace text_analyzer
{
    class factory;

    class wrapper
    {
    public:
        // Constructor: acquire the appropriate analyzer from the factory
        wrapper(factory& factory, const fs::path& file_path);

        // Destructor: release the analyzer back to the factory
        ~wrapper();

        // Overload operator() to behave like the wrapped analyzer
        result_unique_ptr operator()() const;

    private:
        shared_ptr analyzer_;
        fs::path file_path_;
        factory& factory_;
    };
}
