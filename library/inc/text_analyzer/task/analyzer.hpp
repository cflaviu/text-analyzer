#pragma once
#ifndef PCH
    #include <text_analyzer/task/base.hpp>
#endif

namespace text_analyzer::task
{
    // Analyzer task to be used by threads.
    class analyzer: public extended
    {
    public:
        analyzer(factory& factory, file_path_queue& file_paths, result_queue& results);
        analyzer(analyzer&& item);

        analyzer() = delete;
        analyzer(const analyzer&) = delete;

        void operator()() noexcept;

        analyzer& operator=(analyzer&&) = default;

    private:
        bool execute();
        result_unique_ptr analyze_file(const fs::path& file_path) const;

        result_queue& results_;
    };
}
