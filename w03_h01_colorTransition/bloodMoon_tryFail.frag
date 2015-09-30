#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

vec3 white = vec3(1.000,0.976,0.957);
vec3 black = vec3(0.069,0.033,0.066);
vec3 red = vec3(0.702,0.243,0.251);

void main() {
	vec3 color = vec3(0.0);
    vec3 colorA = vec3(0.0);
    vec3 colorB = vec3(0.0);

    float timeKeep = mod(u_time * 0.5,1.0);
    //happens 10 times as fast as timeKeep
    float pctChange = sin(u_time) * 0.5 + 0.5;

    if (timeKeep < 0.2){
    	colorA = white;
    	colorB = black;
    } else if (timeKeep < 0.4) {
    	colorA = black;
    	colorB = red;
    } else if (timeKeep < 0.6) {
    	colorA = red;
    	colorB = black;
    } else if (timeKeep < 0.8) {
    	colorA = black;
    	colorB = white;
    } else {
    	colorA = white;
    	colorB = white;
    }
    

    // Mix uses pct (a value from 0-1) to 
    // mix the two colors
    color = mix(colorA, colorB, pctChange);

	gl_FragColor = vec4(color, 1.0);


    if ((gl_FragCoord.x < 100.) && (gl_FragCoord.y < 100.)){
    	gl_FragColor = vec4(colorA, 1.0);
    }
}