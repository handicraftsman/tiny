cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Tiny VERSION 0.3.3 LANGUAGES C)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX /usr/ CACHE PATH "Default install prefix" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_C_STANDARD 11)

file(GLOB t_SRC tiny/*.c tiny-containers/*.c)
add_library(tiny SHARED ${t_SRC})
target_include_directories(tiny PRIVATE .)

add_executable(tiny-test test.c)
target_include_directories(tiny-test PRIVATE .)
target_link_libraries(tiny-test tiny)

configure_file(${CMAKE_SOURCE_DIR}/tiny.pc.in ${CMAKE_BINARY_DIR}/tiny.pc)

install(FILES ${CMAKE_BINARY_DIR}/tiny.pc DESTINATION lib/pkgconfig)

install(
  TARGETS tiny
  LIBRARY DESTINATION lib
)

install(FILES tiny.h DESTINATION include/tiny)
file(GLOB t_HDR tiny/*.h)
install(FILES ${t_HDR} DESTINATION include/tiny/tiny)
file(GLOB tc_HDR tiny-containers/*.h)
install(FILES ${tc_HDR} DESTINATION include/tiny/tiny-containers)