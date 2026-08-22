#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "klib::klib" for configuration ""
set_property(TARGET klib::klib APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(klib::klib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libklib.a"
  )

list(APPEND _cmake_import_check_targets klib::klib )
list(APPEND _cmake_import_check_files_for_klib::klib "${_IMPORT_PREFIX}/lib/libklib.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
