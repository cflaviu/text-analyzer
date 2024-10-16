#include "text_analyzer/json.hpp"

namespace text_analyzer::json
{
    result_unique_ptr dummy::operator()(const fs::path& file_path) const
    {
        load_file(file_path);
        return std::make_unique<result>();
    }
}
