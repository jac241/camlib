cmake_minimum_required(VERSION 3.25)
project(camlib)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_FIND_FRAMEWORK NEVER)
find_package(Ruby "3.4")
execute_process(COMMAND brew --prefix jemalloc OUTPUT_VARIABLE JEMALLOC_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)

add_library(camlib MODULE library.cpp)
set_target_properties(camlib PROPERTIES PREFIX "")
if(APPLE)
    set_target_properties(camlib PROPERTIES SUFFIX ".bundle")
endif()

# FFI
if (MSVC) # This will include Clang on Windows
    find_package(unofficial-libffi CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::libffi::libffi)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_LIBFFI)
elseif (APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFI REQUIRED IMPORTED_TARGET libffi)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE PkgConfig::FFI)
else()
    find_library(FFI_LIBRARY NAMES ffi REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FFI_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_LIBFFI)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_LIBFFI)


target_include_directories(camlib PRIVATE ${Ruby_INCLUDE_DIR} ${Ruby_CONFIG_INCLUDE_DIR} ${JEMALLOC_PREFIX}/include)
target_include_directories(camlib PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(camlib PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(camlib PRIVATE ${Ruby_LIBRARY} ${JEMALLOC_LIBRARIES} ${JEMALLOC_PREFIX}/lib/libjemalloc.dylib)

install(TARGETS camlib DESTINATION .)
