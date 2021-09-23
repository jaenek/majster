file(GLOB shader_files *.glsl)
foreach(shader IN LISTS shader_files)
	add_custom_command(
		OUTPUT ${shader}.h
		COMMAND sokol-shdc -i ${shader} -o ${shader}.h -l glsl330
		DEPENDS ${shader}
		COMMENT "Compiling ${shader}"
	)
	list(APPEND shader_headers "${shader}.h")
endforeach()
add_custom_target(shaders ALL DEPENDS ${shader_headers})
