//
//  LoVid.h
//  lovid_korea_test_2
//
//  Created by Tyler Henry on 9/19/15.
//
//
//  Class contains an oFVideoPlayer object and
//  variables and functions for video manipulation
//
//  Most video effects are done in ofApp.cpp
//  through ofxLovid.h shader using ofxFX addon
//
//  Tint is done by calling ofSetColor(tint)
//  before drawing the shader in ofApp.cpp
//
//

#pragma once
#include "ofMain.h"


class LoVid {
public:
    
    LoVid(string _path);
    
    void update();
    void draw();
    
    bool isFrameNew();
    ofTexture& getTextureReference();
    
    /*----------------------------------------
    //FX functions
    ----------------------------------------*/
    
    void incPattern(int pattern, bool increase);
    
    void setCenterPos(ofVec2f cPos);
    ofVec2f getCenterPos();
    
    void scale(float pct); // 1.0 is no change / 100%
    void scaleTo(float width); // scale to specified width (w/ proportional height)
    void scaleTo(float width, float height); // scale to specified width and height
    
    void setSpeed(float _speed); // pct 1.0 is normal, -1.0 is backwards
    void setTint(ofColor _tint);
    
    void incScale(bool increase);
    void incTint(bool increase);
    void incContrast(bool increase);
    void incSaturation(bool increase);
    void incPixelation(bool increase);
    void incSpeed(bool increase);
    void incGlitch(bool increase);
    
    void calcFx(); // calculates average FX amt
    float getTotalFx(); // returns avg fx amt
    
    
    ofVideoPlayer vid;

    float w,h,numPix; // video's width, height, RGB count (w*h*3)
    string path; // filename (within /bin/data)
    
    /*----------------------------------------
    //FX variable storage
    ----------------------------------------*/
    
    ofVec2f pos; // draw position in pixels (upper left corner)
    ofVec2f size; // draw size in pixels
    
    float speed = 1.0; // 1.0 is normal, -1.0 is reverse
    
    ofColor tint; // tint with saturation adjustment
    ofColor tintColor = ofColor(255); // tint without saturation adjustment
    float tintVal = 0; // -1.0 is magenta, 0 is white, 1.0 is cyan
    ofColor magenta = ofColor(255,0,255);
    ofColor cyan = ofColor(0,255,255);
    ofColor white = ofColor(255);
    
    float brightness = 1.0; // 0.0 to 2.0 (1.0 is no change)
    float saturation = 1.0; // 0.0 to 2.0 (1.0 is no change)
    float contrast = 1.0; // 0.25 to 4.00 (1.0 is no change)
    
    int pixelation = 1; // # of subdivisions in video width
    float glitch = 0.0; // 0.0 to 1.0, 0.0 is no glitch
    
    float totalFx = 0.0; // deviation from no effect (0.0) out of 1.0
    
    float dotsAmt = 0, wavesAmt = 0, diamondsAmt = 0, crosses1Amt = 0, crosses2Amt = 0, crosses3Amt = 0, chevronsAmt = 0, trapezoids1Amt = 0, trapezoids2Amt = 0, squeezeBoxesAmt = 0; // pattern mask amts 0.-1.
    

};
