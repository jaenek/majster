@ctype vec4 hmm_vec4
@ctype mat4 hmm_mat4

@vs vs
uniform vs_params {
    mat4 vp;
    mat4 transform;
	vec4 flat_color;
};

in vec3 position;

out vec4 color;

void main() {
    gl_Position = vp * transform * vec4(position, 1.0);
    color = flat_color;
}
@end

@fs fs
in vec4 color;
out vec4 frag_color;

void main() {
    frag_color = color;
}
@end

@program flat vs fs
