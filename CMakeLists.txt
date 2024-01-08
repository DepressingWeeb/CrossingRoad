# CMakeList.txt : CMake project for CrossingRoad, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
 # cmake_policy(SET CMP0141 NEW)
  #set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()

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
    include/imgui/imconfig.h
    include/imgui/imgui-knobs.cpp
    include/imgui/imgui-knobs.h
    include/imgui/imgui.cpp
    include/imgui/imgui.h
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_impl_sdl2.cpp
    include/imgui/imgui_impl_sdl2.h
    include/imgui/imgui_impl_sdlrenderer.cpp
    include/imgui/imgui_impl_sdlrenderer.h
    include/imgui/imgui_internal.h
    include/imgui/imgui_stdlib.cpp
    include/imgui/imgui_stdlib.h
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
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

set(SDL2_mixer_DIR 
    include/vclib/sdl2mixer/cmake
)

set(IMGUI_DIR 
    include/imgui
)

# Add source to this project's executab "src/ResourceManager.cpp"le.
add_executable (CrossingRoad ${SOURCES} "include/road.h" "src/road.cpp" "include/RandomLevelGenerator.h" "src/RandomLevelGenerator.cpp" "include/Button.h" "src/Button.cpp" "include/Timer.h" "src/Timer.cpp" "include/Vehicle.h" "src/Vehicle.cpp" "include/EndlessLevelGenerator.h" "src/EndlessLevelGenerator.cpp" "src/StaticAnimatingObject.cpp" "include/Monster.h" "src/Monster.cpp" "include/Animal.h" "src/Animal.cpp" "include/ParallaxBackground.h" "src/ParallaxBackground.cpp" "include/LevelGenerator.h" "src/LevelGenerator.cpp")
target_include_directories(CrossingRoad
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2::SDL2
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2::SDL2main
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2_image::SDL2_image
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2_ttf::SDL2_ttf
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/vclib/SDL2_mixer::SDL2_mixer
)
find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)
find_package(SDL2_mixer)
find_package(SDL2main)
target_link_libraries(CrossingRoad
    PRIVATE SDL2::SDL2
    PRIVATE SDL2::SDL2main
    PRIVATE SDL2_image::SDL2_image
    PRIVATE SDL2_ttf::SDL2_ttf
    PRIVATE SDL2_mixer::SDL2_mixer
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CrossingRoad PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
