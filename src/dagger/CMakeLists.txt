file(GLOB_RECURSE ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

if(APPLE)
    file(GLOB_RECURSE MAC_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.mm
    )
    list(APPEND ENGINE_SOURCES ${MAC_SOURCES})
endif()

file(GLOB_RECURSE ENGINE_HEADERS
    ${PROJECT_SOURCE_DIR}/include/dagger/*.h
    ${PROJECT_SOURCE_DIR}/include/dagger/*.hpp
)

add_library(dagger STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_link_libraries(dagger PUBLIC external)

if(APPLE)
    target_link_libraries(dagger PUBLIC 
        "-framework Foundation"
        "-framework QuartzCore" 
        "-framework Metal"
    )
endif()

target_include_directories(dagger PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/dagger
)