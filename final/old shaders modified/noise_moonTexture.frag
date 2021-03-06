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
    return map(pct, -0.6,0.54, 0.,1., true); //map to 0., 1.
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(0.);

    vec2 mouse = u_mouse/u_resolution;
    vec2 mouseShift = vec2(map(mouse.x, 0.,1., -.5, .5), map(mouse.y, 0.,1., -.5, .5));
    st += mouseShift;

    vec2 pos = st; 

    float pct = st.y;
    pct = noise(pos * 12.); //0.-1.
    
    color = texture2D(u_tex0,vec2(mouse.x,mouse.y)).rgb;//vec3(0.45); //gray
    color += smoothstep(0.95,1., pct); //add white highlights
    color -= (1.-smoothstep(0.2,0.3, pct))*vec3(0.5); //black spots
    color += 1.-smoothstep(0.2,0.25, pct); //interior of black spots

    float dapple = map(noise(pos * 38.), 0.,1., -.1, .1);
    color += dapple;

    gl_FragColor = vec4(color,1.0);
}