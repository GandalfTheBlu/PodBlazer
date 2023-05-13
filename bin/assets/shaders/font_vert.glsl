#version 310 es

precision highp float;

layout(location=0) in vec2 a_position;

uniform vec2 u_localOffset;
uniform mat4 u_M;

layout(location=1) out vec2 v_uv;

void main()
{
	v_uv = a_position * 0.5 + vec2(0.5);
	vec2 pos = (u_M * vec4(a_position + u_localOffset, 0., 1.)).xy;
	gl_Position = vec4(pos, 0., 1.);
}