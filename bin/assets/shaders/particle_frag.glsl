#version 310 es

precision highp float;

uniform float u_time;
uniform vec3 u_localCamPos;

layout(location=0) in vec3 v_localPos;

out vec4 Color;

float sdf(vec3 p)
{
	p.x = abs(p.x) - 0.25;
	p.z *= 0.45;
	float waves = -0.026*cos(p.z*100.+40.*u_time) - 0.015*cos(p.x*50.)*cos(p.y*50.);
	return length(p) - 0.12 + waves;
}

bool rayMarch(vec3 origin, vec3 ray, inout float t)
{
	t = 0.;
	for(float i=0.; i<32.; i++)
	{
		float r = sdf(origin + ray * t);
		if(r < 0.001)
		{
			return true;
		}
		if(t > 32.)
		{
			break;
		}
		t += r;
	}
	
	return false;
}

void main()
{
	vec3 origin = u_localCamPos;
	vec3 ray = normalize(v_localPos - u_localCamPos);
	float t = 0.;
	
	if(!rayMarch(origin, ray, t))
	{
		discard;
	}
	
	vec3 p = origin + ray * t;
	p.x = abs(p.x) - 0.25;
	float r = length(p.xy);
	vec3 col = mix(vec3(0.9,0.3,0.), vec3(0.9,0.8,0.), p.z+0.7-2.5*r);
	
	Color = vec4(col, 1.);
}