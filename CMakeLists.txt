cmake_minimum_required(VERSION 3.16)
project(DungeonCrawler)

option(USE_PREBUILT_EXTERNAL "Use prebuilt external libraries" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

if (WIN32)
    set(EXTERNAL_PLATFORM windows)
    set(SHARED_EXT dll)
    set(IMPLIB_EXT lib)
elseif(APPLE)
    set(EXTERNAL_PLATFORM macos)
    set(SHARED_EXT dylib)
else()
    set(EXTERNAL_PLATFORM linux)
    set(SHARED_EXT so)
endif()

set(EXTERNAL_ROOT "${CMAKE_SOURCE_DIR}/external/prebuilt/${EXTERNAL_PLATFORM}" CACHE PATH "Root of prebuilt external libraries")

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory(external)
add_subdirectory(src/dagger)
add_subdirectory(src)