add_executable(editor
    main.cpp
)
target_link_libraries(editor PRIVATE sdl-cpp toolkit geometry)

add_custom_command(TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:editor> $<TARGET_RUNTIME_DLLS:editor>
    COMMAND_EXPAND_LISTS
)