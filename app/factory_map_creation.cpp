#include <text_analyzer/docx.hpp>
#include <text_analyzer/pool.hpp>
#include <text_analyzer/json.hpp>
#include <text_analyzer/pdf.hpp>
#include <text_analyzer/xml.hpp>

namespace text_analyzer
{
    pool::map_type create_factory_map(const std::size_t pool_size)
    {
        pool::map_type m;
        m[docx::extension] = {pool_size, [] { return std::make_shared<docx::dummy>(20u * 1024u); }};
        m[json::extension] = {pool_size, [] { return std::make_shared<json::dummy>(40u * 1024u); }};
        m[pdf::extension] = {pool_size, [] { return std::make_shared<pdf::dummy>(100u * 1024u); }};
        m[xml::extension] = {pool_size, [] { return std::make_shared<xml::dummy>(60u * 1024u); }};

        return m;
    }
}
