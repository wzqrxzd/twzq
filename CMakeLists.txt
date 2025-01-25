cmake_minimum_required(VERSION 3.12)

project(twzq)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_executable(twzq
    main.cxx
    color.cxx
    speltnya_color_analyzer.cxx
    hyprland_controller.cxx
    waybar_controller.cxx
    wallpaper_controller.cxx
    wofi_controller.cxx
    config_manager.cxx
    wallpaper_manager.cxx
)

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(twzq PRIVATE spdlog fmt ${OpenCV_LIBS})
