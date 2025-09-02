cmake_minimum_required(VERSION 3.14)

project(VMTranslator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(JackCompiler
    src/CompilationEngine.cpp
    src/CompilerResources.cpp
    src/JackCompiler.cpp
    src/JackTokenizer.cpp
    src/main.cpp
    src/SymbolTable.cpp
    src/utils.cpp
    src/VMWriter.cpp
)

set_target_properties(JackCompiler PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
