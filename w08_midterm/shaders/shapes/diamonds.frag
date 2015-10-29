#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform float u_time;

//identity matrix
mat3 matrix = mat3(vec3(1.0,0.0,0.0),
                   vec3(0.0,1.0,0.0),
                   vec3(0.0,0.0,1.0));

mat3 scaleMatrix(vec2 f){

    return mat3(vec3(f.x,0.0,0.0),
                vec3(0.0,f.y,0.0),
                vec3(0.0,0.0,1.0));
}

vec3 scale(vec2 f, vec3 pos){
    return scaleMatrix(f) * pos;
}



mat3 translationMatrix(vec2 f){

    return mat3(vec3(1.0,0.0,0.0),
                vec3(0.0,1.0,0.0),
                vec3(f.x,f.y,1.0));
}

vec3 translate(vec2 f, vec3 pos){
    return translationMatrix(f) * pos;
}

mat3 rotationMatrix(float a){

    return mat3(vec3(cos(a),-sin(a),0.0),
                vec3(sin(a),cos(a),0.0),
                vec3(0.0,0.0,1.0));
}

vec3 rotate(float a, vec3 pos){
    return rotationMatrix(a) * pos;
}

// Reference to
// http://thndl.com/square-shaped-shaders.html

float shapeDistField(vec2 st, float sides){

  float dist = 0.0;

  // Remap the space to -1. to 1.
  st = st *2.-1.;

  // Angle and radius from the current pixel
  float a = atan(st.x,st.y)+PI;
  float r = TWO_PI/float(sides);

  // Shaping function that modulate the distance
  dist = cos(floor(.5+a/r)*r-a)*length(st);

  return 1.-dist; //invert
}

float equilTriangle(vec2 st, float radius, float smoothing){

	//st.y += 0.1; //fix centering

	float dist = shapeDistField(st, 3.); //generate triangle field
	float pct = smoothstep(radius-smoothing*0.5, radius+smoothing*0.5, dist);
	return pct;
}

float squeezedEquilTriangle(vec2 st, float radius, float smoothing, vec2 scalar){
	vec3 pos = vec3(st, 1.);
	pos = translate(vec2(-.5),pos); //move to center
	pos = scale(scalar, pos); //scale
	pos = translate(vec2(0.5), pos); //move back
	return equilTriangle(pos.xy, radius, smoothing);
}

float diamond(vec2 st, float smoothing, vec2 scalar){
  float radius = 0.5;
  vec3 pos = vec3(st, 1.);
  pos = translate(vec2(-.5),pos);
  pos = scale(scalar,pos);
  pos = translate(vec2(.5),pos);
  float pct = squeezedEquilTriangle(vec2(pos.x,pos.y-.2765), radius, smoothing, vec2(1.,2.26));
  pos = translate(vec2(-.5), pos);
  pos = rotate(PI,pos);
  pos = translate(vec2(.5,.446), pos);
  pct += squeezedEquilTriangle(pos.xy, radius, smoothing, vec2(1.,1.13));
  return pct;
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    
    //vec3 color = vec3(shapeDistField(st, 3.)); 

    //vec3 color = vec3(equilTriangle(st, 0.5, 0.1));

    vec3 color = vec3(diamond(st, 0.015, vec2(.87,1.)));


    //vec3 color = vec3(squeezedEquilTriangle(pos.xy, 0.5, 0.015, vec2(2.6,3.9)));

    gl_FragColor = vec4( color, 1.0);
}