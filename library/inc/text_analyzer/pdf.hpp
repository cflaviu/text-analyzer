#pragma once
#ifndef PCH
    #include <text_analyzer/base.hpp>
#endif

namespace text_analyzer::pdf
{
    // Result of PDF analysis
    class result: public text_analyzer::result
    {
    public:
        std::string to_string() const override { return "pdf analysis result"; }
    };

    // Base PDF analyzer
    class base: public text_analyzer::base
    {
    public:
        using result_uptr = std::unique_ptr<result>;

        base(const std::size_t initial_file_content_capacity): text_analyzer::base(initial_file_content_capacity) {}

        inline static const extension_type ext {".pdf"};

        const extension_type& extension() const noexcept override { return ext; }
    };

    // Dummy PDF analyzer
    class dummy: public base
    {
    public:
        // Dummy result of PDF analysis
        class result: public pdf::result
        {
        public:
            std::string to_string() const override { return "dummy pdf analysis result"; }
        };

        dummy(const std::size_t initial_file_content_capacity): base(initial_file_content_capacity) {}

        result_unique_ptr operator()(const fs::path& /*file_path*/) const override;
    };
}
