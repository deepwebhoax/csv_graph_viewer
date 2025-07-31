// include/types.hpp
#pragma once
#include <string>
#include <vector>
#include <map>
#include <imgui.h>

struct PlotConfig {
    std::string title;
    std::string x_label;
    std::string y_label;
    std::string metric;
    bool show_points;
    ImVec4 line_color;
    std::string csv_path;
};

struct CSVData {
    std::vector<float> time;
    std::map<std::string, std::vector<float>> metrics;
};
