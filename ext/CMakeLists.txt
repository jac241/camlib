cmake_minimum_required(VERSION 3.25)
project(rice-test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_FIND_FRAMEWORK NEVER)
find_package(Ruby "3.4")
execute_process(COMMAND brew --prefix jemalloc OUTPUT_VARIABLE JEMALLOC_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    add_compile_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
    add_compile_options(/bigobj /utf-8)
    # The default of /EHsc crashes Ruby when calling longjmp with optimizations on (/O2)
    string(REGEX REPLACE "/EHsc" "/EHs" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-ftemplate-backtrace-limit=0)
    # https://github.com/doxygen/doxygen/issues/9269#issuecomment-1094975328
    add_compile_options(unittest PRIVATE -Wa,-mbig-obj)
endif ()

add_library(rice-test SHARED library.cpp)

target_include_directories(rice-test PRIVATE ${Ruby_INCLUDE_DIR} ${Ruby_CONFIG_INCLUDE_DIR} ${JEMALLOC_PREFIX}/include)
target_include_directories(rice-test PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(rice-test PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(rice-test ${Ruby_LIBRARY} ${JEMALLOC_LIBRARIES} ${JEMALLOC_PREFIX}/lib/libjemalloc.dylib)
