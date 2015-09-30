#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec4 white = vec4(1.000,0.976,0.957,1.0);
vec4 black = vec4(0.069,0.033,0.066,1.0);
vec4 red = vec4(0.702,0.243,0.251,1.0);
vec4 sky = vec4(0.0,0.0,0.0,1.0); //pure black


vec2 center = vec2(0.5,0.5);
float radius = 0.333;
float border = 0.01;

void main() {

    //COLOR

    float timeKeep = mod(u_time * 0.1, 1.0);
    //u_time is in seconds
    //10 secs to get to 1.0

    float modTime = mod(u_time, 2.5);
    //periodic linear function to count to 2.5

    float cShift = -1.0*(cos(modTime*PI/2.5)*0.5-0.5);
    //gentle curve to x = 2.5
    //represents color shift amount for each transition

    vec4 color = vec4(0.0);

    if (timeKeep < 0.25) {
    	color = mix(white,black, cShift);
    } else if (timeKeep < 0.5) {
    	color = mix(black, red, cShift);
    } else if (timeKeep < 0.75) {
    	color = mix(red, black, cShift);
    } else {
    	color = mix(black, white, cShift);
    }


    //SHAPE

    vec2 st = gl_FragCoord.xy/u_resolution; //normalize coords

    st -= center; //offset coords to center
    float dist = sqrt(dot(st,st)); //circle
    float t = smoothstep(radius+border, radius-border, dist);

	gl_FragColor = mix(sky, color, t);

}