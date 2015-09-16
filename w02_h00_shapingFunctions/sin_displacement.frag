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

	//step divides function based on first argument 0-1.f
	//float y = step(0.5,st.x);
	//smoothstep smoothly divides
	//float y = smoothstep(0.2,0.8,st.x);
	//float y = smoothstep(0.2,0.5,st.x) - step(0.8,st.x); //connect 2 smoothsteps

	float y = sin(st.x*PI + u_time)*0.5+0.5;

	vec3 color = vec3(y); //r,g,b from x coord

	//plot the line
	float pct = plot(st, y);
	//color = darken + add green per plot
	color = (1.0-pct)*color + pct*vec3(0.0,1.0,0.0);

	gl_FragColor = vec4(color, 1.0); // color & 100% opacity

}