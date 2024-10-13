#pragma once
#ifndef PCH
    #include <text_analyzer/task/base.hpp>
#endif

namespace text_analyzer::task
{
    // File-path-generator task to be used by a thread.
    class file_path_generator: public extended
    {
    public:
        file_path_generator(factory& factory, file_path_queue& file_paths, fs::path directory);
        file_path_generator(file_path_generator&& item);

        file_path_generator() = delete;
        file_path_generator(const file_path_generator&) = delete;

        void operator()() noexcept;

    private:
        void execute();

        fs::path directory_;
    };
}
