cmake_minimum_required(VERSION 3.21)

add_subdirectory(shaders)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(main)
add_dependencies(main shaders)
target_sources(main PRIVATE main.c config.c renderer2d.c camera.c events.c)
target_include_directories(main PUBLIC ../vendor/sokol ../vendor/sokol_spritebatch/util ../vendor/hmm ${SHADER_DIR})
target_link_libraries(main PUBLIC -pthread GL dl m asound X11 Xi Xcursor lua)
target_compile_definitions(main PUBLIC SOKOL_GLCORE33)
