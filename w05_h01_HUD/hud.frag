#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359
#define TWO_PI 6.28318530718


//#define SMOOTH(r,R) (1.0-smoothstep(R-1.0,R+1.0, r))
/*
#define motion1 vec2(cos(u_time*0.5),sin(u_time*0.1))
#define motion2 vec2(cos(u_time),sin(u_time)*abs(sin(u_time*0.1)))
#define sinAT abs(sin(u_time))
*/

vec3 red = vec3(0.953,0.110,0.082);
vec3 blue = vec3(0.000,0.846,0.669);
vec3 black = vec3(0.055,0.086,0.129);


//MATRICES

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



// SHAPES

float box(vec2 st, vec2 size){
    st += .5;
    size = vec2(0.5) - size*0.5;
    vec2 uv = smoothstep(size,
                        size+vec2(0.001),
                        st);
    uv *= smoothstep(size,
                    size+vec2(0.001),
                    vec2(1.0)-st);
    return uv.x*uv.y;
}

float cross(vec2 st, float size){
    return  box(st, vec2(size,.002)) + 
            box(st, vec2(.002,size));
}

float circle(vec2 st, float size) {
  float pct = 1.0-length(st/size);
  return pct = smoothstep(0.07,0.1, pct) - smoothstep(0.1,0.13, pct);
}
/*
float blip(vec2 uv, vec2 center)
{
  
}
*/

float dot(vec2 uv, vec2 center, float radius)
{
    vec2 d = uv - center;
    float r = sqrt( dot( d, d ) );
    if( r <= 2.5 )
        return 1.0;
    if( ( r<= radius) && ( (abs(d.y+0.5)<=1.0) && ( mod(d.x+1.0, 50.0) < 2.0 ) ) )
        return 1.0;
    else if ( (abs(d.y+0.5)<=1.0) && ( r >= 50.0 ) && ( r < 115.0 ) )
        return 0.5;
    else
      return 0.0;
}

/*from ndel on ShaderToy
float movingLine(vec2 uv, vec2 center, float radius)
{
    //angle of the line
    float theta0 = 90.0 * u_time;
    vec2 d = uv - center;
    float r = sqrt( dot( d, d ) );

    if(r<radius){

      //compute the distance to the line theta=theta0
      vec2 p = radius*vec2(cos(theta0*PI/180.0),
                          -sin(theta0*PI/180.0));
      float l = length( d - p*clamp( dot(d,p)/dot(p,p), 0.0, 1.0) );
      d = normalize(d);

      //compute gradient based on angle difference to theta0
      float theta = mod(180.0*atan(d.y,d.x)/PI+theta0,360.0);
      float gradient = clamp(1.0-theta/90.0,0.0,1.0);
      //return SMOOTH(l,1.0)+0.5*gradient;
      return 1.0;
    }
    else {
      return 0.2;
    }
}
*/


void main( void ) {
	vec2 st = gl_FragCoord.xy/u_resolution.xy;
  vec3 pos = vec3(st,1.0);
  vec2 c = u_resolution.xy/2.;
  
  vec3 color = vec3(0.0);
  //color = movingLine(st, c, 240.0) * red;


  //CIRCLES:

  //center matrix
  pos = translate(vec2(-.5), pos);

  //rotate matrix
  pos = rotate(u_time, pos);

  pos = scale(vec2(sin(u_time)*0.2+0.5),pos);

  vec3 circles = vec3(0.0);

  vec3 pos1 = scale(vec2(sin(u_time*0.1)*0.2+1.,1.), pos);
  circles += vec3(circle(pos1.xy, 0.05));

  vec3 pos2 = scale(vec2(sin(u_time*0.2)*0.2+1.,1.), pos);
  circles += vec3(circle(pos2.xy, 0.1));

  vec3 pos3 = scale(vec2(sin(u_time*0.3)*0.2+1.,1.), pos);
  circles += vec3(circle(pos3.xy, 0.2));

  vec3 pos4 = scale(vec2(sin(u_time*0.4)*0.2+1.,1.), pos);
  circles += vec3(circle(pos4.xy, 0.3));

  vec3 pos5 = scale(vec2(sin(u_time*0.5)*0.2+1.,1.), pos);
  circles += vec3(circle(pos5.xy, 0.5));

  vec3 pos6 = scale(vec2(sin(u_time*0.6)*0.2+1.,1.), pos);
  circles += vec3(circle(pos6.xy, 0.7));

  circles *= red;

  //CROSSHAIRS:

  vec3 posC = translate(vec2(-.5),vec3(st,1.0));

  vec3 posC1 = translate(vec2(sin(u_time)*0.1,cos(u_time)*0.1),posC);
  posC1 = rotate(u_time*0.1, posC1);

  vec3 posC2 = translate(vec2(sin(-u_time)*0.1,cos(-u_time)*0.1),posC);
  posC2 = rotate(u_time*0.1, posC2);

  vec3 posC3 = translate(vec2(sin(u_time)*0.1,cos(-u_time)*0.1),posC);
  posC2 = rotate(u_time*0.1, posC2);

  vec3 posC4 = translate(vec2(sin(-u_time)*0.1,cos(u_time)*0.1),posC);
  posC2 = rotate(u_time*0.1, posC2);

  vec3 crosshairs = vec3(0.0);

  crosshairs += cross(posC1.xy, 2.) * blue;
  crosshairs += cross(posC2.xy, 2.) * blue;
  crosshairs += cross(posC3.xy, 2.) * blue;
  crosshairs += cross(posC4.xy, 2.) * blue;



  color = circles + crosshairs;
  gl_FragColor = vec4(color, 1.0);

}