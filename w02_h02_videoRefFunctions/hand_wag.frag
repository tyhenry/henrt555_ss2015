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

float quadraticBezier(float x, float rX, float rY){

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

	float y = pow(cos(PI * st.x-PI * 0.5), 1.5)-smoothstep(0.5,0.7,st.x);

	vec3 color = vec3(y); //r,g,b from x coord

	//plot the line
	float pct = plot(st, y);
	//color = darken + add green per plot
	color = (1.0-pct)*color + pct*vec3(0.0,1.0,0.0);

	gl_FragColor = vec4(color, 1.0); // color & 100% opacity

}