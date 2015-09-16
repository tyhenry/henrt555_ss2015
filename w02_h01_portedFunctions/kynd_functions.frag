#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

//plot a line on y using value 0-1f
float plot(vec2 st, float pct){
	return smoothstep(pct-0.02, pct, st.y) -
		smoothstep(pct, pct+0.02, st.y);
		//0.05 controls width of line

}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution; //normalize coords

	//float y = st.x;  //linear interpolation (y=x)
	//float y = pow(st.x,5.0); //power interpolation
	//float y = sqrt(st.x); //square root
	//float y = exp(st.x); //exponential - all white?
	//float y = log(st.x); //logarithmic - all black?

	/*   KYND FUNCTIONS:  */

	//float y = 1.0 - pow(abs(st.x), 0.5);
	//float y = 1.0 - pow(abs(st.x), 1.0);
	//float y = 1.0 - pow(abs(st.x), 1.5);
	//float y = 1.0 - pow(abs(st.x), 2.0);
	//float y = 1.0 - pow(abs(st.x), 2.5);
	//float y = 1.0 - pow(abs(st.x), 3.0);
	//float y = 1.0 - pow(abs(st.x), 3.5);

	//float y = pow(cos(PI * st.x * 0.5), 0.5);

	//float y = 1.0 - pow(abs(sin(PI * st.x * 0.5)), 0.5);

	//float y = pow(min(cos(PI * st.x * 0.5), 1.0 - abs(st.x)), 0.5);

	float y = 1.0 - pow(max(0.0, abs(st.x) * 2.0 - 1.0), 0.5);

	vec3 color = vec3(y); //r,g,b from x coord

	//plot the line
	float pct = plot(st, y);
	//color = darken + add green per plot
	color = (1.0-pct)*color + pct*vec3(0.0,1.0,0.0);

	gl_FragColor = vec4(color, 1.0); // color & 100% opacity

}