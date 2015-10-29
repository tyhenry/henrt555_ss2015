#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359
#define TWO_PI 6.28318530718

vec3 color0 = vec3(0.773,0.459,0.753);
vec3 color1 = vec3(0.153,0.369,0.847);
vec3 color2 = vec3(0.063,0.204,0.392);
vec3 color3 = vec3(0.212,0.322,0.490);
vec3 color4 = vec3(0.569,0.475,0.871);

float trans(float x, float p, float w){ 
//x = input, p = peak in x, w = width
  
  return (smoothstep(p-(w*0.5),p,x)+smoothstep(p+(0.5*w),p,x))-1.0;

}

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

void main(){
  vec2 st = gl_FragCoord.xy/u_resolution.xy;
  //st.x *= u_resolution.x/u_resolution.y;
  vec3 color = vec3(1.0);
  float d = 0.0;


  vec3 pos = vec3(st,1.);

  pos = translate(vec2(-.5), pos);
  pos = scale(vec2(2.0), pos);
  vec3 pos1 = rotate(u_time, pos);
  vec3 pos2 = rotate(-u_time, pos);

  // Make the distance field
  d = length( min(abs(pos1.xy)-.3,0.));
  d += length( max(abs(pos1.xy)-.3,0.));
  d *= length( max(abs(pos2.xy)-.3,0.));
  d += length( abs(pos2.xy)-.5);

  vec3 overlay = vec3(fract(d* (sin(u_time*0.1))*1000.*cos(u_time*0.01)));

  //COLOR

  pos = vec3(st,1.);
  pos = rotate(u_time*0.1, pos);

  float pct0 = trans(st.x*sin(u_time)*0.5+0.5, 0.0, 4.0); //input x, peak, width
  float pct1 = trans(st.y*sin(u_time)*0.5+0.5, 0.55, 0.55);
  
  // Mix uses pct (a value from 0-1) to 
  // mix the two colors
  color = mix(vec3(0.0), color0, pct0);
  color = mix(color, color1, pct1);

  //color

  color = color * overlay;

  gl_FragColor = vec4(color,1.0);

  // Drawing with the distance field
  // gl_FragColor = vec4(vec3( step(.3,d) ),1.0);
  // gl_FragColor = vec4(vec3( step(.3,d) * step(d,.4)),1.0);
  // gl_FragColor = vec4(vec3( smoothstep(.3,.4,d)* smoothstep(.6,.5,d)) ,1.0);
}