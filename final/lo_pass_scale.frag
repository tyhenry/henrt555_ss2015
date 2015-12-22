
#version 120

uniform sampler2D u_tex0;
uniform vec2 u_tex0Resolution;

uniform float u_threshold = 1.; //default pass all
uniform float u_power = 5.; //0 - 10

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


void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = texture2D(u_tex0,st).rgb;

    float threshold = u_threshold;
    float power = u_power;

    float l = (color.r + color.g + color.b) / 3.; //luma

    float pct = map(l, threshold, 1., 1., 0.); //map luma from threshold=1 to 0

    color *= pow(pct,power); //pct to nth power to ramp up gradient from black

    gl_FragColor = vec4(color, 1.);
}