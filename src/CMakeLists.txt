cmake_minimum_required(VERSION 3.21)

add_subdirectory(shaders)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(main)
target_sources(main PRIVATE main.c)
target_include_directories(main PUBLIC ../vendor/sokol ../vendor/hmm shaders)
target_link_libraries(main PUBLIC -pthread GL dl m asound X11 Xi Xcursor)
target_compile_definitions(main PUBLIC SOKOL_GLCORE33)
