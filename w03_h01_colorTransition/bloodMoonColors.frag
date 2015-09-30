#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 white = vec3(1.000,0.976,0.957);
vec3 black = vec3(0.069,0.033,0.066);
vec3 red = vec3(0.702,0.243,0.251);

void main() {

    float timeKeep = mod(u_time * 0.1, 1.0);
    //u_time is in seconds
    //10 secs to get to 1.0

    float modTime = mod(u_time, 2.5);
    //periodic linear function to count to 2.5

    float cShift = -1.0*(cos(modTime*PI/2.5)*0.5-0.5);
    //gentle curve to x = 2.5
    //represents color shift amount for each transition
    

    vec3 color = vec3(0.0);

    if (timeKeep < 0.25) {
    	color = mix(white,black, cShift);
    } else if (timeKeep < 0.5) {
    	color = mix(black, red, cShift);
    } else if (timeKeep < 0.75) {
    	color = mix(red, black, cShift);
    } else {
    	color = mix(black, white, cShift);
    }

	gl_FragColor = vec4(color, 1.0);

}