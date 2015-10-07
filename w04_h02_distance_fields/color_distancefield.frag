#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;


vec3 beige = vec3(0.953,0.941,0.914);
vec3 red = vec3(0.973,0.259,0.173);
vec3 blue = vec3(0.220,0.243,0.506);
vec3 yellow = vec3(0.992,0.745,0.275);
vec3 black = vec3(0.027,0.039,0.071); 


float circle(vec2 st, vec2 center, float radius, float smoothing){

    float dist = distance(st,center); //generate distance field cone

    float circle = smoothstep(radius, radius+smoothing, dist); //slice that cone

    return clamp(1.0 - circle, 0.0, 1.0); //invert + clamp (is clamp needed?)

}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution;
    float pct = 0.0;

    float circle = circle(st, vec2(0.6), 0.2, 0.1); //st, center.xy, radius, smoothing
    vec3 color = yellow * circle;

	gl_FragColor = vec4(color, 1.0);
}