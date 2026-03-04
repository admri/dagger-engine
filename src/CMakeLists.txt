cmake_minimum_required(VERSION 3.16)

set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

file(GLOB_RECURSE GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp
)

file(GLOB_RECURSE GAME_HEADERS
    ${PROJECT_SOURCE_DIR}/include/game/*.h
    ${PROJECT_SOURCE_DIR}/include/game/*.hpp
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${GAME_SOURCES} ${GAME_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    engine
    external
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include/game
)

add_custom_command(TARGET DungeonCrawler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:DungeonCrawler>/assets
)

if (USE_PREBUILT_EXTERNAL)
    if(WIN32)
        file(GLOB PREBUILT_LIBS "${EXTERNAL_ROOT}/bin/*.dll")
    elseif(APPLE)
        file(GLOB PREBUILT_LIBS "${EXTERNAL_ROOT}/lib/*.dylib")
    else() # Linux
        file(GLOB PREBUILT_LIBS "${EXTERNAL_ROOT}/lib/lib*.so*")
    endif()

    foreach(lib ${PREBUILT_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${lib}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        )
    endforeach()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE $<$<CONFIG:Release>:YES>
)