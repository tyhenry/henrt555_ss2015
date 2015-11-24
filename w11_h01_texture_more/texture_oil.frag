#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

//custom map function (boolean clamps)
float map(float val, float lo, float hi, float newLo, float newHi, bool bClamp){

    //clamp
    if (bClamp) {
        if (val < lo){ return newLo; }
        if (val > hi){ return newHi; }
    }

    float pct = (val-lo)/(hi-lo); //normalize val
    return ((newHi-newLo) * pct) + newLo; //calc + shift normalized val
}

//custom map function (auto-clamps)
float map(float val, float lo, float hi, float newLo, float newHi){

    //clamp
    if (val < lo){ return newLo; }
    if (val > hi){ return newHi; }

    float pct = (val-lo)/(hi-lo); //normalize val
    return ((newHi-newLo) * pct) + newLo; //calc + shift normalized val
}

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Value Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/lsf3WH
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    float pct = mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ), 
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ), 
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
    // -.6 to .54
    return map(pct, -0.6,0.54, 0.,1.);
}


vec3 yellow = vec3(0.992,0.780,0.216);
vec3 red = vec3(0.847,0.200,0.090);


void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec4 color = vec4(st.x,st.y,0.0,1.0);

    vec2 mouse = u_mouse/u_resolution;
    vec2 mouseShift = vec2(map(mouse.x, 0.,1., -.5, .5), map(mouse.y, 0.,1., -.5, .5));
    //st.y += mouseShift.y;
    st.y -= 0.4;

    float t = abs(1.0-sin(u_time*.1))*5.;
    st += noise(vec2( map(st.x,0.,1.,mouse.y,mouse.x*2.),map(st.y,0.,1.,mouse.y,mouse.x)) * t); // warp space and time

    vec2 pos = st; 

    //float pct = st.y;
    //float pct = noise(pos * 25.); //0.-1.

    //color = vec3(st.x,st.y,0.0);
    color = texture2D(u_tex0,vec2(mouse.x, st.y));

    vec4 dotC = texture2D(u_tex0,mouse);

    float smallPct = noise(pos * 40.);
    float small = smoothstep(0.8,0.9, smallPct);
    color *= (1. - small);
    color += small * dotC;


    gl_FragColor = color;
}