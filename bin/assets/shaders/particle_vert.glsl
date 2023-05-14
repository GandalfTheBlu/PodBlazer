#version 310 es

precision highp float;

layout(location=0) in vec3 a_position;

layout(location=0) out vec3 v_localPos;

uniform mat4 u_MVP;

void main()
{
	v_localPos = a_position;
	gl_Position = u_MVP * vec4(a_position, 1.);
}