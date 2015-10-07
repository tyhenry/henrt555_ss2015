#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution;
    float pct = 0.0;


    // a. The DISTANCE from the pixel to the center
    pct = 2. * distance(st,vec2(0.5)); //multiplying by 2 doubles density
    pct = 1.0 - pct; //invert

    float circle = step(0.5,pct); //turn gradient into circle

    vec3 color = clamp(vec3(pct + circle), vec3(0.0), vec3(1.0)); //show circle and gradient - clamp overlay

	gl_FragColor = vec4(color, 1.0 );
}