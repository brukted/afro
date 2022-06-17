#
# External dependencies
#

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glbinding CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imnodes CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)

#
# Resources
#

embed_data_simple("../../resources/fa-solid-900.ttf" rc)
embed_data_simple("../../resources/droidsans.ttf" rc)
embed_data_text("../../resources/shaders/material/mat_vertex.vert" rc)
embed_data_text("../../resources/shaders/material/uniform_color.frag" rc)
embed_data_text("../../resources/shaders/material/blend.frag" rc)
embed_data_text("../../resources/shaders/material/blur.frag" rc)
embed_data_text("../../resources/shaders/material/channel_select.frag" rc)

#
# Sources
#

#
# Create library
#

# Build library
add_library(afro STATIC ${rc})

#
# Project options
#

set_target_properties(afro PROPERTIES ${DEFAULT_PROJECT_OPTIONS})

# Sub directories

add_subdirectory(core)
add_subdirectory(io)
add_subdirectory(render)
add_subdirectory(ui)
add_subdirectory(undo)
add_subdirectory(utils)

#
# Include directories
#

target_include_directories(
  afro PRIVATE ${DEFAULT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_BINARY_DIR}
               ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(afro PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
# Libraries
#

target_link_libraries(
  afro
  PUBLIC ${DEFAULT_LIBRARIES}
         spdlog::spdlog_header_only
         fmt::fmt-header-only
         glbinding::glbinding
         glfw
         imgui::imgui
         unofficial::imnodes::imnodes
         OpenImageIO::OpenImageIO
         OpenImageIO::OpenImageIO_Util
         unofficial::nativefiledialog::nfd
         glm::glm
         cereal::cereal)

#
# Compile definitions
#

target_compile_definitions(afro PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})

#
# Compile options
#

target_compile_options(afro PRIVATE ${DEFAULT_COMPILE_OPTIONS})

#
# Linker options
#

target_link_libraries(afro PRIVATE ${DEFAULT_LINKER_OPTIONS})

#
# Dependencies
#

add_dependencies(afro embed-data)

#
# Target Health
#

get_target_property(sources afro SOURCES)
perform_health_checks(afro ${sources})
