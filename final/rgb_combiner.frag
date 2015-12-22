
#version 120

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform sampler2D u_tex1;
uniform vec2 u_tex1Resolution;

uniform sampler2D u_tex2;
uniform vec2 u_tex2Resolution;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


void main () {

    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec4 color = vec4(st.x,st.y,0.,1.);

    float r = texture2D(u_tex0,st).r;
    float g = texture2D(u_tex1,st).g;
    float b = texture2D(u_tex2,st).b;

    color = vec4(r,g,b,1.);

    gl_FragColor = color;

}