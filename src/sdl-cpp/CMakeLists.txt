add_library(sdl-cpp
    sdl.cpp
)
target_include_directories(sdl-cpp PUBLIC include)
target_link_libraries(sdl-cpp
    PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf
    PRIVATE toolkit
)