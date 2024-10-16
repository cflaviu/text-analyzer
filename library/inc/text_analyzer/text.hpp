#pragma once
#ifndef PCH
    #include <text_analyzer/base.hpp>
#endif

namespace text_analyzer::text
{
    inline static const extension_type extension {".txt"};

    // Result of text file analysis
    class result: public text_analyzer::result
    {
    public:
        std::string to_string() const override { return "text analysis result"; }
    };

    // Base text file analyzer
    class base: public text_analyzer::base
    {
    public:
        using result_uptr = std::unique_ptr<result>;

        base(const std::size_t initial_file_content_capacity): text_analyzer::base(initial_file_content_capacity) {}

        const extension_type& extension() const noexcept override { return text::extension; }

    private:
    };

    // Dummy text file analyzer
    class dummy: public base
    {
    public:
        // Dummy result of text file analysis
        class result: public text_analyzer::result
        {
        public:
            std::string to_string() const override { return "dummy text analysis result"; }
        };

        dummy(const std::size_t initial_file_content_capacity): base(initial_file_content_capacity) {}

        result_unique_ptr operator()(const fs::path& /*file_path*/) const override;
    };
}
