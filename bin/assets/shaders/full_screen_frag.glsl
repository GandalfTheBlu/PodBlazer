#version 430

layout(location=0) in vec2 v_uv;

out vec4 Color;

void main()
{
	Color = vec4(v_uv.r, v_uv.g, 0., 0.);
}