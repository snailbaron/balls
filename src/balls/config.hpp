#pragma once

#include <string>

struct Config {
    int fps = 240;
    std::string windowTitle = "balls";
    int windowWidth = 1024;
    int windowHeight = 768;
};

inline Config config;