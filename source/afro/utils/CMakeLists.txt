target_sources(
  afro
  PRIVATE asset.h
          log.h
          log.cpp
          paths.h
          paths.cpp
          translation.h
          translation.cpp
          preferences.h
          preferences.cpp
          embed_data.h)

configure_file(build_info.h.in build_info.h @ONLY)
