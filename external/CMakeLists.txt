cmake_minimum_required(VERSION 3.16)

include(${CMAKE_CURRENT_LIST_DIR}/external_config.cmake)

if (USE_PREBUILT_EXTERNAL)

    message(STATUS "Using prebuilt external libraries")

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

    set(EXTERNAL_ROOT "${CMAKE_SOURCE_DIR}/external/prebuilt/${EXTERNAL_PLATFORM}")

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

    message(STATUS "Building external libraries from source")

    add_subdirectory(SDL EXCLUDE_FROM_ALL)
    add_subdirectory(SDL_image EXCLUDE_FROM_ALL)
    add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)
    add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

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
)