#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

float circle(in vec2 _st, in float _radius){
	
	vec2 mid = _st - vec2(0.5);
	float dist = smoothstep(_radius - (_radius*0.01),
							_radius + (_radius*0.01),
							dot(mid,mid)*2.0);
	return 1.-dist; //invert
}

float rect(vec2 st, vec2 center, vec2 size, float smoothing){

	vec4 edges = vec4(center.x - size.x*0.5, center.y - size.y*0.5, center.x + size.x*0.5, center.y + size.y*0.5);
	
	float left = smoothstep(edges.x-smoothing,edges.x, st.x);
	float bottom = smoothstep(edges.y-smoothing,edges.y, st.y);
	float right = 1.0 - smoothstep(edges.z,edges.z+smoothing, st.x); //invert result
	float top = 1.0 - smoothstep(edges.w,edges.w+smoothing, st.y); //invert result

	return left * bottom * right * top;
}

float box(in vec2 _st, in vec2 _size){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                        _size+vec2(0.001),
                        _st);
    uv *= smoothstep(_size,
                    _size+vec2(0.001),
                    vec2(1.0)-_st);
    return uv.x*uv.y;
}

float cross(in vec2 _st, float _size){
    return  box(_st, vec2(_size,_size/4.)) + 
            box(_st, vec2(_size/4.,_size));
}

vec2 tile(in vec2 _st, in vec2 _tileDims, out vec2 _st_index){

	_st *= _tileDims; // scale up by x and y

	_st_index = floor(_st); // remove decimal for tile indices

	return fract(_st); // replicate .0-1. tile
}

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution;
	vec3 color = vec3(0.0);

	vec2 st_index;
	vec2 st_tile = tile(st, vec2(3.), st_index);

	if (mod(st_index, 2.) == vec2(0.)){ // if both even
		color = vec3(circle(st_tile, 0.4));
	} else if ((mod(st_index.x, 1.) == 0.)
				&& (mod(st_index.y, 2.) == 0.)){ //if x odd, y even
		color = vec3(cross(st_tile, 0.8));
	} else if ((mod(st_index.y, 1.) == 0.)
				&& (mod(st_index.x, 2.) == 0.)){ //if x even, y odd
		color = vec3(cross(st_tile, 0.8));
	} else {
		color = vec3(circle(st_tile, 0.4)); //if both odd
	}

	//color = vec3(st,0.0);
	//color = vec3(circle(st, 0.5));

	gl_FragColor = vec4(color,1.0);
}