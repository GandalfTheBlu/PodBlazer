#version 310 es

precision highp float;

layout(location=0) in vec3 a_position;
layout(location=1) in vec2 a_uv;
layout(location=2) in vec3 a_normal;

layout(location=0) out vec3 v_position;
layout(location=1) out vec2 v_uv;
layout(location=2) out vec3 v_normal;

uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat3 u_N;
uniform vec3 u_camPos;

void main()
{
	// shrink y scale when nearing max distance
	float maxDist = 30.;
	vec3 pos0 = a_position;
	vec3 pos1 = (u_M * vec4(pos0, 1.)).xyz;
	pos0.y *= max(0., 1.-exp(-(maxDist-distance(pos1, u_camPos))));
	
	v_uv = a_uv;
	v_normal = u_N * a_normal;
	v_position = (u_M * vec4(pos0, 1.)).xyz;
	gl_Position = u_MVP * vec4(pos0, 1.);
}