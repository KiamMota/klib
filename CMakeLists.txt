cmake_minimum_required(VERSION 3.16)
project(klib LANGUAGES C)

set(SRC
  kfile.c
  kprint.c
  kstring.c
  os.c
)

add_library(klib STATIC ${SRC})

# Define os arquivos de cabeçalho (.h) que precisam ser públicos
set_target_properties(klib PROPERTIES
  PUBLIC_HEADER "ktypes.h;kmalloc.h;kfile.h;kprint.h;kstring.h;os.h" # Substitua pelos nomes reais dos seus .h
)

target_include_directories(klib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)

install(TARGETS klib
  EXPORT klibTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}      # Geralmente /usr/local/lib
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/klib # Geralmente /usr/local/include/klib
)

install(EXPORT klibTargets
  FILE klibTargets.cmake
  NAMESPACE klib::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/klib
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/klibConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/klibConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/klib
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/klibConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/klib
)
