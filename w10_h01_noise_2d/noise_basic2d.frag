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
        if (val < lo){
            return newLo;
        }
        if (val > hi){
            return newHi;
        }
    }

    float pct = (val-lo)/(hi-lo); //normalize val
    return ((newHi-newLo) * pct) + newLo; //calc + shift normalized val
}

//custom map function (auto-clamps)
float map(float val, float lo, float hi, float newLo, float newHi){

    //clamp
    if (val < lo){
        return newLo;
    }
    if (val > hi){
        return newHi;
    }

    float pct = (val-lo)/(hi-lo); //normalize val
    return ((newHi-newLo) * pct) + newLo; //calc + shift normalized val
}

// 2D Random
float random (in vec2 st) { 
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 corners percentages
    return mix(a, b, u.x) + 
            (c - a)* u.y * (1.0 - u.x) + 
            (d - b) * u.x * u.y;
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    vec2 mouse = u_mouse/u_resolution;
    vec2 mouseShift = vec2(map(mouse.x, 0.,1., -0.5, 0.5), map(mouse.y, 0.,1., -0.5, 0.5));

    st += mouseShift;

    // Scale the coordinate system to see
    // some noise in action
    vec2 pos = vec2(st * map(sin(u_time*0.5), -1., 1., 1., 1000.));

    // Use the noise function
    float n = noise(pos);

    gl_FragColor = vec4(vec3(n), 1.0);
}