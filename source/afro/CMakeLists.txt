#
# External dependencies
#

#
# Sources
#

list(APPEND sources)

#
# Create library
#

# Build library
add_library(afro OBJECT ${sources})

#
# Project options
#

set_target_properties(
  afro
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
  afro PRIVATE ${DEFAULT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_BINARY_DIR}
               ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/source/include)

target_include_directories(afro PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#
# Libraries
#

target_link_libraries(afro PRIVATE ${DEFAULT_LIBRARIES})

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

# add_dependencies(afro )

#
# Target Health
#

perform_health_checks(afro ${sources})
