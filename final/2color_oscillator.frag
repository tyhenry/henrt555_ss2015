
#version 120

uniform vec3 color0 = vec3(0.,0.,0.);
uniform vec3 color1 = vec3(1.,1.,1.);

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


void main () {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.);

    float pct = sin(u_time) * 0.5 + 0.5;

    color = mix(color0, color1, pct);

    gl_FragColor = vec4(color, 1.);
}