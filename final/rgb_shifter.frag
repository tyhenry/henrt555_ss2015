
#version 120

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

uniform float u_red = 1.;
uniform float u_green = 1.;
uniform float u_blue = 1.;


void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec4 color = vec4(st.x,st.y,0.,1.);

    color = texture2D(u_tex0,st);

    vec3 rgb;
    rgb.r = color.r * u_red;
    rgb.g = color.g * u_green;
    rgb.b = color.b * u_blue;

    gl_FragColor = vec4(rgb, 1.);

}