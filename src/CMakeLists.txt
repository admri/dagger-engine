cmake_minimum_required(VERSION 3.16)

set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(DELVE_SOURCES
    delve/Game.cpp
    delve/scenes/GameplayScene.cpp
    delve/systems/CombatSystem.cpp
    delve/systems/EnemyAISystem.cpp
    delve/systems/FOVSystem.cpp
    delve/systems/LightingRenderSystem.cpp
    delve/systems/MovementSystem.cpp
    delve/systems/PlayerInputSystem.cpp
    delve/systems/ShootingSystem.cpp
    delve/systems/TileMapRenderSystem.cpp
    delve/world/ChunkManager.cpp
    delve/world/TileMap.cpp
    delve/world/WorldGenerator.cpp
)

set(DELVE_HEADERS
    delve/Game.h
    delve/components/EnemyAIComponent.h
    delve/components/FOVComponent.h
    delve/components/HealthComponent.h
    delve/components/PlayerInputComponent.h
    delve/components/ProjectileComponent.h
    delve/components/WeaponComponent.h
    delve/scenes/GameOverScene.h
    delve/scenes/GameplayScene.h
    delve/scenes/MainMenuScene.h
    delve/scenes/PauseScene.h
    delve/systems/CombatSystem.h
    delve/systems/EnemyAISystem.h
    delve/systems/FOVSystem.h
    delve/systems/LightingRenderSystem.h
    delve/systems/MovementSystem.h
    delve/systems/PlayerInputSystem.h
    delve/systems/ShootingSystem.h
    delve/systems/TileMapRenderSystem.h
    delve/systems/UISystem.h
    delve/types/PlayerAnim.h
    delve/world/ChunkManager.h
    delve/world/TileMap.h
    delve/world/WorldGenerator.h
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${DELVE_SOURCES} ${DELVE_HEADERS})
if(ipo_supported)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Recompile shaders
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompileShaders)
setup_shader_compilation(${PROJECT_NAME})

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