add_subdirectory(afro)
add_subdirectory(app)

if(AFRO_WITH_TESTS)
  add_subdirectory(tests)
endif()
