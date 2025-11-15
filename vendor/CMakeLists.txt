cmake_minimum_required(VERSION 3.10)

# ---------------------------
# SDL3
# ---------------------------
set(SDL_TEST OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

# ---------------------------
# SDL_image #
# ---------------------------
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AV1 OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_DAV1D OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# ---------------------------
# SDL_ttf
# ---------------------------
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)

add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)

# ---------------------------
# SDL_mixer
# ---------------------------
set(SDLMIXER_VORBIS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MP3 OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

# ---------------------------
# Combined vendor library
# ---------------------------
add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
)