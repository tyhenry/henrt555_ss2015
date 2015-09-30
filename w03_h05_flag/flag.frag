#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 green = vec3(0.000,0.576,0.333);
vec3 yellow = vec3(0.988,0.792,0.086);
vec3 blue = vec3(0.196,0.416,0.737);


void main() {

    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.y = 1.0 - st.y; //invert y
    vec3 color = vec3(0.0);

    float pctG = step(0.0,st.y)-step(0.333,st.y);
    float pctY = step(0.333,st.y)-step(0.666,st.y);
    float pctB = step(0.666,st.y)-step(1.0,st.y);
    

    // Mix uses pct (a value from 0-1) to 
    // mix the two colors
    color = mix(vec3(0.0), green, pctG);
    color = mix(color, yellow, pctY);
    color = mix(color, blue, pctB);

	gl_FragColor = vec4(color, 1.0);

}