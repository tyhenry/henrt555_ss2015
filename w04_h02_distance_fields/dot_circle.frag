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

float circle(in vec2 st, in float radius, float smoothing){
    vec2 l = st-vec2(0.5);
    return 1.-smoothstep(radius-(radius*smoothing),
                         radius+(radius*smoothing),
                         dot(l,l)*4.0);
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    
    vec3 color = vec3(circle(st,0.4,0.1));

    gl_FragColor = vec4( color, 1.0);
}