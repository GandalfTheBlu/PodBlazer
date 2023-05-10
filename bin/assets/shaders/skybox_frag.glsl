#version 430

uniform mat3 u_invMVP;
uniform vec3 u_lightDir;

layout(location=0) in vec2 v_position;

out vec4 Color;

float getStars(vec3 ray)
{
	vec3 d1 = 2. * fract(ray * 35.52) * fract(-ray * 35.52);
	float d2 = dot(d1, d1);
	float d3 = 1. - 8. * fract(d2 * 5.623) * fract(-d2 * 5.623);
	float d4 = clamp(20.*(max(d3, 0.9)-0.9), 0., 1.);
	return d4 * 0.25;
}

void main()
{
	vec3 ray = normalize(u_invMVP * vec3(v_position, 1.));
	vec3 sun = -u_lightDir;
	
	float d = ray.x*sun.x + ray.z*sun.z;
	float pw1 = pow(max(0., 1.-ray.y), 3.);
	float pw2 = pow(max(0., 1.-ray.y), 5.);
	float redshift = max(0., d * pw1);
	float purpleshift = max(0., -d * pw2);
	
	float stars = getStars(ray) * pow(0.5 - 0.5 * sun.y, 5.);
	vec3 nightSky = mix(vec3(0.1, 0.1, 0.2), vec3(1.), stars);
	vec3 sky = mix(nightSky, vec3(0.52, 0.8, 0.92), 0.5 + 0.5 * sun.y);
	sky = mix(sky, vec3(0.9, 0.35, 0.1), redshift);
	sky = mix(sky, vec3(0.9, 0.3, 0.4), purpleshift);
	vec3 ground = vec3(0.3, 0.4, 0.3);
	float t = pow(max(0., ray.y), 0.2);
	vec3 col = mix(ground, sky, t);
	
	vec3 toSun = ray + u_lightDir;
	float t2 = pow(max(0., ray.y), 0.5);
	float sunPwr = exp(-(60.+60.*t2)*dot(toSun, toSun)) * max(0., sign(ray.y));
	col += vec3(sunPwr);
	
	Color = vec4(col, 1.);
}