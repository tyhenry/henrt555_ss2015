#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float random (float x){
    return fract(sin(x)*10e5);
}

float noise(float x) {
    float i = floor(x);
    float f = fract(x);
    return mix(random(i),random(i+1.),smoothstep(0.,1.,f));
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    vec3 color = vec3(0.0);

    color.r = noise(u_time*0.5);
    color.g = noise(u_time + 1000.);
    color.b = noise(u_time*1.5 + 2000.);

    gl_FragColor = vec4(color, 1.0);
}