# ------------------------------------
cmake_minimum_required(VERSION 3.15)

project(SurgeReflect LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# ------------------------------------

file(GLOB_RECURSE SOURCE_FILES
    SurgeReflect/*.cpp
    SurgeReflect/*.hpp 
)

set(INCLUDE_DIRS
    SurgeReflect/Source
    SurgeReflect/Include
)

set(LIB_LINKS)

add_executable(SurgeReflect ${SOURCE_FILES})
target_include_directories(SurgeReflect PUBLIC ${INCLUDE_DIRS})
target_link_libraries(SurgeReflect PUBLIC ${LIB_LINKS})
