#
# External dependencies
#

#
# Sources
#

list(APPEND sources main.cpp)

#
# Create executable
#

# Build executable
add_executable(app ${sources})

#
# Project options
#

set_target_properties(
  app
  PROPERTIES DEBUG_POSTFIX "d"
             CXX_STANDARD 17
             LINKER_LANGUAGE "CXX"
             POSITION_INDEPENDENT_CODE ON
             CXX_VISIBILITY_PRESET "hidden"
             CXX_EXTENSIONS Off)

#
# Include directories
#

target_include_directories(
  app PRIVATE ${DEFAULT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR})

#
# Libraries
#

target_link_libraries(app PRIVATE ${DEFAULT_LIBRARIES} afro)

#
# Compile definitions
#

target_compile_definitions(app PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})

#
# Compile options
#

target_compile_options(app PRIVATE ${DEFAULT_COMPILE_OPTIONS})

#
# Linker options
#

target_link_libraries(app PRIVATE ${DEFAULT_LINKER_OPTIONS})

#
# Dependencies
#

add_dependencies(app afro)

#
# Target Health
#

perform_health_checks(app ${sources})
