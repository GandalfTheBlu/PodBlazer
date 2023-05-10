#version 430

uniform mat3 u_invMVP;
uniform vec3 u_lightDir;

layout(location=0) in vec2 v_position;

out vec4 Color;

void main()
{
	vec3 ray = normalize(u_invMVP * vec3(v_position, 1.));
	
	vec3 sky = vec3(0.9, 0.97, 1.);
	vec3 horizon = vec3(0.95, 0.92, 0.6);
	vec3 ground = vec3(0.3, 0.4, 0.3);
	float t = max(0., ray.y);
	float t1 = pow(t, 0.3);
	float t2 = 1.-pow(1.-t, 4.);
	vec3 col = mix(ground, mix(horizon, sky, t2), t1);
	
	vec3 toSun = ray + u_lightDir;
	float sun = exp(-40.*dot(toSun, toSun));
	col += vec3(0.99, 0.97, 0.8) * sun;
	
	Color = vec4(col, 1.);
}