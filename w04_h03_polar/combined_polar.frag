#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    vec2 pos = vec2(0.5)-st;

    float r = length(pos)*2.0;
    float a = atan(pos.y,pos.x);

    //float f = cos(a*3.);
    // f = abs(cos(a*3.));
    // f = abs(cos(a*2.5))*.5+.3;
    // f = abs(cos(a*12.)*sin(a*3.))*.8+.1;
    float f1 = smoothstep(-.5,1., cos(a*10.+u_time*10.))*0.2+0.5;

    vec3 color1 = vec3( 1.-smoothstep(f1,f1+0.01,r) );

    float f2 = smoothstep(-.5,1., cos(a*4.-u_time*20.))*0.3+0.1;

    vec3 color2 = vec3( 1.-smoothstep(f2,f2+0.01,r) );

    color = color1 - color2;

    gl_FragColor = vec4(color, 1.0);
}