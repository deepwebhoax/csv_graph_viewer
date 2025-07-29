cmake_minimum_required(VERSION 3.15)
project(CSVGraphViewer)

set(CMAKE_CXX_STANDARD 17)

add_executable(csv_graph_viewer main.cpp)

# Link SDL3, ImGui, ImPlot, and nlohmann_json
find_package(SDL3 REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(csv_graph_viewer PRIVATE SDL3::SDL3 nlohmann_json::nlohmann_json)

# You must add ImGui and ImPlot manually or via submodules
target_include_directories(csv_graph_viewer PRIVATE imgui implot)
target_sources(csv_graph_viewer PRIVATE
    imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_sdl3.cpp imgui/backends/imgui_impl_sdlrenderer3.cpp
    implot/implot.cpp implot/implot_items.cpp implot/implot_demo.cpp
)
