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
	vec2 padding1 = vec2(0.01, 0.);
	vec2 padding2 = vec2(1., 0.99);
	vec2 coords = u_charPosition + clamp(v_uv, padding1, padding2) * u_charSize;
	float alpha = texture(u_fontTexture, coords).r;
	Color = vec4(u_color, alpha);
}