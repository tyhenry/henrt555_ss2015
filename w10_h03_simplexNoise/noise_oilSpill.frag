#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

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


//simplex noise
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v) {
    const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,  // -1.0 + 2.0 * C.x
                        0.024390243902439); // 1.0 / 41.0
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    vec2 i1;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
        + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

vec3 c1 = vec3(0.278,0.078,0.216);
vec3 c2 = vec3(0.302,1.000,0.580);
vec3 c3 = vec3(1.000,0.969,0.302);
vec3 c4 = vec3(1.000,0.329,0.286);
vec3 c5 = vec3(1.000,0.573,0.286);
vec3 c6 = vec3(0.447,1.000,1.000);

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(0.0);
    vec2 pos = vec2(st*5.);

    vec2 mouse = u_mouse/u_resolution;
    vec2 mouseShift = vec2(map(mouse.x, 0.,1., -.5, .5), map(mouse.y, 0.,1., -.5, .5));
    st += mouseShift;

    float DF = 0.0;

    // Add a random position
    vec2 vel = vec2(0.);
    vel.x = snoise(vec2(u_time*0.1));
    vel.y = snoise(vec2(u_time*0.1));
    DF += snoise(st+vel)*.5+.5;

    // Add a random position
    float a = 0.0;
    a = snoise(st*vec2(cos(u_time*0.15),sin(u_time*0.1))*0.1)*3.1415;
    vel = vec2(1.);//vec2(cos(a),sin(a));
    DF /= snoise(pos+vel)*.25+.25;

    float pct = 1. - smoothstep(.1,.9, fract(DF));

    color += smoothstep(0.8,0.9, pct) * c6;
    color += (1. - smoothstep(0.1,0.3,pct)) * c5;
    if (pct <= .4){
        color = c3;
    }

    gl_FragColor = vec4(color,1.0);
}