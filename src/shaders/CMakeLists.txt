file(GLOB SHADERS *.glsl)
foreach(SHADER IN LISTS SHADERS)
    execute_process(COMMAND sokol-shdc -i "${SHADER}" -o "${SHADER}.h" -l glsl330 COMMAND_ECHO STDOUT)
endforeach()
