import qbs 1.0

Project {
    name: "text-analyzer"
    references: [
        "app/app.qbs",
        "library/library.qbs",
        "trainer/trainer.qbs",
        "test/test.qbs",
    ]
}

