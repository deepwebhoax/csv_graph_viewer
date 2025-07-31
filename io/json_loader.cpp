// io/json_loader.cpp
#include "json_loader.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include "types.hpp"

using json = nlohmann::json;

std::vector<PlotConfig> loadJSON(const std::string& json_path) {
    std::ifstream f(json_path);
    if (!f.is_open()) {
        std::cerr << "Failed to open config file: " << json_path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    json j;
    try {
        f >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<PlotConfig> configs;
    for (const auto& plot : j["plots"]) {
        PlotConfig pc;
        pc.title = plot["title"];
        pc.x_label = plot["x_label"];
        pc.y_label = plot["y_label"];
        pc.metric = plot["metric"];
        pc.show_points = plot["show_points"];
        auto c = plot["line_color"];
        pc.line_color = ImVec4(
            static_cast<float>(c[0].get<int>())/255.0f, 
            static_cast<float>(c[1].get<int>())/255.0f, 
            static_cast<float>(c[2].get<int>())/255.0f, 1.0f);
        pc.csv_path = plot["csv_path"];
        configs.push_back(pc);
    }
    return configs;
}