// main.cpp
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include "io/csv_loader.hpp"
#include "io/json_loader.hpp"
#include "gui/plot_window.hpp"



int main(int argc, char** argv) {
    ImVec2 window_dims = ImVec2(1600, 900);
    ImVec2i window_pos;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "CSV Grapher", window_dims.x, window_dims.y, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_GetWindowPosition(window, &window_pos.x, &window_pos.y);

    auto configs = loadJSON("../config.json");
    std::map<std::string, CSVData> loadedCSVs;
    for (const auto& config : configs)
        loadedCSVs[config.csv_path] = loadCSV(config.csv_path);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(window_pos.y, window_pos.x), ImGuiCond_Always);
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();

        RenderPlots(configs, loadedCSVs);

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
