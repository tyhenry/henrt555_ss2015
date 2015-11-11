// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float random (vec2 st) { 
	if (u_mouse.y/u_resolution.y > 0.5){
    	return fract(sin(dot(st.xy,
    	                 //vec2(12.,78.)))* 
        				 //43.);
                         vec2(12.9898,78.233)))* 
        				 sin(u_time)*43758.5453123);
    }
       	return fract(sin(dot(st.xy,
    	                 vec2(12.,78.)))* 
        				 43.);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    float rnd = random( st + u_mouse);

    gl_FragColor = vec4(vec3(rnd),1.0);
}