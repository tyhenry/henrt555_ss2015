
#version 120

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform float threshold = 1.;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = texture2D(u_tex0,st).rgb;

    float l = (color.r + color.g + color.b) / 3.;

    if (l > threshold){
    	color.rgb = vec3(0.);
    }

    gl_FragColor = vec4(color, 1.);
}