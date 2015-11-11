#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float random (float x){
	return fract(sin(x)*10e5);
}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution.xy;

	st.x *= 100.;
	st.y *= 100.;

	vec2 st_i = floor(st);
	vec2 st_f = fract(st);

	
	if (random(st_i.y+1000.) < 0.5){
		st_i.x = 1. - st_i.x; //flip direction based on random st_i.y value
	}
	

	float timeControl = random(st_i.y+1.); //speed based on st_i.y

	float time = floor(u_time * 40. * timeControl); //time control
	st_i.x += time; //movement in x

	float pct = random(st_i.x); //random value 0-1

	float threshold = u_mouse.x/u_resolution.x; //threshold at u_mouse.x

	pct = step(threshold,pct); //threshold pct

	vec3 color = vec3(pct);

	gl_FragColor = vec4(color,1.0);
}



