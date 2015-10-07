#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

void main() {

	vec2 st = gl_FragCoord.xy/u_resolution.xy; //normalize coords
	vec3 color = vec3(0.0);

	float smoothing = 0.1; //amount for smoothstep

	//each step result will return 1.0 or 0.0
	vec4 edges = vec4(0.1,0.2,0.2,0.4);
	float left = smoothstep(edges.x,edges.x+smoothing, st.x); //== if (x > 0.4)
	float bottom = smoothstep(edges.y,edges.y+smoothing, st.y); //== if (y > 0.3)
	float right = smoothstep(edges.z,edges.z+smoothing, 1.0-st.x); //== if (x < 0.2)
	float top = smoothstep(edges.w,edges.w+smoothing, 1.0-st.y); //==if (y < 0.5);

	/*
	vec2 borders = step(vec2(0.1), st);
	float pct = borders.x * borders.y; //left AND bottom

	vec2 borders2 = step(vec2(0.1), 1.0-st); //invert st
	pct *= borders2.x * borders2.y; //AND right AND top
	*/

	float pct = floor(10.0*left * bottom * right * top) * 0.1;

	color = vec3(pct);

	gl_FragColor = vec4(color,1.0);
}