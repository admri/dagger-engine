set(DAGGER_SOURCES
    assets/AssetManager.cpp
    assets/Font.cpp
    audio/AudioSystem.cpp
    audio/Sound.cpp
    core/Application.cpp
    core/SceneManager.cpp
    core/Window.cpp
    core/systems/AnimationSystem.cpp
    core/systems/CameraSystem.cpp
    core/systems/CollisionSystem.cpp
    core/systems/RenderSystem.cpp
    graphics/Renderer.cpp
    graphics/Texture.cpp
    input/Input.cpp
)

if(APPLE)
    set(MAC_SOURCES
        core/graphics/MacHacks.mm
    )
    list(APPEND DAGGER_SOURCES ${MAC_SOURCES})
endif()

set(DAGGER_HEADERS
    ${PROJECT_SOURCE_DIR}/include/dagger/assets.h
    ${PROJECT_SOURCE_DIR}/include/dagger/audio.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core.h
    ${PROJECT_SOURCE_DIR}/include/dagger/dagger.h
    ${PROJECT_SOURCE_DIR}/include/dagger/graphics.h
    ${PROJECT_SOURCE_DIR}/include/dagger/input.h
    ${PROJECT_SOURCE_DIR}/include/dagger/main.h
    ${PROJECT_SOURCE_DIR}/include/dagger/assets/AssetManager.h
    ${PROJECT_SOURCE_DIR}/include/dagger/assets/AssetPreloader.h
    ${PROJECT_SOURCE_DIR}/include/dagger/assets/Font.h
    ${PROJECT_SOURCE_DIR}/include/dagger/assets/ResourceHandle.h
    ${PROJECT_SOURCE_DIR}/include/dagger/assets/ResourceManager.h
    ${PROJECT_SOURCE_DIR}/include/dagger/assets/ResourceTraits.h
    ${PROJECT_SOURCE_DIR}/include/dagger/audio/AudioSystem.h
    ${PROJECT_SOURCE_DIR}/include/dagger/audio/Sound.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/AppConfig.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/Application.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/Component.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/Constants.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/Entity.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/GameEvent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/Scene.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/SceneManager.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/AnimationComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/BoxColliderComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/CameraComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/CameraTargetComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/SpriteComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/TextComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/TransformComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/components/VelocityComponent.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/systems/AnimationSystem.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/systems/CameraSystem.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/systems/CollisionSystem.h
    ${PROJECT_SOURCE_DIR}/include/dagger/core/systems/RenderSystem.h
    ${PROJECT_SOURCE_DIR}/include/dagger/graphics/Color.h
    ${PROJECT_SOURCE_DIR}/include/dagger/graphics/Rect.h
    ${PROJECT_SOURCE_DIR}/include/dagger/graphics/Renderer.h
    ${PROJECT_SOURCE_DIR}/include/dagger/graphics/Texture.h
    ${PROJECT_SOURCE_DIR}/include/dagger/input/Input.h
    ${PROJECT_SOURCE_DIR}/include/dagger/input/KeyCodes.h
    core/Window.h
    graphics/MacHacks.h
)

add_library(dagger STATIC ${DAGGER_SOURCES} ${DAGGER_HEADERS})
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