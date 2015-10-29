//
//  ofxPatterns.h
//  SHADERS_MIDTERM_VJ_APP
//
//  Created by Tyler Henry on 10/29/15.
//
//
//  This shader contains 10 patterns which can
//  be combined and adjusted as video masks
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxPatterns : public ofxFXObject {
public:
    
    ofxPatterns(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        //SHADER CODE
        fragmentShader =
        STRINGIFY(
                  
                  
                  const float PI = 3.141592;
                  const float TWO_PI = 6.283185;
                  
                  uniform sampler2DRect tex0; //video texture
                  
                  uniform vec2 u_resolution; //dims of texture
                  uniform float u_time; //ofGetElapsedTimef()
                  
                  //mask amounts
                  uniform float dotsAmt;
                  uniform float wavesAmt;
                  uniform float diamondsAmt;
                  uniform float crosses1Amt;
                  uniform float crosses2Amt;
                  uniform float crosses3Amt;
                  uniform float chevronsAmt;
                  uniform float trapezoids1Amt;
                  uniform float trapezoids2Amt;
                  uniform float squeezeBoxesAmt;
                  
                  
                  /// MATRIX TRANSFORMS
                  
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
                  
                  ///// SHAPES /////
                  
                  //// DIST FIELDS ////
                  
                  float shapeDistField(vec2 st, float sides){
                      // Reference to
                      // http://thndl.com/square-shaped-shaders.html
                      
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
                  
                  //// CIRCLES ////
                  
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
                  
                  
                  //// TRIANGLES ////
                  
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
                  
                  
                  //// QUADS ////
                  
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
                  
                  float box(in vec2 _st, in vec2 _size, in float smoothing){
                      _size = vec2(0.5) - _size*0.5;
                      vec2 uv = smoothstep(_size,
                                           _size+vec2(smoothing),
                                           _st);
                      uv *= smoothstep(_size,
                                       _size+vec2(smoothing),
                                       vec2(1.0)-_st);
                      return uv.x*uv.y;
                  }
                  
                  float trapezoid(vec2 st, vec2 size, float smoothing, float stretch){
                      vec3 pos = vec3(st, 1.);
                      pos = translate(vec2(-.5),pos); //move to center
                      pos = scale(vec2(1.,.8),pos);
                      pos = rotate(PI*0.333, pos); //rotate
                      pos = scale(vec2(1.-stretch,1.),pos);
                      pos = rotate(-PI*0.333, pos); //rotate
                      pos = translate(vec2(0.5), pos); //move back
                      return box(pos.xy, size, smoothing);
                  }
                  
                  
                  //// MORE ////
                  
                  float squeezeBox(vec2 st, vec2 size, float smoothing, float stretch){
                      float pct = trapezoid(st, size, smoothing, stretch);
                      st.x = 1. - st.x; //invert axis
                      pct += trapezoid(st, size, smoothing, stretch);
                      return pct;
                  }
                  
                  float chevron(vec2 st, vec2 shrink){
                      vec3 pos = vec3(st, 1.);
                      pos = translate(vec2(-0.5),pos);
                      pos = scale(vec2(0.45)*shrink,pos);
                      pos = translate(vec2(0.5),pos);
                      
                      vec2 size = vec2(0.25);
                      float smoothing = 0.001;
                      float stretch = 0.5;
                      
                      vec3 pos1 = vec3(pos.xy, 1.);
                      pos1 = translate(vec2(0.114,0.0),pos1);
                      pos1 = translate(vec2(-0.5), pos1);
                      pos1 = scale(vec2(2.),pos1);
                      pos1 = rotate(PI*.37, pos1);
                      pos1 = translate(vec2(0.5), pos1);
                      float pct = trapezoid(pos1.xy, size, smoothing, stretch);
                      vec3 pos2 = vec3(pos.xy, 1.);
                      pos2.x = 1.-pos2.x;
                      pos2 = translate(vec2(0.114,0.0),pos2);
                      pos2 = translate(vec2(-0.5), pos2);
                      pos2 = scale(vec2(2.),pos2);
                      pos2 = rotate(PI*.37, pos2);
                      pos2 = translate(vec2(0.5), pos2);
                      pct += trapezoid(pos2.xy, size, smoothing, stretch);
                      
                      return pct;
                  }
                  
                  float cross(vec2 st, vec2 size, vec2 intersection, float smoothing){
                      vec2 pos = st;
                      pos.y = st.y - intersection.y;
                      float horizBox = box(pos, vec2(size.x,size.y), smoothing);
                      pos = st;
                      pos.x = st.x - intersection.x;
                      float vertBox = box(pos, vec2(size.y,size.x), smoothing);
                      
                      return vertBox + horizBox;
                  }
                  
                  
                  
                  
                  ///// LINES /////
                  
                  //plot a line on y using value 0-1f
                  float plot(vec2 st, float pct, float smoothing){
                      return smoothstep(pct-smoothing, pct, st.y) -
                      smoothstep(pct, pct+smoothing, st.y);
                      //0.05 controls width of line
                      
                  }
                  
                  ///CUBIC BEZIER
                  
                  // Helper functions:
                  float slopeFromT (float t, float A, float B, float C){
                      float dtdx = 1.0/(3.0*A*t*t + 2.0*B*t + C);
                      return dtdx;
                  }
                  
                  float xFromT (float t, float A, float B, float C, float D){
                      float x = A*(t*t*t) + B*(t*t) + C*t + D;
                      return x;
                  }
                  
                  float yFromT (float t, float E, float F, float G, float H){
                      float y = E*(t*t*t) + F*(t*t) + G*t + H;
                      return y;
                  }
                  
                  float cubicBezier(float x, float a, float b, float c, float d){
                      
                      float y0a = 0.00; // initial y
                      float x0a = 0.00; // initial x
                      float y1a = b;    // 1st influence y
                      float x1a = a;    // 1st influence x
                      float y2a = d;    // 2nd influence y
                      float x2a = c;    // 2nd influence x
                      float y3a = 1.00; // final y
                      float x3a = 1.00; // final x
                      
                      float A =   x3a - 3.*x2a + 3.*x1a - x0a;
                      float B = 3.*x2a - 6.*x1a + 3.*x0a;
                      float C = 3.*x1a - 3.*x0a;
                      float D =   x0a;
                      
                      float E =   y3a - 3.*y2a + 3.*y1a - y0a;
                      float F = 3.*y2a - 6.*y1a + 3.*y0a;
                      float G = 3.*y1a - 3.*y0a;
                      float H =   y0a;
                      
                      // Solve for t given x (using Newton-Raphelson), then solve for y given t.
                      // Assume for the first guess that t = x.
                      float currentt = x;
                      int nRefinementIterations = 5;
                      for (int i=0; i < nRefinementIterations; i++){
                          float currentx = xFromT (currentt, A,B,C,D);
                          float currentslope = slopeFromT (currentt, A,B,C);
                          currentt -= (currentx - x)*(currentslope);
                          currentt = clamp(currentt, 0.,1.);
                      }
                      
                      float y = yFromT (currentt,  E,F,G,H);
                      return y;
                      
                  }
                  
                  //// BEZIER SQUIGGLY WAVE ////
                  
                  float squiggle(vec2 st, float mag, float reps, float smoothing){
                      
                      st *= reps;
                      float y = cubicBezier(st.x, 1.-mag, mag, mag, 1.-mag);
                      int intReps = int(reps);
                      for (int i=1; i < intReps; i+=2){
                          y -= cubicBezier(st.x-float(i), mag, 1.-mag, 1.-mag, mag);
                          y += cubicBezier(st.x-float(i+1), 1.-mag, mag, mag, 1.-mag);
                      }
                      //y *= 1./reps;
                      st *= 1./reps;
                      y = plot(st, y, smoothing);
                      return y;
                  }
                  
                  
                  //// PATTERNS ////
                  
                  //TILES
                  
                  vec2 tile(in vec2 st, in vec2 zoom, out vec2 st_index){
                      
                      st *= zoom; // scale up by x and y
                      
                      st_index = floor(st); // remove decimals for tile indices
                      
                      return fract(st); // replicate .0-1. tile
                  }
                  
                  vec2 brickTile(in vec2 st, in vec2 zoom, in vec2 offsetMod, in vec2 offset, out vec2 st_index){
                      
                      st *= zoom; // scale up by x and y
                      
                      st_index = floor(st); // remove decimals for tile indices
                      
                      st.x -= step(1., mod(st.y, offsetMod.y)) * offset.y;
                      st.y -= step(1., mod(st.x, offsetMod.x)) * offset.x;
                      
                      return fract(st); // replicate .0-1. tile
                  }
                  
                  vec2 chevronTile(in vec2 st, in vec2 zoom, out vec2 st_index){
                      
                      st *= zoom; // scale up by x and y
                      
                      st_index = floor(st); // remove decimals for tile indices
                      
                      //st.y -= step(1., mod(st.x, 2.)) * -fract(u_time);
                      
                      if (mod(st_index.x, 2.) == 1.){
                          st.y += fract(u_time*3.);
                      } else {
                          st.y -= fract(u_time*3.);
                      }
                      
                      return fract(st); // replicate .0-1. tile
                  }
                  
                  vec2 trapTile(in vec2 st, in vec2 zoom, out vec2 st_index){
                      
                      st *= zoom; // scale up by x and y
                      
                      st_index = floor(st); // remove decimals for tile indices
                      
                      //st.y -= step(1., mod(st.x, 2.)) * -fract(u_time);
                      
                      if (mod(st_index.x, 2.) == 1.){
                          st.y += fract(u_time*3.);
                      } else {
                          st.y -= fract(u_time*3.);
                      }
                      
                      return fract(st); // replicate .0-1. tile
                  }
                  
                  vec2 trapTile2(in vec2 st, in vec2 zoom, out vec2 st_index){
                      
                      st *= zoom; // scale up by x and y
                      
                      st_index = floor(st); // remove decimals for tile indices
                      
                      //st.y -= step(1., mod(st.x, 2.)) * -fract(u_time);
                      
                      st.x -= sin(u_time + st_index.y)*0.1;
                      
                      
                      if (step(1., mod(st_index.y, 2.)) == 0.){
                          st.x = 1. - st.x;
                      }
                      
                      
                      return fract(st); // replicate .0-1. tile
                  }
                  
                  vec2 squeezeTile(in vec2 st, in vec2 zoom, out vec2 st_index){
                      
                      st *= zoom; // scale up by x and y
                      
                      st_index = floor(st); // remove decimals for tile indices
                      
                      //st.y -= step(1., mod(st.x, 2.)) * -fract(u_time);
                      
                      float cornerIndex = 0.;
                      
                      cornerIndex += step(1.,mod(st_index.x, 2.));
                      cornerIndex += step(1., mod(st_index.y, 2.))*2.;
                      
                      // 0 == upper left
                      // 1 == upper right
                      // 2 == lower left
                      // 3 == lower right
                      
                      st = fract(st); // replicate .0-1. tile
                      
                      //vec3 pos = vec3(st, 1.);
                      
                      if (cornerIndex == 1.){
                          st.x = 1. - st.x;
                      } else if (cornerIndex == 2.){
                          st.y = 1. - st.y;
                      } else if (cornerIndex == 3.){
                          st.x = 1. - st.x;
                          st.y = 1. - st.y;
                      }
                      
                      return st;
                      
                  }
                  
                  //PATTERN OUTPUT
                  
                  float dots(vec2 st){
                      
                      st -= 0.2;
                      
                      vec2 st_index;
                      vec2 st_tile = tile(st, vec2(11.,21.), st_index);
                      
                      //vec2 st_tile = brickTile(st, vec2(11.), vec2(2.), vec2(sin(u_time)*0.1,cos(u_time)*0.1), st_index);
                      
                      st_tile.x -= step(1., mod(st_index.y, 2.)) * cos(u_time)*0.1;
                      st_tile.y -= step(1., mod(st_index.x, 2.)) * sin(u_time)*0.1;
                      
                      float offset = sin(u_time+mod(st_index.x, 2.3))*.04*mod(st_index.y,2.8); //circle distortion
                      
                      float dots = 0.;
                      
                      st_tile *= vec2(2.,1);
                      
                      
                      //if ((mod(st_index.x, 2.) == 0.) && (mod(st_index.y, 2.) == 0.)){
                      vec3 tilePos = vec3(st_tile, 1.);
                      //tilePos = translate()
                      dots = wobbleCircle(st_tile,0.01,2.5,offset*.4);
                      //}
                      
                      // add some "randomness"
                      float pulse = sin(u_time*0.4 + 100. * (st_index.x+1.8) * 2. / (st_index.y+1.) * 0.5) * 0.5 + 0.5;
                      
                      float pct = dots * pulse;
                      pct = clamp(pct, 0., 1.);
                      
                      return pct;
                      
                  }
                  
                  float waves(vec2 st){
                      
                      vec2 st_index;
                      float nRows = (cos(u_time*0.1)*0.5+0.5)*10.+50.;
                      vec2 st_tile = tile(st, vec2(1.,nRows), st_index); //only tile y
                      
                      float pct = squiggle(st_tile, (sin(u_time*10.)*0.5+0.5)*.5+0.25, 20., 0.2);
                      pct = clamp(pct, 0., 1.);
                      
                      return pct;
                      
                  }
                  
                  float diamonds(vec2 st){
                      
                      vec2 st_index;
                      float nRows = (cos(u_time*0.1)*0.5+0.5)*5.;
                      vec2 st_tile = tile(st, vec2(5.,nRows), st_index); //only tile y
                      
                      float pct = diamond(st_tile, 0.01, vec2(.85,.99));
                      
                      pct = clamp(pct, 0., 1.);
                      
                      return pct;
                  }
                  
                  float crosses1(vec2 st){
                      
                      vec2 st_index;
                      vec2 st_tile = tile(st, vec2(21.,21.), st_index);
                      
                      float rotDir = step(1., mod(st_index.x, 2.)) * 2. - 1.; //-1 or 1
                      rotDir *= step(1., mod(st_index.y, 2.)) * 2. - 1.; //-1 or 1
                      
                      vec3 pos = vec3(st_tile, 1.);
                      pos = translate(vec2(-0.5),pos);
                      pos = rotate(PI*sin(u_time)*rotDir,pos);
                      pos = translate(vec2(0.5),pos);
                      
                      float pct = cross(pos.xy, vec2((sin(u_time*rotDir)*0.5+0.5)*0.7+0.3,.05), vec2((cos(u_time)*0.5+0.5)*0.04), 0.001);
                      
                      
                      pct = clamp(pct, 0., 1.);
                      
                      return pct;
                  }
                  
                  float crosses2(vec2 st){
                      
                      st.x += 0.05;
                      
                      vec2 st_index;
                      
                      float stretch = sin(u_time)*0.5+0.5;
                      
                      vec2 st_tile = trapTile2(st, vec2(14., 21.), st_index);
                      
                      float opacity = sin(u_time*0.5+mod(st_index.x, 2.0)+mod(st_index.y,2.0))*0.5+0.5;
                      
                      
                      vec3 pos = vec3(st_tile, 1.);
                      pos = translate(vec2(-.5),pos);
                      pos = rotate(u_time, pos);
                      pos = translate(vec2(.5),pos);
                      
                      float pct = 0.;
                      
                      if ((mod(st_index.x, 3.0) == 1.) && (mod(st_index.y, 3.)==1.)){
                          pct = cross(pos.xy, vec2(0.5, 0.065), vec2(0.02), 0.03);
                      }
                      
                      //float opacity = (step(1., mod(st_index.x))+1.)*2;
                      //opacity of cross modified by index
                      pct *= opacity;
                      
                      pct = clamp(pct, 0., 1.);
                      return pct;
                  }
                  
                  float crosses3(vec2 st){
                      
                      vec2 st_index;
                      
                      float stretch = sin(u_time)*0.5+0.5;
                      
                      vec2 st_tile = trapTile2(st, vec2(14., 21.), st_index);
                      
                      vec3 pos = vec3(st_tile, 1.);
                      pos = translate(vec2(-.5),pos);
                      //pos = rotate(u_time, pos);
                      pos = scale(vec2((sin(u_time+st_index.y)*0.5+0.5)*.7+.8), pos);
                      pos = translate(vec2(.5),pos);
                      
                      float pct = cross(pos.xy, vec2(0.5, 0.065), vec2(0.02*st_index.x*0.01*st_index.y), 0.03);
                      
                      pct = clamp(pct, 0., 1.);
                      return pct;
                  }
                  
                  float chevrons(vec2 st){
                      
                      vec2 st_index;
                      
                      //float nCols = floor((sin(u_time)*0.5+0.5)*40.);
                      vec2 st_tile = chevronTile(st, vec2(40.,40.), st_index);
                      
                      if (step(1., mod(st_index.x, 2.)) == 0.){
                          st_tile.y = 1. - st_tile.y;
                      }
                      
                      float pct = chevron(st_tile, vec2(1.,.73));
                      
                      pct = clamp(pct, 0., 1.);
                      return pct;
                  }
                  
                  float trapezoids1(vec2 st){
                      
                      vec2 st_index;
                      
                      //float nCols = floor((sin(u_time)*0.5+0.5)*40.);
                      float offset = sin(u_time);
                      vec2 st_tile = brickTile(st, vec2(7.,21.), vec2(2.,2.), vec2(offset,0.), st_index);
                      
                      /*
                       if (step(1., mod(st_index.y, 2.)) == 0.){
                       st_tile.x = 1. - st_tile.x;
                       }
                       */
                      
                      st_tile.x -= sin(u_time + st_index.y)*0.1;
                      
                      float pct = trapezoid(st_tile, vec2(0.2), 0.01, 0.5);
                      
                      pct = clamp(pct, 0., 1.);
                      return pct;
                  }
                  
                  float trapezoids2(vec2 st){
                      
                      vec2 st_index;
                      
                      float stretch = sin(u_time)*0.5+0.5;
                      
                      vec2 st_tile = trapTile2(st, vec2(14., 21.), st_index);
                      
                      vec3 pos = vec3(st_tile, 1.);
                      pos = translate(vec2(-.5),pos);
                      pos = rotate(u_time, pos);
                      pos = translate(vec2(.5),pos);
                      
                      float pct = trapezoid(pos.xy, vec2(0.2), 0.02, stretch*.6);
                      
                      pct = clamp(pct, 0., 1.);
                      return pct;
                  }
                  
                  float squeezeBoxes(vec2 st){
                      
                      vec2 st_index;
                      
                      vec2 st_tile = squeezeTile(st, vec2(30.,30.), st_index);
                      
                      vec3 pos = vec3(st_tile, 1.);
                      pos = translate(vec2(-.5,.5), pos);
                      
                      pos = translate(vec2(-.5),pos);
                      pos = rotate(u_time, pos);
                      pos = translate(vec2(.5),pos);
                      
                      float squeeze = sin(u_time*10.+st_index.x+st_index.y)*.1;
                      float squeezeOffset = cos(u_time*10.+(29.-st_index.x)+(29.-st_index.y));
                      
                      float pct = squeezeBox(pos.xy, vec2(0.2+squeezeOffset*0.05), 0.003, 0.6+squeeze);
                      
                      pct = clamp(pct, 0., 1.);
                      return pct;
                  }
                  
                  
                  
                  ///MAIN
                  
                  void main(){
                      vec2 st = gl_TexCoord[0].st;
                      
                      vec2 st_norm = st/(u_resolution*1.2); //normalize (w/ offset?)
                      st_norm.y = 1.-st_norm.y; //invert y
                      
                      float mask = 0.;
                      
                      mask += dots(st_norm) * dotsAmt;
                      mask += waves(st_norm) * wavesAmt;
                      mask += diamonds(st_norm) * diamondsAmt;
                      mask += crosses1(st_norm) * crosses1Amt;
                      mask += crosses2(st_norm) * crosses2Amt;
                      mask += crosses3(st_norm) * crosses3Amt;
                      mask += chevrons(st_norm) * chevronsAmt;
                      mask += trapezoids1(st_norm) * trapezoids1Amt;
                      mask += trapezoids2(st_norm) * trapezoids2Amt;
                      mask += squeezeBoxes(st_norm) * squeezeBoxesAmt;
                      
                      mask = clamp(mask, 0., 1.);
                      
                      vec3 color = vec3(texture2DRect(tex0,st));
                      
                      color *= mask;
                      
                      gl_FragColor = vec4( color, 1.0);
                  }
                  
                  
                  
                  );
    }
    
    //UPDATE UNIFORMS
    void update(){
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
        
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0);
        shader.setUniform2f("u_resolution", vidDims.x, vidDims.y);
        shader.setUniform1f("u_time", ofGetElapsedTimef());
        
        shader.setUniform1f("dotsAmt",dotsAmt);
        shader.setUniform1f("wavesAmt", wavesAmt);
        shader.setUniform1f("diamondsAmt", diamondsAmt);
        shader.setUniform1f("crosses1Amt", crosses1Amt);
        shader.setUniform1f("crosses2Amt", crosses2Amt);
        shader.setUniform1f("crosses3Amt", crosses3Amt);
        shader.setUniform1f("chevronsAmt", chevronsAmt);
        shader.setUniform1f("trapezoids1Amt", trapezoids1Amt);
        shader.setUniform1f("trapezoids2Amt", trapezoids2Amt);
        shader.setUniform1f("squeezeBoxesAmt", squeezeBoxesAmt);
        
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
    };
    
    //video dimensions
    ofVec2f vidDims = ofVec2f(400.0, 400.0); //default vine size 400x400
    
    float dotsAmt = 0, wavesAmt = 0, diamondsAmt = 0, crosses1Amt = 0, crosses2Amt = 0, crosses3Amt = 0, chevronsAmt = 0, trapezoids1Amt = 0, trapezoids2Amt = 0, squeezeBoxesAmt = 0;
    
};