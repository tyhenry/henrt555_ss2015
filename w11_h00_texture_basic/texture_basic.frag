// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec4 color = vec4(st.x,st.y,0.0,1.0);

    //move by mouse
 	vec2 stMouse = st + (u_mouse/u_resolution)*2.;

    //zoom by 2
    vec2 stHalf = vec2(stMouse.x/2.,stMouse.y/2.);

    //rotate 90
    vec2 stRot = vec2(stHalf.y,stHalf.x);


    color = texture2D(u_tex0,stRot);

    gl_FragColor = color;
}