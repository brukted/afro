find_package(fmt CONFIG REQUIRED)

add_executable(embed-data main.cpp)

target_link_libraries(embed-data PRIVATE fmt::fmt-header-only)

set_target_properties(embed-data PROPERTIES ${DEFAULT_PROJECT_OPTIONS})

function(embed_data_simple file_from list_to_add)

  # remove ../'s
  get_filename_component(_file_from ${CMAKE_CURRENT_SOURCE_DIR}/${file_from}
                         REALPATH)
  get_filename_component(_file_to ${CMAKE_CURRENT_BINARY_DIR}/${file_from}.cpp
                         REALPATH)

  list(APPEND ${list_to_add} ${_file_to})
  source_group(Generated FILES ${_file_to})
  set(${list_to_add}
      ${${list_to_add}}
      PARENT_SCOPE)

  get_filename_component(_file_to_path ${_file_to} PATH)

  add_custom_command(
    OUTPUT ${_file_to}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_file_to_path}
    COMMAND "$<TARGET_FILE:embed-data>" ${_file_from} ${_file_to} 0
    DEPENDS ${_file_from} embed-data)

  set_source_files_properties(${_file_to} PROPERTIES GENERATED TRUE)
endfunction()

function(embed_data_null_terminated file_from list_to_add)

  # remove ../'s
  get_filename_component(_file_from ${CMAKE_CURRENT_SOURCE_DIR}/${file_from}
                         REALPATH)
  get_filename_component(_file_to ${CMAKE_CURRENT_BINARY_DIR}/${file_from}.cpp
                         REALPATH)

  list(APPEND ${list_to_add} ${_file_to})
  source_group(Generated FILES ${_file_to})
  set(${list_to_add}
      ${${list_to_add}}
      PARENT_SCOPE)

  get_filename_component(_file_to_path ${_file_to} PATH)

  add_custom_command(
    OUTPUT ${_file_to}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_file_to_path}
    COMMAND "$<TARGET_FILE:embed-data>" ${_file_from} ${_file_to} 1}
    DEPENDS ${_file_from} embed-data)

  set_source_files_properties(${_file_to} PROPERTIES GENERATED TRUE)
endfunction()
