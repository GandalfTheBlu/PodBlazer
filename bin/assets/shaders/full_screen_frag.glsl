#version 310 es

precision highp float;
precision highp int;

layout(binding=0) uniform sampler2D u_texture;

layout(location=0) in vec2 v_uv;

out vec4 Color;

void main()
{
	Color = texture(u_texture, v_uv);
}