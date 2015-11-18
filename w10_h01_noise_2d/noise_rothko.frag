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
        if (val <= lo){ return newLo; }
        if (val >= hi){ return newHi; }
    }

    float pct = (val-lo)/(hi-lo); //normalize val
    return ((newHi-newLo) * pct) + newLo; //calc + shift normalized val
}

//custom map function (auto-clamps)
float map(float val, float lo, float hi, float newLo, float newHi){

    //clamp
    if (val <= lo){ return newLo; }
    if (val >= hi){ return newHi; }

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

float dField(vec2 st, vec2 center, float radius, float smoothing){

    float dist = distance(st,center); //generate distance field cone

    float field = smoothstep(radius, radius+smoothing, dist); //slice that cone

    //return clamp(field, 0.0, 1.0); //invert + clamp (is clamp needed?)
    return field;

}

float dFieldNoise (in vec2 st){
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Distance Field Interpolation
    vec2 u = vec2( dField(f, vec2(.5), 0.1, 1.) );

    // Cubic Hermine Curve.  Same as SmoothStep()
    //vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 corners percentages
    return mix(a, b, u.x) + 
            (c - a)* u.y * (1.0 - u.x) + 
            (d - b) * u.x * u.y;
}

float rect(vec2 st, vec2 center, float w, float h, float smoothing){

    vec4 edges = vec4(center.x - w*0.5, center.y - h*0.5, center.x + w*0.5, center.y + h*0.5);
    
    float left = smoothstep(edges.x-smoothing,edges.x, st.x);
    float bottom = smoothstep(edges.y-smoothing,edges.y, st.y);
    float right = 1.0 - smoothstep(edges.z,edges.z+smoothing, st.x); //invert result
    float top = 1.0 - smoothstep(edges.w,edges.w+smoothing, st.y); //invert result

    return left * bottom * right * top;
}

vec3 peach = vec3(0.918,0.463,0.365);
vec3 blue = vec3(0.039,0.208,0.353);
vec3 mauve = vec3(0.318,0.267,0.325);

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    vec2 pos1 = vec2( 0.5, 0.69);
    float rect1 = rect(st, pos1, 0.84, 0.44, 0.07);

    vec2 pos2 = vec2( 0.5, 0.22);
    float rect2 = rect(st, pos2, 0.84, 0.28, 0.04);

    //add noise to colors

    // Scale
    vec2 nPos1 = vec2(st * 11.);
    vec2 nPos2 = vec2(st * 14.);

    // Noise
    float n1 = map(noise(nPos1 + vec2(1000.)), 0., 1., 0.9, 1.);
    float n2 = map(noise(nPos2 + vec2(2000.)), 0., 1., 0.93, 1.);

    peach *= n1;
    blue *= n2;

    vec3 color = mauve - rect1*mauve - rect2*mauve;
    color += rect1*peach + rect2*blue;

    gl_FragColor = vec4(color, 1.0);
}









