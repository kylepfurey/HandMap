
# Initialize CMake
cmake_minimum_required(VERSION 4.3)
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (DEFINED ENV{HOME})
        set(CMAKE_TOOLCHAIN_FILE
                "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake"
                CACHE STRING "vcpkg toolchain file"
        )
    elseif (DEFINED ENV{USERPROFILE})
        set(CMAKE_TOOLCHAIN_FILE
                "$ENV{USERPROFILE}/vcpkg/scripts/buildsystems/vcpkg.cmake"
                CACHE STRING "vcpkg toolchain file"
        )
    endif ()
endif ()
project(HandMap CXX)

# Set C++ compiler version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

# Collect all sources (including main.cpp)
file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Build HandMap as a single executable
add_executable(HandMap ${SOURCE})

# Set include directory and build macros
target_include_directories(HandMap PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(HandMap PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
        SDL_MAIN_HANDLED
)

# Find external libraries
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_path(FAST_CPP_CSV_PARSER_INCLUDE_DIRS
        NAMES fast-cpp-csv-parser/csv.h
        REQUIRED
)
target_include_directories(HandMap PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${FAST_CPP_CSV_PARSER_INCLUDE_DIRS}
)

# Link external libraries
target_link_libraries(HandMap PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
)
