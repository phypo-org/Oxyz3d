cmake_minimum_required(VERSION 3.19)


project(Oxyz3d LANGUAGES CXX)

# --- C++ ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# --- Options équivalentes à ton Makefile ---
add_compile_definitions(
  _REENTRANT
  _THREAD_SAFE
  _POSIX_PTHREAD_SEMANTICS
  _LARGEFILE_SOURCE
  _LARGEFILE64_SOURCE
  USING_LUA
  USING_PYTHON
)

# --- Sources ---
file(GLOB_RECURSE OXYZ3D_SOURCES
     src/*.cpp)

add_executable(Oxyz3d ${OXYZ3D_SOURCES})

# =========================================================
# FLTK (via fltk-config, comme ton Makefile le fait implicitement)
# =========================================================

find_program(FLTK_CONFIG fltk-config REQUIRED)

execute_process(
  COMMAND ${FLTK_CONFIG} --use-gl --use-images --cxxflags
  OUTPUT_VARIABLE FLTK_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${FLTK_CONFIG} --use-gl --use-images --ldflags
  OUTPUT_VARIABLE FLTK_LDFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

separate_arguments(FLTK_CXXFLAGS)
separate_arguments(FLTK_LDFLAGS)

target_compile_options(Oxyz3d PRIVATE ${FLTK_CXXFLAGS})
target_link_options(Oxyz3d PRIVATE ${FLTK_LDFLAGS})

# =========================================================
# OpenGL
# =========================================================

find_package(OpenGL REQUIRED)
target_link_libraries(Oxyz3d PRIVATE OpenGL::GL OpenGL::GLU)

# =========================================================
# Lua 5.4
# =========================================================

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)

target_include_directories(Oxyz3d PRIVATE ${LUA_INCLUDE_DIRS})
target_link_libraries(Oxyz3d PRIVATE ${LUA_LIBRARIES})

# =========================================================
# Python
# =========================================================

find_package(Python3 3.12 REQUIRED COMPONENTS Interpreter Development)

target_include_directories(Oxyz3d PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(Oxyz3d PRIVATE ${Python3_LIBRARIES})

# =========================================================
# Threads / dl / m (équivalent Makefile)
# =========================================================

target_link_libraries(Oxyz3d PRIVATE pthread dl m)
