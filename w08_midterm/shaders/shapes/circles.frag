#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

float circle(in vec2 st, in float radius, in float smoothing){
    vec2 center = st-vec2(0.5);
    return 1.-smoothstep(radius-(radius*smoothing),
                         radius+(radius*smoothing),
                         dot(center,center)*2.0);
}

float wobbleCircle(in vec2 st, in float radius, in float smoothing, in float offset){
	float pct = 0.;

	//middle circle
	pct = circle(st, radius, smoothing);

	//2nd circle
	pct += circle(vec2(st.x,st.y + offset), radius*(1.-offset*2.), smoothing);

	//3rd circle
	pct += circle(vec2(st.x + offset*0.5,st.y), radius*(1.-offset*2.), smoothing);

	//4th circle
	pct += circle(vec2(st.x - offset, st.y-offset*0.2), radius*(1.-offset*3.), smoothing);

	return clamp(pct, 0., 1.);
}

float wobbleCircleOutline(in vec2 st, in float radius, in float smoothing, in float offset, in float thickness){

	float pct = 0.;
	pct = wobbleCircle(st, radius+thickness*0.5, smoothing, offset);
	pct -= wobbleCircle(st, radius-thickness*0.5, smoothing, offset);
	return pct;
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    float offset = sin(u_time*0.5)*0.1;
    
    vec3 color = vec3(wobbleCircleOutline(st,0.1,0.05,offset, 0.01));

    gl_FragColor = vec4( color, 1.0);
}