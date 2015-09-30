#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

vec3 color0 = vec3(0.463,0.941,0.816);
vec3 color1 = vec3(0.161,0.718,0.494);
vec3 color2 = vec3(0.314,0.502,0.318);
vec3 color3 = vec3(0.404,0.329,0.220);
vec3 color4 = vec3(0.482,0.439,0.482);
vec3 color5 = vec3(0.592,0.596,0.765);
vec3 color6 = vec3(0.686,0.722,0.929);

float chunk (float x, float num){ 
//num = order of chunk

    float begin = num*0.14;
	
	return (step(begin+0.03, x) - step(begin+0.14, x));

}

void main() {

    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    float pct0 = chunk(st.x, 0.); //input x, peak, width
    float pct1 = chunk(st.x, 1.);
    float pct2 = chunk(st.x, 2.);
    float pct3 = chunk(st.x, 3.);
    float pct4 = chunk(st.x, 4.);
    float pct5 = chunk(st.x, 5.);
    float pct6 = chunk(st.x, 6.);


    vec3 color = vec3(0.502,0.369,0.231); //base color
    
    color = mix(color, color0, pct0);
    color = mix(color, color1, pct1);
    color = mix(color, color2, pct2);
    color = mix(color, color3, pct3);
    color = mix(color, color4, pct4);
    color = mix(color, color5, pct5);
    color = mix(color, color6, pct6);

	gl_FragColor = vec4(color, 1.0);

}