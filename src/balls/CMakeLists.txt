add_executable(balls
    collision.cpp
    main.cpp
    view.cpp
    world.cpp
)
target_link_libraries(balls PRIVATE sdl-cpp toolkit geometry)

add_custom_command(TARGET balls POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:balls> $<TARGET_RUNTIME_DLLS:balls>
    COMMAND_EXPAND_LISTS
)