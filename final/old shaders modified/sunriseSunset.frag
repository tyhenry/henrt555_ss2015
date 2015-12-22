#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 yellow = vec3(0.980,0.820,0.373); //yellow
vec3 lOrange = vec3(0.941,0.584,0.243); //lOrange
vec3 dOrange = vec3(0.800,0.345,0.122); //dOrange
vec3 red = vec3(0.620,0.255,0.102); //red
vec3 blue = vec3(0.345,0.345,0.478); //blue
vec3 black = vec3(0.027,0.047,0.145); //black
vec3 sky = vec3(0.672,0.912,1.000); //sky

float trans(float x, float p, float w){ 
//x = input, p = peak in x, w = width
	
	return (smoothstep(p-(w*0.5),p,x)+smoothstep(p+(0.5*w),p,x))-1.0;
}

void main() {

    float timeKeep = mod(u_time * 0.1, 1.0);
    //u_time is in seconds
    //10 secs to get to 1.0

    float modTime = mod(u_time, 2.5);
    //periodic linear function to count to 2.5

    float cShift = -1.0*(cos(modTime*PI/2.5)*0.5-0.5);
    //gentle curve to x = 2.5
    //represents color shift amount for each transition

    //vec2 st = gl_FragCoord.xy/u_resolution.xy;
    //st.y = 1.0 - st.y; //invert y

    //sunrise
    float pct0 = trans(timeKeep, 0.1, 0.3); //input x, peak, width
    float pct1 = trans(timeKeep, 0.125, 0.3);
    float pct2 = trans(timeKeep, 0.15, 0.3);
    float pct3 = trans(timeKeep, 0.20, 0.3);
    float pct4 = trans(timeKeep, 0.25, 0.3);

    //day
    float pct5 = trans(timeKeep, 0.5, 1.0);

    //sunset
    float pct6 = trans(timeKeep, 0.75, 0.3);
    float pct7 = trans(timeKeep, 0.80, 0.3);
    float pct8 = trans(timeKeep, 0.85, 0.3);
    float pct9 = trans(timeKeep, 0.875, 0.3);
    float pct10 = trans(timeKeep, 0.9, 0.3);

    float pct11 = trans(timeKeep, 1.0, 0.6); //black


    vec3 color = black;
    
    color = mix(color, blue, pct0);
    color = mix(color, red, pct1);
    color = mix(color, dOrange, pct2);
    color = mix(color, lOrange, pct3);
    color = mix(color, yellow, pct4);
    color = mix(color, sky, pct5);
    color = mix(color, yellow, pct6);
    color = mix(color, lOrange, pct7);
    color = mix(color, dOrange, pct8);
    color = mix(color, red, pct9);
    color = mix(color, blue, pct10);
    color = mix(color, black, pct11);


	gl_FragColor = vec4(color, 1.0);

}