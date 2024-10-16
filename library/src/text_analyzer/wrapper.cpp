#include "text_analyzer/wrapper.hpp"
#include "text_analyzer/pool.hpp"

namespace text_analyzer
{
    wrapper::wrapper(pool& pool, const fs::path& file_path):
        analyzer_ {pool.acquire(file_path.extension())},
        file_path_(file_path),
        pool_(pool)
    {
    }

    wrapper::~wrapper()
    {
        try
        {
            pool_.release(file_path_, analyzer_);
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
