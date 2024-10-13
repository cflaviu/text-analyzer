#include "text_analyzer/base.hpp"
#include <fstream>

namespace text_analyzer
{
    base::base(const std::size_t initial_file_content_capacity)
    {
        file_content_.reserve(initial_file_content_capacity);
    }

    void base::load_file(const fs::path& file_path) const
    {
        std::ifstream file(file_path);

        file.seekg(0, std::ios::end);
        const auto size = static_cast<std::size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        if (size > file_content_.capacity())
            file_content_.reserve(size);

        file_content_.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
}
