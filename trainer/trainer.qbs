CppApplication {
    name: "ta-trainer"
    Depends { name: 'ta-library' }
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    cpp.includePaths: ["inc", "inc_dep"]
    install: true
    files: [
        "inc/text_analyzer/training/application.hpp",
        "main.cpp",
        "src/text_analyzer/training/application.cpp",
    ]
}
