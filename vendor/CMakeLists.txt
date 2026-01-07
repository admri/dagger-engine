cmake_minimum_required(VERSION 3.16)

include(${CMAKE_CURRENT_LIST_DIR}/vendor_config.cmake)

if (USE_PREBUILT_VENDOR)

    message(STATUS "Using prebuilt vendor libraries")

    if (WIN32)
        set(VENDOR_PLATFORM windows)
        set(SHARED_EXT dll)
        set(IMPLIB_EXT lib)
    elseif(APPLE)
        set(VENDOR_PLATFORM macos)
        set(SHARED_EXT dylib)
    else()
        set(VENDOR_PLATFORM linux)
        set(SHARED_EXT so)
    endif()

    set(VENDOR_ROOT "${CMAKE_SOURCE_DIR}/vendor/prebuilt/${VENDOR_PLATFORM}")

    function(import_shared name)
        if (APPLE)
            set(lib_name "lib${name}")
        else()
            set(lib_name "${name}")
        endif()

        add_library(${name} SHARED IMPORTED)

        if(WIN32)
            set_target_properties(${name} PROPERTIES
                IMPORTED_LOCATION "${VENDOR_ROOT}/bin/${lib_name}.${SHARED_EXT}"  # DLL
                IMPORTED_IMPLIB "${VENDOR_ROOT}/lib/${lib_name}.${IMPLIB_EXT}"    # LIB
                INTERFACE_INCLUDE_DIRECTORIES "${VENDOR_ROOT}/include"
            )
        else()
            set_target_properties(${name} PROPERTIES
                IMPORTED_LOCATION "${VENDOR_ROOT}/lib/${lib_name}.${SHARED_EXT}"  # .so / .dylib
                INTERFACE_INCLUDE_DIRECTORIES "${VENDOR_ROOT}/include"
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

    message(STATUS "Building vendor libraries from source")

    add_subdirectory(SDL EXCLUDE_FROM_ALL)
    add_subdirectory(SDL_image EXCLUDE_FROM_ALL)
    add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)
    add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

endif()

# ---------------------------
# Combined vendor interface
# ---------------------------
add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
)