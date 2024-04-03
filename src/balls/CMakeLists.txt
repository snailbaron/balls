add_executable(balls
    collision.cpp
    main.cpp
    timer.cpp
    world.cpp
 "intersection.cpp" "geometry.cpp" "distance.cpp" "view.cpp" "sdl.cpp")
target_link_libraries(balls PRIVATE SDL3::SDL3)
add_custom_command(TARGET balls POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:balls> $<TARGET_RUNTIME_DLLS:balls>
    COMMAND_EXPAND_LISTS
)