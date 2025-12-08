cmake_minimum_required(VERSION 3.26)
project(camlib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_FIND_FRAMEWORK NEVER)
find_package(Ruby REQUIRED)
find_package(PkgConfig REQUIRED)

add_library(camlib MODULE library.cpp)
set_target_properties(camlib PROPERTIES PREFIX "")
if(APPLE)
    set_target_properties(camlib PROPERTIES SUFFIX ".bundle")
endif()

# Dependencies
if(MSVC)
    find_package(unofficial-libffi CONFIG REQUIRED)
    target_link_libraries(camlib PRIVATE unofficial::libffi::libffi)
else()
    pkg_check_modules(FFI REQUIRED IMPORTED_TARGET libffi)
    pkg_check_modules(JEMALLOC REQUIRED IMPORTED_TARGET jemalloc)
    target_link_libraries(camlib PRIVATE PkgConfig::FFI PkgConfig::JEMALLOC)
endif()

target_compile_definitions(camlib PRIVATE HAVE_LIBFFI)

target_include_directories(camlib PRIVATE ${Ruby_INCLUDE_DIR} ${Ruby_CONFIG_INCLUDE_DIR})
target_include_directories(camlib PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(camlib PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(camlib PRIVATE ${Ruby_LIBRARY})

install(TARGETS camlib DESTINATION .)
