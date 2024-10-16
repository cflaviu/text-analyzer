#pragma once
#ifndef PCH
    #include <filesystem>
    #include <text_analyzer/pool.hpp>
    #include <text_analyzer/task/analyzer.hpp>
    #include <text_analyzer/task/file_path_generator.hpp>
    #include <text_analyzer/task/saver.hpp>
#endif

namespace text_analyzer
{
    namespace fs = ::std::filesystem;

    class application
    {
    public:
        struct config
        {
            std::size_t file_path_queue_capacity = 1000u;
            std::size_t result_queue_capacity = 1000u;
            std::uint8_t analyzer_thread_count = 4u;
        };

        application(const config& config, pool::map_type factory_map);

        void run(const fs::path& directory);

    private:
        void wait_execution();

        pool pool_;
        task::file_path_queue file_paths_;
        task::result_queue results_;
        task::analyzer analyzer_;
        task::saver saver_;
        std::thread file_path_generator_thread_;
        std::thread saver_thread_;
        std::vector<std::thread> analyzer_threads_;
    };
}
