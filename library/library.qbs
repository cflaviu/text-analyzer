import qbs

StaticLibrary {
    name: "ta-library"
    Depends { name: 'cpp' }
    cpp.cxxLanguageVersion: "c++23"
    //cpp.enableRtti: false
    cpp.includePaths: ["inc", "inc_dep"]
    //cpp.cxxFlags: "-fsanitize=address"
    //cpp.staticLibraries: "asan"
    files: [
        "inc/stdext/object_pool.hpp",
        "inc/stdext/producer_consumer_queue.hpp",
        "inc/text_analyzer/base.hpp",
        "inc/text_analyzer/factory.hpp",
        "inc/text_analyzer/pdf.hpp",
        "inc/text_analyzer/task/analyzer.hpp",
        "inc/text_analyzer/task/base.hpp",
        "inc/text_analyzer/task/file_path_generator.hpp",
        "inc/text_analyzer/task/saver.hpp",
        "inc/text_analyzer/text.hpp",
        "inc/text_analyzer/wrapper.hpp",
        "src/text_analyzer/base.cpp",
        "src/text_analyzer/factory.cpp",
        "src/text_analyzer/pdf.cpp",
        "src/text_analyzer/text.cpp",
        "src/text_analyzer/task/analyzer.cpp",
        "src/text_analyzer/task/file_path_generator.cpp",
        "src/text_analyzer/task/saver.cpp",
        "src/text_analyzer/wrapper.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
