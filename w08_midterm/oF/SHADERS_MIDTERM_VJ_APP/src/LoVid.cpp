//
//  LoVid.cpp
//  lovid_korea_test_2
//
//  Created by Tyler Henry on 9/19/15.
//
//

#include "LoVid.h"

LoVid::LoVid(string _path){
    
    path = _path; //video filename
    
    vid.loadMovie(path); //load the video file
    
    
    w = vid.getWidth();
    h = vid.getHeight();
    //numPix = vid.getWidth() * vid.getHeight() * 3; // total pix * 3 channels (rgb)
    
    // default pos (0,0)
    pos.x = 0.0;
    pos.y = 0.0;
    
    // default draw size (video file width, height)
    size.x = w;
    size.y = h;
    
    pixelation = w; // default pixelation to match width (i.e. no pixelation)
    
    vid.setVolume(0.0); // mute video
    
    tint = ofColor(255,255,255);
    
}

void LoVid::update(){
    
    vid.update();
    
    // adjust tint based on current saturation
    float lerpAmt = ofMap(saturation, 0.0, 2.0, 1.0, 0.0);
    tint = tintColor.getLerped(white, lerpAmt); //linear interpolation
    
}

void LoVid::draw(){
    
    ofPushStyle();
    
        ofSetColor(tint); // change tint
        vid.draw(pos.x, pos.y, size.x, size.y);
        
    ofPopStyle();
}

bool LoVid::isFrameNew(){
    return vid.isFrameNew();
}

ofTexture& LoVid::getTextureReference(){
    return vid.getTextureReference();
}


/*----------------------------------------
------------------------------------------
// Video FX
------------------------------------------
----------------------------------------*/

/*----------------------------------------
 // Pattern Masks
 ----------------------------------------*/

void LoVid::incPattern(int pattern, bool increase){
    
    switch (pattern){
        case 0:
        {
            if (increase){
                dotsAmt+=0.05;
            } else {
                dotsAmt-=0.05;
            }
            dotsAmt = ofClamp(dotsAmt, 0.0, 1.0);
        }
            break;
        case 1:
        {
            if (increase){
                wavesAmt+=0.05;
            } else {
                wavesAmt-=0.05;
            }
            wavesAmt = ofClamp(wavesAmt, 0.0, 1.0);
        }
            break;
        case 2:
        {
            if (increase){
                diamondsAmt+=0.05;
            } else {
                diamondsAmt-=0.05;
            }
            diamondsAmt = ofClamp(diamondsAmt, 0.0, 1.0);
        }
            break;
        case 3:
        {
            if (increase){
                crosses1Amt+=0.05;
            } else {
                crosses1Amt-=0.05;
            }
            crosses1Amt = ofClamp(crosses1Amt, 0.0, 1.0);
        }
            break;
        case 4:
        {
            if (increase){
                crosses2Amt+=0.05;
            } else {
                crosses2Amt-=0.05;
            }
            crosses2Amt = ofClamp(crosses2Amt, 0.0, 1.0);
        }
            break;
        case 5:
        {
            if (increase){
                crosses3Amt+=0.05;
            } else {
                crosses3Amt-=0.05;
            }
            crosses3Amt = ofClamp(crosses3Amt, 0.0, 1.0);
        }
            break;
        case 6:
        {
            if (increase){
                chevronsAmt+=0.05;
            } else {
                chevronsAmt-=0.05;
            }
            chevronsAmt = ofClamp(chevronsAmt, 0.0, 1.0);
        }
            break;
        case 7:
        {
            if (increase){
                trapezoids1Amt+=0.05;
            } else {
                trapezoids1Amt-=0.05;
            }
            trapezoids1Amt = ofClamp(trapezoids1Amt, 0.0, 1.0);
        }
            break;
        case 8:
        {
            if (increase){
                trapezoids2Amt+=0.05;
            } else {
                trapezoids2Amt-=0.05;
            }
            trapezoids2Amt = ofClamp(trapezoids2Amt, 0.0, 1.0);
        }
            break;
        case 9:
        {
            if (increase){
                squeezeBoxesAmt+=0.05;
            } else {
                squeezeBoxesAmt-=0.05;
            }
            squeezeBoxesAmt = ofClamp(squeezeBoxesAmt, 0.0, 1.0);
        }
            break;
    }
    
}


/*----------------------------------------
// Position
----------------------------------------*/

void LoVid::setCenterPos(ofVec2f cPos){
    pos.x = cPos.x - size.x * 0.5;
    pos.y = cPos.y - size.y * 0.5;
}

ofVec2f LoVid::getCenterPos(){
    ofVec2f cPos;
    cPos.x = pos.x + size.x * 0.5;
    cPos.y = pos.y + size.y * 0.5;
    return cPos;
}

/*----------------------------------------
// Scale
----------------------------------------*/

void LoVid::scale(float pct){ // relative to current size
    
    ofVec2f oldCPos = this->getCenterPos();
    
    size *= pct;
    
    // alter position to keep center
    this->setCenterPos(oldCPos);
    
}

void LoVid::scaleTo(float width){
    float pct = width/size.x;
    this->scale(pct);
}

void LoVid::scaleTo(float width, float height){ // scale to defined size in pixels
    ofVec2f oldCPos = this->getCenterPos();

    size.x = width;
    size.y = height;
    
    // alter position to keep center
    this->setCenterPos(oldCPos);
}

void LoVid::incScale(bool increase){
    if (increase){
        // max width = screen width, max height = screen height
        if ((size.x <= ofGetWidth()-10) && (size.y <= ofGetHeight()-10)){
            scaleTo(size.x+10.0);
        }
    } else {
        if (size.x >= 110){ // min width = 100px
            scaleTo(size.x-10.0);
        }
    }
    
    calcFx();

    ofLogVerbose("Scaled") << path << " to w,h: " << size.x << "," << size.y;
}

/*----------------------------------------
// Speed
----------------------------------------*/

void LoVid::setSpeed(float _speed){
    speed = _speed;
    vid.setSpeed(speed);
    ofLogVerbose("Set speed") << path << " to %: " << speed;
}

void LoVid::incSpeed(bool increase){
    
    if (increase){
        speed += 0.1;
    } else {
        speed -= 0.1;
    }
    speed = ofClamp(speed, -2.0, 2.0);
    vid.setSpeed(speed);
    
    ofLogVerbose("Set speed") << path << " to %: " << speed;
}

/*----------------------------------------
// Tint
----------------------------------------*/

void LoVid::setTint(ofColor _tint){
    tint = _tint;
    ofLogVerbose("Set tint") << path << " to: " << tint;
}

void LoVid::incTint(bool increase){
    // change tint
    // tintVal = -1 (magenta) to 1 (cyan)
    
    if (increase){
        tintVal += 0.1; // closer to cyan
    } else {
        tintVal -= 0.1; // closer to magenta
    }
    tintVal = ofClamp(tintVal,-1.0,1.0); // clamp at magenta or cyan
    
    // interpolate tint based on tintVal
    if (tintVal == 0){
        tintColor = white;
    }
    else {
        // adjust tint based on tintVal
        
        if ((tintVal >= -1.0) && (tintVal < 0.0)){
            float lerpAmt = abs(tintVal);
            tintColor = white.getLerped(magenta,lerpAmt); // lerp to magenta
        }
        else if ((tintVal > 0.0) && (tintVal <= 1.0)){
            float lerpAmt = tintVal;
            tintColor = white.getLerped(cyan,lerpAmt); // lerp to cyan
        }
        else {
            // tintVal outside of range - weird
            ofLogError("Tinted") << path << " tried to use tintVal " << tintVal << ", outside of -1.0,1.0 tintVal range";
            
            tintVal = 0; // reset tintVal
            tintColor = white; // set tint to white
        }
    }
    
    calcFx();
    
    ofLogVerbose("Tinted") << path << " to: " << tintColor << ", tintVal: " << tintVal;
    
}

/*----------------------------------------
// Contrast
----------------------------------------*/

void LoVid::incContrast(bool increase){
    // contrast range: 0.25 to 4.0
    
    if (increase){
        contrast += 0.1;
    } else {
        contrast -= 0.1;
    }
    contrast = ofClamp(contrast, 0.25, 4.0);
    
    calcFx();
    
    ofLogVerbose("Contrasted") << path << " to: " << contrast;
}

/*----------------------------------------
// Saturation
----------------------------------------*/

void LoVid::incSaturation(bool increase){
    // saturation range: 0.0 to 2.0
    
    if (increase){
        saturation += 0.05;
    } else {
        saturation -= 0.05;
    }
    saturation = ofClamp(saturation, 0.0, 2.0);
    
    calcFx();
    
    ofLogVerbose("Saturated") << path << " to: " << saturation;
}

/*----------------------------------------
// Pixelation
----------------------------------------*/

void LoVid::incPixelation(bool increase){
    // pixelation range: 3 to [video width] subdivisions on x axis
    
    float delta = ofMap(pixelation, 0, w, 1, 30); // amount to change pixelation
    if (!increase){
        if (pixelation <= w - delta){ // min pixelation = width of video
            pixelation += delta;
        } else {
            pixelation = w;
        }
    } else {
        if (pixelation >= 3 + delta){ // max pixelation = 3 subdivisions in w and h
            pixelation -= delta;
        } else {
            pixelation = 3;
        }
    }
    
    calcFx();

    ofLogVerbose("Pixelated") << path << " to divisions: " << pixelation;
}

/*----------------------------------------
 // Glitch
 ----------------------------------------*/

void LoVid::incGlitch(bool increase){
    // glitch range: 0.0 to 1.0
    
    if (increase){
        glitch += 0.025;
    } else {
        glitch -= 0.025;
    }
    glitch = ofClamp(glitch, 0.0, 1.0);
    
    calcFx();
    
    ofLogVerbose("Glitch") << path << " to: " << glitch;
}

/*----------------------------------------
 // Return average total effect
 ----------------------------------------*/

float LoVid::getTotalFx(){
    
    return totalFx;
    
}

void LoVid::calcFx(){
    
    // glitch: 0 - 1 (0 normal)
    // pixelation: 3 - 400 (400 normal)
    // saturation: 0 - 2 (1 normal)
    // contrast: 0.25 - 4 (1 normal)
    // tint: -1 - 1 (0 normal)
    // scale: 100h - ofGetHeight() (400 normal)
    
    // map all fx 0-1
    float glitchAmt = glitch;
    float pixAmt = ofMap(pixelation, 3, 400, 1, 0, true);
    float satAmt = ofMap(saturation, 0, 2, -1, 1, true);
    satAmt = abs(satAmt);
    float contAmt = 0;
    if (contrast < 1){
        contAmt = ofMap(contrast, 0.25, 1, 1, 0, true);
    } else if (contrast > 1){
        contAmt = ofMap(contrast, 1, 4, 0, 1, true);
    }
    float tintAmt = abs(tintVal);
    float scaleAmt = size.x / 400;
    if (scaleAmt == 1){
        scaleAmt = 0;
    } else if (scaleAmt > 1){
        scaleAmt = ofMap(scaleAmt, 1, 2, 0, 1, true);
    } else {
        scaleAmt = ofMap(scaleAmt, 0.25, 1, 1, 0, true);
    }
    
    // average
    totalFx = (glitchAmt + pixAmt + satAmt + contAmt + tintAmt + scaleAmt)/6;
    
}
