//
//  easyShader.cpp
//  shaderFinalPrototype_ofxSyphon
//
//  Created by Tyler on 12/2/15.
//
//

#include "easyShader.hpp"

EasyShader::EasyShader(){};

EasyShader::EasyShader(string _path, ofVec2f _pos, ofVec2f _size){
    
    init(_path, _pos, _size);
    
}

EasyShader::EasyShader(string _path){
    
    init(_path, ofVec2f(ofGetWidth()/2,ofGetHeight()/2), ofVec2f(200));
    
}

//----- private common constructor code -----//

void EasyShader::init(string _path, ofVec2f _pos, ofVec2f _size){
    
    shaderPath = _path;
    shader.load("", shaderPath);
    shaderSource = ofBufferFromFile(shaderPath);
    
    //gui
    gui = new ofxDatGui(_pos.x, _pos.y + _size.y);
    gui->addHeader(_path);
    gui->setOpacity(0.5);
    timeSpeed = gui->addSlider("time speed", 0, 100, 50);
    mouseXSlider = gui->addSlider("u_mouse.x", 0, 1, 0);
    mouseYSlider = gui->addSlider("u_mouse.y", 0, 1, 0);
    
    //ofRectangle bounds(0,0,size.x,size.y);
    //mousePad = gui->add2dPad("u_mouse", bounds);
    //gui->on2dPadEvent(this, &EasyShader::on2dPadEvent);

    findUniforms();
    
    //add custom uniforms per shader:
    
    for (int i=0; i<uniforms.size(); i++){
        
        //add float uniforms (normalized)
        if ((uniforms[i].type == "float") && (uniforms[i].name != "u_time")){
            gui->addSlider(uniforms[i].name, 0, 1, 0);
        }
    }

    setPos(_pos.x, _pos.y);
    
    size.x = _size.x;
    size.y = _size.y;
    
    origPos = pos;
    origSize = size;
    
    output.allocate(size.x,size.y);
    
    for (int i=0; i<nTexInputs; i++){
        ofTexture tex;
        tex.allocate(1000,1000,GL_RGB);
        ofImage dTex;
        dTex.load("defaultTex.png");
        tex = dTex.getTexture();
        textures.push_back(tex);
    }
    
    updateGui();
    
    
}


//----- update/draw -----//

void EasyShader::update(){
    
    float ts = timeSpeed->getValue();
    if (ts < 50) {
        timeSpeedMapped = ofMap(ts, 0, 50, 0, 1);
    } else if (ts >= 50){
        timeSpeedMapped = ofMap(ts, 50, 100, 1, 100);
    }
    
    uMouseX = mouseXSlider->getValue();
    uMouseY = mouseYSlider->getValue();
    uMouseX = ofMap(uMouseX, 0, 1, 0, size.x);
    uMouseY = ofMap(uMouseY, 0, 1, 0, size.y);
    
}

void EasyShader::setTimeSpeed(float _timeSpeed){
    
    timeSpeedMapped = _timeSpeed;
    
}

void EasyShader::updateFbo(){
    
    output.begin();
        shader.begin();
        shader.setUniform2f("u_resolution", size.x, size.y);
        shader.setUniform1f("u_time", ofGetElapsedTimef() * timeSpeedMapped);
        shader.setUniform2f("u_mouse", uMouseX, uMouseY);
    
        //update custom uniforms
        for (int i=0; i<uniforms.size(); i++){
            
            if ((uniforms[i].type == "float") && (uniforms[i].name != "u_time")){
                string uName = uniforms[i].name;
                float uVal = gui->getSlider(uniforms[i].name)->getValue();
                shader.setUniform1f(uName, uVal);
                //cout << "set uniform " << uName << " to " << uVal << endl;
            }

        }
    
        for (int i=0; i<nTexInputs; i++){
            string texName = "u_tex";
            texName = texName.append(ofToString(i));
            shader.setUniformTexture(texName, textures[i], i);
            shader.setUniform2f(texName.append("resolution"), textures[i].getWidth(), textures[i].getHeight());
        }
    
        ofClear(0, 0, 0);
        ofDrawRectangle(0,0,size.x,size.y);
    
        shader.end();
    output.end();
    
}


void EasyShader::setTexture(ofTexture _tex, int nTex){
    
    
    textures[nTex].allocate(_tex.getWidth(), _tex.getHeight(), GL_RGB);
    textures[nTex] = _tex;
    
}

void EasyShader::draw(){
    
    updateFbo();
    
    //draw fbo
    
    output.draw(pos.x,pos.y+size.y,size.x,-size.y); //invert y
}

// ----------------------------------------------------------- */
/* Note: the draw3D and drawPlane functions aren't working yet */
/* ---------- only 2D drawing is available as of now --------- */
// ----------------------------------------------------------- */

void EasyShader::draw3D(){
    
    updateFbo();
    
    //draw fbo
    
    output.draw(zPos.x,zPos.y+size.y,size.x,-size.y); //invert y
    
}

void EasyShader::drawPlane(){
    
    updateFbo();

    //draw fbo to a plane on screen
    //shader file must have gl_TexCoord
    
    shader.begin();
    
        shader.setUniform2f("u_resolution", size.x, size.y);
        shader.setUniform1f("u_time", ofGetElapsedTimef());
        
        ofDrawPlane(zPos.x, zPos.y, size.x, size.y); //auto-inverted y
    
    shader.end();
}
/* ----------^^^ above functions not working yet ^^^---------- */
// ----------------------------------------------------------- */



void EasyShader::drawGui(){
    
    if (selected){
        ofPushStyle();
            ofNoFill();
            ofSetColor(255,125);
            ofDrawRectangle(pos.x,pos.y,size.x,size.y);
        
            if (bIn){ ofFill(); } else { ofNoFill(); }
            ofDrawRectangle(inBox);
        
            if (bOut){ ofFill(); } else { ofNoFill(); }
            ofDrawRectangle(outBox);

        ofPopStyle();
        gui->setVisible(true);
        
        //tex.draw(0,0,100,100);
        
    } else {
        gui->setVisible(false);
    }
}

void EasyShader::updateGui(){
    
    gui->setOrigin(pos.x,pos.y+size.y);
    
    //ofRectangle bounds(0,0,size.x,size.y);
    //mousePad->setBounds(bounds);

    inBox.set(pos.x-10, pos.y, 10, size.y);
    outBox.set(pos.x+size.x, pos.y, 10, size.y);
    
}

//----- pos/size manipulation -----//

void EasyShader::setPos(float x, float y){
    
    pos.x = x;
    pos.y = y;
    
    //update 3D position
    zPos.x = pos.x - ofGetWidth()/2;
    zPos.y = pos.y - ofGetHeight()/2;
    
    updateGui();
}

void EasyShader::setCPos(float x, float y){
 
    setPos(x - size.x/2, y - size.y/2);
}

void EasyShader::setZPos(float x, float y){
    
    zPos.x = x;
    zPos.y = y;
    
    //update 2D position
    pos.x = zPos.x - size.x/2 + ofGetWidth()/2;
    pos.y = zPos.y - size.y/2 + ofGetHeight()/2;
    
}

ofVec2f EasyShader::contains(float x, float y){
    
    if (x >= pos.x && y >= pos.y &&
        x <= pos.x + size.x && y <= pos.y + size.y){
        return ofVec2f(x-pos.x,y-pos.y); //return point relative to pos
    }
    
    return ofVec2f(-1,-1);
}

bool EasyShader::insideInBox(float x, float y){
    
    return inBox.inside(x,y);
}

bool EasyShader::insideOutBox(float x, float y){
    
    return outBox.inside(x,y);
}

void EasyShader::resetPos(){
    
    setPos(origPos.x, origPos.y);
}

void EasyShader::resetSize(){
    ofVec2f cPos = ofVec2f(pos.x + size.x/2, pos.y + size.y/2);
    size = origSize;
    setCPos(cPos.x,cPos.y);
    
    output.allocate(size.x,size.y);
}

void EasyShader::scale(float scalar){
    
    //save center position
    ofVec2f cPos = ofVec2f(pos.x + size.y/2, pos.y + size.y/2);
    
    ofVec2f newSize = size * scalar;
    
    if ((newSize.x > 1) && (newSize.y > 1)){
        size = newSize;
        setCPos(cPos.x, cPos.y);
        output.allocate(size.x,size.y);
    }
    
}


void EasyShader::select(){
    
    selected = true;
}

void EasyShader::deselect(){
    
    selected = false;
}

bool EasyShader::isSelected(){
    return selected;
}

int EasyShader::getIncTexN(){
    
    
    int cTexN = currentTexN;
    
    //increment currentTexN
    currentTexN++;
    
    if (currentTexN > nTexInputs){
        currentTexN = 0; //loop to first texture
    }
    
    return cTexN;
    
}

void EasyShader::findUniforms(){

    ofBuffer::Lines lines = shaderSource.getLines();
    
    for (ofBuffer::Line line=lines.begin(); line!=lines.end(); line++){
        string s = line.asString();
        s = trim(s);
        if (s.find("uniform") == 0)
        {
            vector<string> ss = ofSplitString(s," ", true, false);
            
            if ((ss[2] != "u_resolution") && (ss[2] != "u_time") && (ss[2] != "u_mouse")){
                
                //add unique uniform to uniforms vector
                Uniform u;
                u.type = ss[1];
                u.name = ss[2];
                uniforms.push_back(u);
                
                if (u.type == "sampler2D"){
                    nTexInputs++;
                }
            }

        }
    }
    
    for (int i=0; i<uniforms.size(); i++){
        if (i==0){
            cout << endl << shaderPath << endl
            << "------------------------" << endl
            << "unique uniforms:" << endl;
        }
        cout << uniforms[i].type << " " << uniforms[i].name << endl;
        if (i==uniforms.size()-1){
            cout << "------------------------" << endl;
            cout << "# texture inputs: " << nTexInputs << endl << endl;
        }
    }
}

void EasyShader::on2dPadEvent(ofxDatGui2dPadEvent e)
{
    //uMouseX = e.x;
    //uMouseY = e.y;
    
    cout << "on2dPadEvent: " << e.target->getLabel() << " " << e.x << ":" << e.y << endl;
    
    uMouseX = e.x;
    uMouseY = e.y;
}

