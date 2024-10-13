#include "text_analyzer/wrapper.hpp"
#include "text_analyzer/factory.hpp"

namespace text_analyzer
{
    wrapper::wrapper(factory& factory, const fs::path& file_path):
        analyzer_ {factory.acquire(file_path)},
        file_path_(file_path),
        factory_(factory)
    {
    }

    wrapper::~wrapper()
    {
        try
        {
            factory_.release(file_path_, analyzer_);
        }
        catch (...)
        {
        }
    }

    result_unique_ptr wrapper::operator()() const
    {
        return (*analyzer_)(file_path_);
    }
}
