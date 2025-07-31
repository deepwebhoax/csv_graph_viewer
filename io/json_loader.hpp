// io/json_loader.hpp
#pragma once
#include <string>
#include <vector>
#include <types.hpp>

std::vector<PlotConfig> loadJSON(const std::string& json_path);
