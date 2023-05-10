#version 310 es

precision highp float;
precision highp int;

layout(location=0) in vec2 a_position;

uniform mat4 u_MVP;

layout(location=0) out vec2 v_uv;

void main()
{
	v_uv = a_position * 0.5 + 0.5;
	gl_Position = u_MVP * vec4(a_position, 0., 1.);
}