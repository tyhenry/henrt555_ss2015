#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 red = vec3(0.910,0.102,0.129);
vec3 orange = vec3(0.937,0.392,0.118);
vec3 yellow = vec3(0.980,0.643,0.090);
vec3 green = vec3(0.349,0.659,0.239);
vec3 blue = vec3(0.000,0.486,0.773);
vec3 violet = vec3(0.349,0.157,0.525);

float trans(float x, float p, float w){ 
//x = input, p = peak in x, w = width
	
	return (smoothstep(p-(w*0.5),p,x)+smoothstep(p+(0.5*w),p,x))-1.0;

}

void main() {

    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.y = 1.0 - st.y; //invert y
    vec3 color = vec3(0.0);

    float pct0 = trans(st.y, 0.0, 1.0); //input x, peak, width
    float pct1 = trans(st.y, 0.2, 1.0);
    float pct2 = trans(st.y, 0.4, 1.0);
    float pct3 = trans(st.y, 0.6, 1.0);
    float pct4 = trans(st.y, 0.8, 1.0);
    float pct5 = trans(st.y, 1.0, 1.0);
    

    // Mix uses pct (a value from 0-1) to 
    // mix the two colors
    color = mix(vec3(0.0), red, pct0);
    color = mix(color, orange, pct1);
    color = mix(color, yellow, pct2);
    color = mix(color, green, pct3);
    color = mix(color, blue, pct4);
    color = mix(color, violet, pct5);

	gl_FragColor = vec4(color, 1.0);

}