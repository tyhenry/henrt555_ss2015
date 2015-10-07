#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

void main() {

	vec2 st = gl_FragCoord.xy/u_resolution.xy; //normalize coords
	vec3 color = vec3(0.0);

	float smoothing = 0.01; //amount for smoothstep

	//each step result will return 1.0 or 0.0
	float left = smoothstep(0.4,0.4+smoothing, st.x); //== if (x > 0.4)
	float bottom = smoothstep(0.3,0.3+smoothing, st.y); //== if (y > 0.3)
	float right = smoothstep(0.2,0.2+smoothing, 1.0-st.x); //== if (x < 0.2)
	float top = smoothstep(0.5,0.5+smoothing, 1.0-st.y); //==if (y < 0.5);

	/*
	vec2 borders = step(vec2(0.1), st);
	float pct = borders.x * borders.y; //left AND bottom

	vec2 borders2 = step(vec2(0.1), 1.0-st); //invert st
	pct *= borders2.x * borders2.y; //AND right AND top
	*/

	float pct = left * bottom * right * top;

	color = vec3(pct);

	gl_FragColor = vec4(color,1.0);
}