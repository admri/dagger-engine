cmake_minimum_required(VERSION 3.8)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL project (vendored or system)
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Collect source files
add_executable(DungeonCrawler
  main.cpp
)

# Link SDL3
target_link_libraries(DungeonCrawler PRIVATE SDL3::SDL3)

# Include headers
target_include_directories(DungeonCrawler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Copy SDL DLL next to the executable (Windows only)
if (WIN32)
    add_custom_command(TARGET DungeonCrawler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:DungeonCrawler>)
endif()