cmake_minimum_required(VERSION 2.8)
project(libcaption)
# set(CMAKE_C_FLAGS "-Wall -O3 ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")
add_definitions(-D__STDC_CONSTANT_MACROS)
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# If CMAKE_BUILD_TYPE is not specified, then default to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# Don't need to prefix local includes with "caption/*"
include_directories(${PROJECT_SOURCE_DIR}/caption)


set(eia608_from_utf8_c ${PROJECT_SOURCE_DIR}/src/eia608_from_utf8.c)
set(eia608_from_utf8_re2c_c ${PROJECT_SOURCE_DIR}/src/eia608_from_utf8.re2c.c)
set(eia608_from_utf8_cached_c ${PROJECT_SOURCE_DIR}/src/eia608_from_utf8.cached.c)
set(srt_c ${PROJECT_SOURCE_DIR}/src/srt.c)
set(srt_re2c_c ${PROJECT_SOURCE_DIR}/src/srt.re2c.c)
set(srt_cached_c ${PROJECT_SOURCE_DIR}/src/srt.cached.c)

find_program(RE2C re2c)
if(NOT RE2C)
  message(STATUS "re2c not found. Using cached version of generated files.")
  configure_file(${srt_cached_c} ${srt_c} COPYONLY)
  configure_file(${eia608_from_utf8_c_cached} ${eia608_from_utf8_c} COPYONLY)
else()
  add_custom_command(OUTPUT ${srt_c}
    COMMAND ${RE2C} -bis -o ${srt_c} ${srt_re2c_c}
    DEPENDS ${srt_re2c_c}
    COMMENT "Generating ${srt_c}")

    add_custom_command(OUTPUT ${eia608_from_utf8_c}
    COMMAND ${RE2C} -bis -o ${eia608_from_utf8_c} ${eia608_from_utf8_re2c_c}
    DEPENDS ${eia608_from_utf8_re2c_c}
    COMMENT "Generating ${eia608_from_utf8_c}")
endif()


set(CAPTION_SOURCES
  ${srt_c}
  ${eia608_from_utf8_c}
  src/caption.c
  src/cea708.c
  src/eia608.c
  src/eia608_charmap.c
  src/types.c
  src/mpeg.c
  src/scc.c
  src/utf8.c
  src/vtt.c
  src/xds.c
)

set(CAPTION_HEADERS
  caption/types.h
  caption/caption.h
  caption/cea708.h
  caption/eia608.h
  caption/eia608_charmap.h
  caption/mpeg.h
  caption/scc.h
  caption/srt.h
  caption/utf8.h
  caption/vtt.h
  caption/xds.h
)

add_library(caption ${CAPTION_SOURCES})

if(CMAKE_VERSION VERSION_EQUAL 2.8.12 OR CMAKE_VERSION VERSION_GREATER 2.8.12)
target_include_directories(caption PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()

option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# unit-tests
#add_executable(eia608_test unit_tests/eia608_test.c )
#target_link_libraries(eia608_test caption)


# Tests - TODO autro run these
#add_executable(test_captions unit_tests/test_sei.c )
#target_link_libraries(test_captions caption)

#add_executable(test_wrap unit_tests/test_wrap.c )
#target_link_libraries(test_wrap caption)

#add_executable(test_captions unit_tests/vector.cpp)
#target_link_libraries(test_captions caption)


install (TARGETS caption DESTINATION lib EXPORT caption-targets)
install (FILES ${CAPTION_HEADERS} DESTINATION include/caption)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
