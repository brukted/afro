target_sources(
  afro
  PRIVATE assert.h
          log.h
          log.cpp
          paths.h
          paths.cpp
          translation.h
          translation.cpp
          preferences.h
          preferences.cpp
          embed_data.h
          math.h
          math.cpp)

configure_file(build_info.h.in build_info.h @ONLY)
