#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

vec3 white = vec3(1.000,0.976,0.957);
vec3 black = vec3(0.069,0.033,0.066);
vec3 red = vec3(0.702,0.243,0.251);

void main() {

    float timeKeep = mod(u_time * 0.1, 1.0);
    //u_time is in seconds
    //10 secs to get to 1.0
    

    // Mix uses pct (a value from 0-1) to 
    // mix the two colors
    vec3 color = mix(white, black, timeKeep);

	gl_FragColor = vec4(color, 1.0);

}