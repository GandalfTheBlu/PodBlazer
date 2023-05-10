#version 430

uniform vec3 u_color;
uniform vec3 u_camPos;
uniform vec3 u_lightDir;

layout(location=0) in vec3 v_position;
layout(location=2) in vec3 v_normal;

out vec4 Color;

void main()
{
	vec3 norm = normalize(v_normal);
	vec3 ray = normalize(v_position - u_camPos);
	float diff = max(0., dot(-u_lightDir, norm));
	float spec = pow(max(0., dot(reflect(ray, norm), -u_lightDir)), 32.);
	vec3 ground = vec3(0.1, 0.2, 0.1);
	vec3 sky = vec3(0.2, 0.2, 0.4);
	vec3 amb = mix(ground, sky, 0.5 + 0.5 * norm.y);
	vec3 col = (vec3(diff + spec) + amb) * u_color;
	
	Color = vec4(col, 1.);
}