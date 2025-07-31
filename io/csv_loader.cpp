// io/csv_loader.cpp
#include "csv_loader.hpp"
#include <fstream>
#include <sstream>

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
