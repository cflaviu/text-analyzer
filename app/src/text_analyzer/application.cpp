#include "text_analyzer/application.hpp"
#include "text_analyzer/pdf.hpp"
#include "text_analyzer/text.hpp"
#include <iostream>

namespace text_analyzer
{
    application::application(const config& config):
        factory_(
            config.analyzer_thread_count, [&] { return std::make_shared<text::dummy>(config.initial_text_file_content_capacity); },
            [&] { return std::make_shared<pdf::dummy>(config.initial_pdf_content_capacity); }),
        file_paths_(config.file_path_queue_capacity),
        results_(config.result_queue_capacity),
        analyzer_(std::ref(factory_), std::ref(file_paths_), std::ref(results_)),
        saver_(results_)
    {
        analyzer_threads_.reserve(config.analyzer_thread_count);
    }

    void application::wait_execution()
    {
        file_path_generator_thread_.join();
        for (auto& thread: analyzer_threads_)
        {
            thread.join();
        }

        saver_thread_.join();
        analyzer_threads_.clear();
    }

    void application::run(const fs::path& directory)
    {
        log() << "start using " << analyzer_threads_.capacity() << " threads\n";

        task::file_path_generator file_path_generator(factory_, file_paths_, directory);
        file_path_generator_thread_ = std::thread(std::ref(file_path_generator));

        for (auto count = analyzer_threads_.capacity(); count--;)
        {
            analyzer_threads_.emplace_back(std::ref(analyzer_));
        }

        saver_thread_ = std::thread(std::ref(saver_));
        wait_execution();

        log() << "finished\nitems generated: " << file_path_generator.item_count() << "\nitems processed: " << analyzer_.item_count()
              << "\nitems saved: " << saver_.item_count() << '\n';
    }
}
