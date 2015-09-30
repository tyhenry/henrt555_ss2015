#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 color0 = vec3(0.922,0.118,0.318);
vec3 color1 = vec3(1.000,1.000,1.000);
vec3 color2 = vec3(0.969,0.525,0.863);
vec3 color3 = vec3(0.922,0.086,0.196);
vec3 color4 = vec3(0.965,0.875,0.239);
vec3 color5 = vec3(0.922,0.086,0.196);

float trans(float x, float p, float w){ 
//x = input, p = peak in x, w = width
	
	return (smoothstep(p-(w*0.5),p,x)+smoothstep(p+(0.5*w),p,x))-1.0;

}

void main() {

    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.y = 1.0 - st.y; //invert y
    vec3 color = vec3(0.0);

    float pct0 = trans(st.y, 0.0, 0.5); //input x, peak, width
    float pct1 = trans(st.y, 0.2, 0.5);
    float pct2 = trans(st.y, 0.3, 0.2);
    float pct3 = trans(st.y, 0.6, 1.0);
    float pct4 = trans(st.y, 0.85, 0.2);
    float pct5 = trans(st.y, 1.0, 0.3);
    

    // Mix uses pct (a value from 0-1) to 
    // mix the two colors
    color = mix(vec3(0.922,0.086,0.196), color0, pct0);
    color = mix(color, color1, pct1);
    color = mix(color, color2, pct2);
    color = mix(color, color3, pct3);
    color = mix(color, color4, pct4);

	gl_FragColor = vec4(color, 1.0);

}