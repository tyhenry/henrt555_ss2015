#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

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

    float offset1 = sin(u_time * noise(u_mouse.x/u_resolution.x))*0.1;
	float offset2 = sin(u_time * noise(u_mouse.y/u_resolution.y))*0.1;

	vec2 pos1 = vec2(st.x + map(noise(u_time*0.5),0.,1.,-0.05,0.05), st.y);
	vec2 pos2 = vec2(st.x + map(noise(u_time*0.5 + 1000.),0.,1.,-0.05,0.05), st.y);


    float circle1 = wobbleCircleOutline(pos1,0.05,map(noise(u_time),0.,1.,0.1,0.5), offset1, map(noise(u_time),0.,1.,0.01,0.05));
    float circle2 = wobbleCircleOutline(pos2, 0.05,map(noise(u_time+1000.),0.,1.,0.1,0.5), offset2, map(noise(u_time+1000.),0.,1.,0.01,0.05));

    vec3 color1 = vec3(noise(u_time),noise(u_time+100.),noise(u_time+200.));
    vec3 color2 = vec3(noise(u_time+1000.),noise(u_time+1100.),noise(u_time+1200.));

    //vec3 color = mix((circle1*color1), (circle2*color2), 0.5);//noise(u_time));
	vec3 color = (circle1*color1) + (circle2*color2);

    gl_FragColor = vec4( color, 1.0);
}