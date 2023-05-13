#version 310 es

precision highp float;

layout(location=1) in vec2 v_uv;

layout(binding=0) uniform sampler2D u_fontTexture;
uniform vec2 u_charPosition;
uniform vec2 u_charSize;
uniform vec3 u_color;

out vec4 Color;

void main()
{
	vec2 coords = u_charPosition + v_uv * u_charSize;
	vec3 col = texture(u_fontTexture, coords).rgb * u_color;
	
	if(max(col.r, max(col.g, col.b)) < 0.01)
	{
		discard;
	}
	
	Color = vec4(col, 1.);
}