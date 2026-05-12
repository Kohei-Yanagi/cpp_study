cmake_minimum_required( VERSION 3.25 )

project( report001 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_executable( report001 src/main.cpp )
target_include_directories( report001 PRIVATE image )
target_compile_options( report001 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8> )
