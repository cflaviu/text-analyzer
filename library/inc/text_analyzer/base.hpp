#pragma once
#ifndef PCH
    #include <filesystem>
    #include <iosfwd>
    #include <memory>
    #include <string>
#endif

namespace text_analyzer
{
    namespace fs = ::std::filesystem;
    using extension_type = std::string;

    // Base result of analyzers
    class result
    {
    public:
        virtual ~result() = default;

        virtual std::string to_string() const = 0; // For displaying the result as a string
    };

    using result_unique_ptr = std::unique_ptr<result>;

    // Base class for analyzers
    class interface
    {
    public:
        virtual ~interface() = default;

        virtual const extension_type& extension() const noexcept = 0;

        virtual result_unique_ptr operator()(const fs::path& file_path) const = 0;
    };

    // Base class for analyzers
    class base: public interface
    {
    protected:
        base(const std::size_t initial_file_content_capacity);

        void load_file(const fs::path& file_path) const;

        mutable std::string file_content_;
    };

    using unique_ptr = std::unique_ptr<base>;
    using shared_ptr = std::shared_ptr<base>;

    std::osyncstream log();
}
