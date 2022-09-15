include_directories(FileData Parser)
file(GLOB SOURCES "./*/*.cpp")
add_library(ParserLib ${SOURCES})