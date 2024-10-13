import qbs

CppApplication {
    Depends { name: "ta-library" }
    name: "ta-unit-tests"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    cpp.includePaths: ["inc", "inc_dep"]
    cpp.libraryPaths: []
    cpp.staticLibraries: [
        //"/usr/lib/libCatch2WithMain.a",
        //"asan",
    ]
    //cpp.cxxFlags: "-fsanitize=address"
    files: [
        "src/text_analyzer/test.cpp"
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
