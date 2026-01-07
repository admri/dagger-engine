# ---------------------------
# SDL3
# ---------------------------
set(SDL_TEST OFF CACHE BOOL "" FORCE)

# ---------------------------
# SDL_image
# ---------------------------
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AV1 OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_DAV1D OFF CACHE BOOL "" FORCE)

# ---------------------------
# SDL_ttf
# ---------------------------
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)

# ---------------------------
# SDL_mixer
# ---------------------------
set(SDLMIXER_VORBIS_STB OFF CACHE BOOL "" FORCE)
set(SDLMIXER_VORBIS_VORBISFILE OFF CACHE BOOL "" FORCE)
set(SDLMIXER_VORBIS_TREMOR OFF CACHE BOOL "" FORCE)

set(SDLMIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MP3 OFF CACHE BOOL "" FORCE)