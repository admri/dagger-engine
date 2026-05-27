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
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_library(dagger STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
if(ipo_supported)
    set_target_properties(dagger PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(dagger PRIVATE external)

if(APPLE)
    target_link_libraries(dagger PRIVATE 
        "-framework Foundation"
        "-framework QuartzCore" 
        "-framework Metal"
    )
endif()

target_include_directories(dagger
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/include/dagger
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)