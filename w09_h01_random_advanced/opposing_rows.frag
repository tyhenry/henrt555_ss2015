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
	st.y *= 2.;

	vec2 st_i = floor(st);
	vec2 st_f = fract(st);

	if (st_i.y == 1.){
		st_f.y = 1. - st_f.y; //mirror bottom half
		st_i.x = 1. - st_i.x; //flip direction
	}

	//float timeControl = random(floor(u_time * 4.));
	float timeControl = u_mouse.y/u_resolution.y; //speed based on u_mouse.y

	float time = floor(u_time * 40. * timeControl); //time control
	st_i.x += time; //movement in x

	float pct = random(st_i.x); //random value 0-1

	float threshold = u_mouse.x/u_resolution.x; //threshold at u_mouse.x

	pct = step(threshold,pct); //threshold pct

	vec3 color = vec3(pct);

	gl_FragColor = vec4(color,1.0);
}



