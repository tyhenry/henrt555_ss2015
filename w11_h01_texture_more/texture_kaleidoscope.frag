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

#define PI 3.14159265358979323846


vec2 rotate2D(vec2 _st, float _angle){
    _st -= 0.5;
    _st =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _st;
    _st += 0.5;
    return _st;
}

void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec4 color = vec4(st.x,st.y,0.0,1.0);

    float ang1 = u_time* -.2;
    st = rotate2D(st, ang1);


    // invert bottom
    if (st.y < .5){
    	st.y = (1.-st.y);
    }
    // invert right
    if (st.x > .5){
    	st.x = (1.-st.x);
    }

    st *= (sin(u_time)*0.5+0.7);
    st -= u_mouse/u_resolution;

    float ang = PI*(sin(u_time*.2)*0.5+0.5);
    st = rotate2D(st, ang);


    color = texture2D(u_tex0,st);

    gl_FragColor = color;
}