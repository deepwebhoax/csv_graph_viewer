// main.cpp
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <implot.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

using json = nlohmann::json;

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

CSVData loadCSV(const std::string& path) {
    CSVData data;
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> headers;

    if (!std::getline(file, line)) return data;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) headers.push_back(token);
    for (const auto& h : headers) data.metrics[h] = {};

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        int col = 0;
        while (std::getline(lineStream, token, ',')) {
            float val = std::stof(token);
            data.metrics[headers[col++]].push_back(val);
        }
    }
    data.time = data.metrics["time"];
    return data;
}

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

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CSV Grapher", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    auto configs = loadJSON("../config.json");
    std::map<std::string, CSVData> loadedCSVs;
    for (const auto& config : configs)
        loadedCSVs[config.csv_path] = loadCSV(config.csv_path);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_MOUSE_WHEEL) {
                SDL_Log("Mouse wheel event: x=%.2f, y=%.2f, window=%d", e.wheel.x, e.wheel.y, e.wheel.windowID);
                // if (ImGui_ImplSDL3_GetViewportForWindowID(e->wheel.windowID) != nullptr)
                // ImGui::LogText(std::to_string(e.wheel.windowID).c_str(), "Mouse wheel event: x=%.2f, y=%.2f", e.wheel.x, e.wheel.y);
                // io.event
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport());
        for (const auto& cfg : configs) {
            const auto& data = loadedCSVs[cfg.csv_path];
            if (ImPlot::BeginPlot(cfg.title.c_str())) {
                ImPlot::SetupAxes(cfg.x_label.c_str(), cfg.y_label.c_str());
                ImPlot::PushStyleColor(ImPlotCol_Line, cfg.line_color);
                
                auto it = data.metrics.find(cfg.metric);
                if (it != data.metrics.end()) {
                    ImPlot::PlotLine(cfg.metric.c_str(), data.time.data(),
                                    it->second.data(), data.time.size());
                } else {
                    std::cerr << "Metric '" << cfg.metric << "' not found in CSV.\n";
                }
                
                if (cfg.show_points) {
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
                    ImPlot::PlotScatter((cfg.metric + "_pts").c_str(),
                        data.time.data(), data.metrics.at(cfg.metric).data(), data.time.size());
                }
                ImPlot::PopStyleColor();
                ImPlot::EndPlot();
            }
        }

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImPlot::DestroyContext();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
