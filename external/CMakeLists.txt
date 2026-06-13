cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
include(${CMAKE_CURRENT_LIST_DIR}/external_config.cmake)

if (USE_PREBUILT_EXTERNAL)

    message(STATUS "Using prebuilt external libraries")

    function(import_shared name)
        add_library(${name} SHARED IMPORTED)

        if(WIN32)
            set_target_properties(${name} PROPERTIES
                IMPORTED_LOCATION "${EXTERNAL_ROOT}/bin/${name}.${SHARED_EXT}"  # DLL
                IMPORTED_IMPLIB "${EXTERNAL_ROOT}/lib/${name}.${IMPLIB_EXT}"    # LIB
                INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_ROOT}/include"
            )
        else()
            set_target_properties(${name} PROPERTIES
                IMPORTED_LOCATION "${EXTERNAL_ROOT}/lib/lib${name}.${SHARED_EXT}"  # .so / .dylib
                INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_ROOT}/include"
            )
        endif()
    endfunction()

    import_shared(SDL3)
    import_shared(SDL3_image)
    import_shared(SDL3_ttf)
    import_shared(SDL3_mixer)

    add_library(SDL3::SDL3 ALIAS SDL3)
    add_library(SDL3_image::SDL3_image ALIAS SDL3_image)
    add_library(SDL3_ttf::SDL3_ttf ALIAS SDL3_ttf)
    add_library(SDL3_mixer::SDL3_mixer ALIAS SDL3_mixer)

else()

    message(STATUS "Fetching external libraries from GitHub...")

    # SDL
    message(STATUS "Downloading and configuring SDL3...")
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.8
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)

    # SDL_image
    FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.4.4
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3_image)

    # SDL_ttf
    message(STATUS "Downloading and configuring SDL3_ttf...")
    FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-3.2.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3_ttf)

    # SDL_mixer
    message(STATUS "Downloading and configuring SDL3_mixer...")
    FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-3.2.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3_mixer)

    # nlohmann/json
    message(STATUS "Downloading and configuring nlohmann_json...")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(nlohmann_json)

endif()

# ---------------------------
# Combined external interface
# ---------------------------
add_library(external INTERFACE)
target_link_libraries(external INTERFACE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
    nlohmann_json::nlohmann_json
)