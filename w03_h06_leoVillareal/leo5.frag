#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 color0 = vec3(0.333,0.184,0.941);
vec3 color1 = vec3(0.565,0.400,1.000);
vec3 color2 = vec3(0.200,0.169,0.894);
vec3 color3 = vec3(0.655,0.616,1.000);
vec3 color4 = vec3(0.867,0.780,1.000);
vec3 color5 = vec3(0.494,0.404,1.000);

float trans(float x, float p, float w){ 
//x = input, p = peak in x, w = width
	
	return (smoothstep(p-(w*0.5),p,x)+smoothstep(p+(0.5*w),p,x))-1.0;

}

void main() {

    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.y = 1.0 - st.y; //invert y

    st -= 0.5;

    float r = length(st*3.);
    float a = atan(st.y,st.x)/PI;
    a = a*0.5+0.5;

    st = vec2(a,r);


    float pct0 = trans(st.y, 0.0, 0.4); //input x, peak, width
    float pct1 = trans(st.y, 0.2, 0.5);
    float pct2 = trans(st.y, 0.5, 0.5);
    float pct3 = trans(st.y, 0.6, 0.8);
    float pct4 = trans(st.y, 0.8, 0.6);
    float pct5 = trans(st.y, 1.0, 0.3);
    

    vec3 color = vec3(0.0);
    color = mix(vec3(0.0), color0, pct0);
    color = mix(color, color1, pct1);
    color = mix(color, color2, pct2);
    color = mix(color, color3, pct3);
    color = mix(color, color4, pct4);
    color = mix(color, color5, pct5);

	gl_FragColor = vec4(color, 1.0);

}