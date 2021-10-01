target_sources(afro PRIVATE log.h log.cpp paths.h paths.cpp translation.h
                            translation.cpp)

configure_file(build_info.h.in build_info.h @ONLY)
