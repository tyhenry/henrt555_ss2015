#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

void main() {

	vec2 st = gl_FragCoord.xy/u_resolution.xy; //normalize coords
	vec3 color = vec3(0.0);

	/*each result will return 1.0 or 0.0
	float left = step(0.1, st.x); //== if (x > 0.1)
	float bottom = step(0.1, st.y); //== if (y > 0.1)
	*/
	vec2 borders = step(vec2(0.1), st);
	float pct = borders.x * borders.y; //left AND bottom

	vec2 borders2 = step(vec2(0.1), 1.0-st); //invert st
	pct *= borders2.x * borders2.y; //AND right AND top

	color = vec3(pct);

	gl_FragColor = vec4(color,1.0);
}