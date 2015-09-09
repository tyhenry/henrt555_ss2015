#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution; //canvas size
uniform vec2 u_mouse; //mouse pos inside canvas
uniform float u_time; //time (sec since load)

float redTime, greenTime, blueTime, redSTime, greenSTime, blueSTime;

void main(){
	/*
	redTime = u_time * 2.0;
	greenTime = u_time * 0.5;
	blueTime = u_time * 10.0;
	redSTime = abs(sin(redTime));
	greenSTime = abs(sin(greenTime));
	blueSTime = abs(sin(blueTime));
	*/

	vec2 st = gl_FragCoord.xy/u_resolution; //normalize coordinates
	vec2 mp = u_mouse/u_resolution; //mouse pos normalized

	float blueColor = (abs(sin(u_time * mp.x)));

	gl_FragColor = vec4(st.x, st.y, blueColor, 1.0);

}