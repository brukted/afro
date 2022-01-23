add_subdirectory(editors)

target_sources(
  afro
  PUBLIC ui_main.h
         ui_main.cpp
         ui_window.h
         ui_window.cpp
         icons.h
         icons.cpp
         icons_font_awesome_5.h
         debug_ui.h
         debug_ui.cpp
         ui_utils.h
         ui_utils.cpp
         theme.h
         theme.cpp)
