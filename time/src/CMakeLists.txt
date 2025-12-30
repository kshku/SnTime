set(HEADERFILES
    platform.h
    defines.h
    sntime.h
)

set(SRCS
)

set(SPECIFIC_SRCS
    sntime.c
)

set(SUBDIRS
    win32
    nix
)

set(INCLUDE_BASE "${PROJECT_SOURCE_DIR}/time/include/sntime")

list(TRANSFORM HEADERFILES PREPEND "${INCLUDE_BASE}/")

target_sources(sntime PRIVATE ${HEADERFILES} ${SRCS})

foreach(dir IN LISTS SUBDIRS)
    foreach(src IN LISTS SPECIFIC_SRCS)
        target_sources(sntime PRIVATE "${dir}/${src}")
    endforeach()
endforeach()

