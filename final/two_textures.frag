
#version 120

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform sampler2D u_tex1;
uniform vec2 u_tex1Resolution;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec4 color0 = vec4(st.x,st.y,0.,1.);
    vec4 color1 = vec4(st.x,st.y,0.,1.);

    color0 = texture2D(u_tex0,st);
    color1 = texture2D(u_tex1,st);

    //gl_FragColor = mix(color0,color1,0.5);
    gl_FragColor = color0 * color1;

}