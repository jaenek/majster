@ctype vec2 hmm_vec2
@ctype vec3 hmm_vec3
@ctype vec4 hmm_vec4
@ctype mat4 hmm_mat4

@vs vs
uniform vs_params {
    mat4 vp;
};

in vec2 position;
in vec2 texcoord;
in vec2 inst_position;
in vec2 size;
in vec4 color;

out vec4 out_color;
out vec2 uv;

void main() {
	vec4 pos = vec4(position * size + inst_position, 0.0f, 1.0f);
    gl_Position = vp * pos;
    out_color = color;
	uv = texcoord;
}
@end

@fs fs
uniform sampler2D tex;

in vec4 out_color;
in vec2 uv;

out vec4 frag_color;

void main() {
    frag_color = texture(tex, uv) * out_color;
}
@end

@program flat vs fs
