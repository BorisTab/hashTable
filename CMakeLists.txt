cmake_minimum_required(VERSION 3.16)
project(hashTable)

enable_language(CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set_source_files_properties(hash.asm PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
set(CMAKE_CXX_FLAGS "-O0")

add_executable(hashTable main.cpp list.h fileRead.h hash.asm)