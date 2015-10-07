#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;


float circle(vec2 st, vec2 center, float radius, float smoothing){

    float dist = distance(st,center); //generate distance field cone

    float circle = smoothstep(radius, radius+smoothing, dist); //slice that cone

    return clamp(1.0 - circle, 0.0, 1.0); //invert + clamp (is clamp needed?)

}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution;
    float pct = 0.0;

    vec3 color = vec3(circle(st, vec2(0.6), 0.2, 0.1)); //st, center.xy, radius, smoothing

	gl_FragColor = vec4(color, 1.0);
}