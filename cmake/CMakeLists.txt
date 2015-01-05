cmake_minimum_required(VERSION 2.8)
project(sdl_rwops_util)

### Common stuff

get_filename_component(CMAKELISTDIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(ROOT "${CMAKELISTDIR}" PATH)

find_package(SDL REQUIRED)
find_package(GTest)

include_directories("${ROOT}" "${ROOT}/deps/vstring" "${SDL_INCLUDE_DIR}")

ADD_DEFINITIONS(-DSDL_RWOPS_UTIL_BUILDING)

### Static Library

add_library(sdl_rwops_util_static STATIC "${ROOT}/sdl_rwops_util.c")
set_target_properties(sdl_rwops_util_static PROPERTIES LINKER_LANGUAGE C)

install(FILES "${ROOT}/sdl_rwops_util.h" DESTINATION include)
install(TARGETS sdl_rwops_util_static DESTINATION lib)

### Dynamic library

add_library(sdl_rwops_util SHARED "${ROOT}/sdl_rwops_util.c")
set_target_properties(sdl_rwops_util PROPERTIES LINKER_LANGUAGE C)
if(MSVC)
  set_target_properties(sdl_rwops_util PROPERTIES COMPILE_FLAGS -DSDL_RWOPS_UTIL_DLL)
endif()
install(TARGETS sdl_rwops_util DESTINATION bin)

### Test runners

if(GTEST_FOUND)
  enable_testing()
  include_directories("${GTEST_INCLUDE_DIRS}")
  add_executable(sdl_rwops_util_test "${ROOT}/sdl_rwops_util_test.cpp")
  target_link_libraries(sdl_rwops_util_test sdl_rwops_util ${GTEST_BOTH_LIBRARIES} ${SDL_LIBRARY})
  add_test(run_test sdl_rwops_util_test)
else()
  message(STATUS "GoogleTest needed for testing")
endif()
