@ctype vec4 hmm_vec4
@ctype mat4 hmm_mat4

@vs vs
uniform vs_params {
    mat4 vp;
    mat4 transform;
	vec4 flat_color;
};

in vec3 position;
in vec2 texcoord;

out vec4 color;
out vec2 uv;

void main() {
    gl_Position = vp * transform * vec4(position, 1.0);
    color = flat_color;
	uv = texcoord;
}
@end

@fs fs
uniform sampler2D tex;

in vec4 color;
in vec2 uv;

out vec4 frag_color;

void main() {
    frag_color = texture(tex, uv) * color;
}
@end

@program flat vs fs
