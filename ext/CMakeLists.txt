cmake_minimum_required(VERSION 3.26)
project(camlib)

set(CMAKE_CXX_STANDARD 20)

# Default to Release build if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Add standard Release flags for GCC/Clang if not already present
if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

set(CMAKE_FIND_FRAMEWORK NEVER)
find_package(Ruby REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

add_library(camlib MODULE camlib.cpp
        Webcam.cpp
        Webcam.hpp)
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
    pkg_check_modules(JEMALLOC IMPORTED_TARGET jemalloc) # jemalloc is now optional
    target_link_libraries(camlib PRIVATE PkgConfig::FFI)
    if(JEMALLOC_FOUND) # Link jemalloc only if found
        target_link_libraries(camlib PRIVATE PkgConfig::JEMALLOC)
    endif()
endif()

target_compile_definitions(camlib PRIVATE HAVE_LIBFFI)

target_include_directories(camlib PRIVATE ${Ruby_INCLUDE_DIR} ${Ruby_CONFIG_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
target_include_directories(camlib PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(camlib PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(camlib PRIVATE ${Ruby_LIBRARY} ${OpenCV_LIBS})

install(TARGETS camlib DESTINATION .)
