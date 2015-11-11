#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

vec3 green = vec3(0.,.7,0.);

float random (float x){
	return fract(sin(x)*10e5);
}

//custom map function (clamps)
float map(float val, float lo, float hi, float newLo, float newHi){

	//clamp
	if (val < lo){
		return newLo;
	}
	if (val > hi){
		return newHi;
	}

	float origRange = hi - lo;
	float newRange = newHi - newLo;

	float pct = (val-lo)/origRange; //pct in orgRange
	pct *= newRange; //pct in newRange

	return newLo + pct; //shift pct to newLo - newHi range
}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution.xy;

	st.x *= 100.;
	st.y *= 100.;

	vec2 st_i = floor(st);
	vec2 st_f = fract(st);


	float timeControl = random(st_i.x+1.) + .1; //speed based on st_i.y

	float time = floor(u_time * 40. * timeControl); //time control

	st_i.y += time; //movement in x

	float pct = random(st_i.y); //random value 0-1

	float threshold = u_mouse.x/u_resolution.x; //threshold at u_mouse.x
	//threshold = map(threshold, 0., 1., 0.3, 0.6); //re-map u_mouse.x;

	if (pct > 0.96){
		green = vec3(1.); //make white
	}

	if (step(threshold,pct) == 0.){ //threshold pct
		pct = 0.;
	}

	vec3 color = vec3(pct * green);

	gl_FragColor = vec4(color,1.0);
}



