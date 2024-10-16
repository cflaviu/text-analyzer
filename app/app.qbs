CppApplication {
    name: "ta-app"
    Depends { name: 'ta-library' }
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    //cpp.cxxFlags: "-fsanitize=address"
    //cpp.staticLibraries: "asan"
    cpp.includePaths: ["inc", "inc_dep"]
    install: true
    files: [
        "factory_map_creation.cpp",
        "main.cpp",
        "inc/text_analyzer/application.hpp",
        "src/text_analyzer/application.cpp",
    ]
}
