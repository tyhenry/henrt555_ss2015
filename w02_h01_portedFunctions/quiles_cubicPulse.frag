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

float cubicPulse(float x, float c, float w){ //x coord, peak on x axis, half of full width

	x = abs(x -c);
	if (x>w) return 0.0;
	x /= w;
	return 1.0 - x*x*(3.0-2.0*x);

}

float quadraticBezier(float x, float rX, float rY){ //x coord, bezier handle x, handle y

	float epsilon = 0.00001;

	rX = max(0.0, min(1.0, rX));
	rY = max(0.0, min(1.0, rY));
	if (rX == 0.5){
		rX += epsilon;
	}

	//solve t from x (inverse opertion... yeah...)
	float om2a = 1.0 - 2.0*rX;
	float t = (sqrt(rX*rX + om2a*x) - rX)/om2a;
	float y = (1.0-2.0*rY)*(t*t) + (2.0*rY)*t;
	return y;
}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution; //normalize coords

	//float y = st.x;  //linear interpolation (y=x)
	//float y = pow(st.x,5.0); //power interpolation
	//float y = sqrt(st.x); //square root
	//float y = exp(st.x); //exponential - all white?
	//float y = log(st.x); //logarithmic - all black?

	/*  KYND FUNCTIONS:  */

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

	//float y = 1.0 - pow(max(0.0, abs(st.x) * 2.0 - 1.0), 0.5);


	/*  GOLAN'S QUADRATIC BEZIER  */

	//float y = quadraticBezier(st.x, 0.087, 0.893);
	//float y = quadraticBezier(st.x, 0.94, 0.28);
	//float y = quadraticBezier(st.x, sin(u_time)*0.5+0.5, cos(u_time)*0.5+0.5);


	/*  QUILES CUBIC PULSE, i.e combining two smoothsteps, up and then down  */

	float y = cubicPulse(st.x, 0.5, 0.5)*0.75; //0.75 is height


	vec3 color = vec3(y); //r,g,b from x coord

	//plot the line
	float pct = plot(st, y);
	//color = darken + add green per plot
	color = (1.0-pct)*color + pct*vec3(0.0,1.0,0.0);

	gl_FragColor = vec4(color, 1.0); // color & 100% opacity

}