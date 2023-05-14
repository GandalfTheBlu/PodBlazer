#version 310 es

precision highp float;

layout(binding=0) uniform sampler2D u_texture; 
uniform vec3 u_camPos;
uniform vec3 u_lightDir;
uniform float u_roughness;
uniform vec3 u_exhaustPos;

layout(location=0) in vec3 v_position;
layout(location=1) in vec2 v_uv;
layout(location=2) in vec3 v_normal;

out vec4 Color;

void main()
{
	vec3 norm = normalize(v_normal);
	vec3 toPoint = v_position - u_camPos;
	vec3 ray = normalize(toPoint);
	float diff = max(0., dot(-u_lightDir, norm));
	float spec = pow(max(0., dot(reflect(ray, norm), -u_lightDir)), clamp(1.-u_roughness, 1./64., 1.) * 64.);
	vec3 ground = vec3(0.2, 0.1, 0.1);
	vec3 sky = vec3(0.2, 0.2, 0.4);
	vec3 amb = mix(ground, sky, 0.5 + 0.5 * norm.y) * (1. + max(0., -u_lightDir.y));
	float occlusion = (u_lightDir.y > 0. ? 0. : 1.);
	vec3 col = (vec3((diff + spec) * occlusion) + amb) * texture(u_texture, v_uv).rgb;
	
	float dist = dot(toPoint, toPoint);
	float x = clamp(0.001*dist, 0., 1.);
	float fog = x * x * (3. - 2. * x);
	vec3 fogCol = mix(vec3(0.7, 0.7, 0.85), vec3(0.1, 0.1, 0.2), 0.5 + 0.5 * u_lightDir.y);
	col = mix(col, fogCol, fog);
	
	vec3 toExhaust = v_position - u_exhaustPos;
	float bloom = exp(-0.75*dot(toExhaust, toExhaust));
	col += vec3(0.9,0.7,0.) * min(bloom, 1.);
	
	Color = vec4(col, 1.);
}