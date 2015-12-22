//uniforms
//speed=0-36000

#version 120

uniform sampler2D u_tex0;

uniform vec2 u_resolution;
uniform float u_time;

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution;
	vec3 color = vec3(0.);
	float pct = 0;

	pct = sin(u_time) * 0.5 + 0.5;

	color = vec3(pct);
	gl_FragColor = vec4(color, 1.);
}