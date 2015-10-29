#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

float circle(in vec2 _st, in float _radius){
	
	vec2 mid = _st - vec2(0.5);
	float dist = smoothstep(_radius - (_radius*0.01),
							_radius + (_radius*0.01),
							dot(mid,mid)*2.0);
	return 1.-dist; //invert
}

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution;
	vec3 color = vec3(0.0);

	vec2 mouseNormal = u_mouse/u_resolution;
	st *= mouseNormal * 10.; // max tiles 10*10
	st = fract(st);

	color = vec3(st,0.0);
	color = vec3(circle(st, 0.5));

	gl_FragColor = vec4(color,1.0);
}