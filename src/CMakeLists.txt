cmake_minimum_required(VERSION 3.16)

set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

file(GLOB_RECURSE GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/delve/*.cpp
)

file(GLOB_RECURSE GAME_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/delve/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/delve/*.hpp
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${GAME_SOURCES} ${GAME_HEADERS})
if(ipo_supported)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    dagger
    external
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/delve
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