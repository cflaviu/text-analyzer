#pragma once
#ifndef PCH
    #include <filesystem>
#endif

namespace text_analyzer
{
    namespace fs = ::std::filesystem;
}

namespace text_analyzer::training
{
    class application
    {
    public:
        application();

        void run(const fs::path& directory);

    private:
    };
}
