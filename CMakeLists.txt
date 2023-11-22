# CMakeList.txt : CMake project for CrossingRoad, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CrossingRoad")
set(SOURCES
    include/CrossingRoad.h
    src/CrossingRoad.cpp
    include/Texture.h
    src/Texture.cpp
    include/AnimatingObject.h
    src/AnimatingObject.cpp
    include/global.h
    src/global.cpp
    include/Character.h
    src/Character.cpp
    include/ResourceManager.h
    src/ResourceManager.cpp
)
set(SDL2_DIR
    include/vclib/sdl2/cmake
)
set(SDL2_image_DIR
    include/vclib/sdl2img/cmake
)
set(SDL2_ttf_DIR
    include/vclib/sdl2ttf/cmake
)
# Add source to this project's executab "src/ResourceManager.cpp"le.
add_executable (CrossingRoad ${SOURCES} "include/road.h" "src/road.cpp" "include/RandomLevelGenerator.h" "src/RandomLevelGenerator.cpp")
target_include_directories(CrossingRoad
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2::SDL2
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2::SDL2main
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2_image::SDL2_image
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2_ttf::SDL2_ttf
)
find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)
find_package(SDL2main)
target_link_libraries(CrossingRoad
    PRIVATE SDL2::SDL2
    PRIVATE SDL2::SDL2main
    PRIVATE SDL2_image::SDL2_image
    PRIVATE SDL2_ttf::SDL2_ttf
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CrossingRoad PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
