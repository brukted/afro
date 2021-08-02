target_sources(afro PRIVATE log.h log.cpp paths.h paths.cpp)

configure_file(build_info.h.in build_info.h @ONLY)
