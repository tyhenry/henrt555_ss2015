
#version 120

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec4 color = vec4(st.x,st.y,0.,1.);

    color = vec4(0., 0., texture2D(u_tex0,st).b, 1.);

    gl_FragColor = color;

}