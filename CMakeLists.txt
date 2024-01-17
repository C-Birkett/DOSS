cmake_minimum_required(VERSION 3.0.0)
project(DOSS
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd intellisense

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

add_library(doss_lib
    consts.h
    planets.h
    planets.cpp
)

target_link_libraries(doss_lib raylib)

# important names
set(APPNAME ${PROJECT_NAME})
add_executable(${APPNAME} ./main.cpp)

# raylib
target_link_libraries(${APPNAME} 
                        raylib
                        doss_lib)

#target_compile_features(${APPNAME} PRIVATE cxx_std_17)
