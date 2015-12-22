#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    ofDisableArbTex();
    
    addShader("sunOil_fbo.frag",ofVec2f(0,0),ofVec2f(200,200));
    
    addShader("texture_oil.frag");

    //----- GUI -----//
    ofxDatGuiLog::quiet();
    
    //ofSetLogLevel(OF_LOG_VERBOSE);

}

//--------------------------------------------------------------
void ofApp::update(){
    

    
    for (int i=0; i<patches.size(); i++){
        
        int f = patches[i].from;
        int t = patches[i].to;
        int n = patches[i].texN;
        
        shaders[t].setTexture(shaders[f].output.getTexture(), n);
        
        if (checkTextures){
            cout << endl << "textures to " << t << ": " << endl;
            cout << "tex" << n << " from " << f << endl;
            cout << "(shader " << t << " accepts " << shaders[t].nTexInputs << " textures)" << endl << endl;
        }
    
    }
    checkTextures = false;
    
    for (int i=0; i<shaders.size(); i++){
        shaders[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for (int i=0; i<shaders.size(); i++){
        shaders[i].draw();
    }
    
    syphon.publishScreen();
    
    //----- GUI -----//
    for (int i=0; i<shaders.size(); i++){
        shaders[i].drawGui();
    }
    
    //draw patch "cable" if patching
    if (bPatching){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        if (selectedShader >= 0){
            ofVec2f start;
            start.x = shaders[selectedShader].pos.x + shaders[selectedShader].size.x/2;
            start.y = shaders[selectedShader].pos.y + shaders[selectedShader].size.y/2;
            ofDrawLine(start.x,start.y,ofGetMouseX(),ofGetMouseY());
        }
        ofPopStyle();
    }
                   
    //draw all patches
    
    for (int i=0; i<patches.size(); i++){
        ofPushStyle();
        ofFill();
        ofSetColor(255);
        
        ofVec2f start = shaders[patches[i].from].outBox.getCenter();
        ofVec2f end = shaders[patches[i].to].inBox.getCenter();
        end.y += patches[i].texN * 10;
        ofDrawLine(start, end);
        
        ofPopStyle();
    }
}


//--------------------------------------------------------------
void ofApp::addShader(string path, ofVec2f pos, ofVec2f size){
    
    EasyShader shader = EasyShader(path,pos,size);
    shaders.push_back(shader);
    
}

//--------------------------------------------------------------
void ofApp::addShader(string path){
    
    EasyShader shader = EasyShader(path);
    shaders.push_back(shader);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
            
        case 'r':
        {
            for (int i=0; i<shaders.size(); i++){
                shaders[i].resetSize();
                shaders[i].resetPos();
            }
            break;
        }
            
        case 's':
        {
            for (int i=0; i<shaders.size(); i++){
                shaders[i].resetSize();
            }
            break;
        }
            
        case 'p':
        {
            bPatching = !bPatching;
            break;
        }
            
        case 't':
        {
            checkTextures = true;
            break;
        }
            
        case OF_KEY_UP:
        {
            for (int i=0; i<shaders.size(); i++){
                if (shaders[i].isSelected()){
                    shaders[i].scale(1.1);
                }
            }
            break;
        }
            
        case OF_KEY_DOWN:
        {
            for (int i=0; i<shaders.size(); i++){
                if (shaders[i].isSelected()){
                    shaders[i].scale(0.9);
                }
            }
            break;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if (shaderDrag >= 0){
        shaders[shaderDrag].setPos(x-shaderDragPos.x,y-shaderDragPos.y);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    for (int i=0; i<shaders.size(); i++){

            shaderDragPos = shaders[i].contains(x,y);
            
            if (shaderDragPos.x >= 0){
                
                // a shader is clicked
                
                // if we're creating a patch
                
                if (bPatching){
                    
                    //if user clicks selected shader
                    
                    if (i == selectedShader){
                        
                        //delete patches from that shader
                        
                        for (int j=0; j<patches.size(); j++){
                            if (patches[j].from == selectedShader){
                                patches.erase(patches.begin()+j);
                            }
                        }
                    }
            
                    //if user clicks a different shader
                    
                    else {
                
                        //make patch from selected shader to shader clicked (i)
                        
                        if (shaders[i].nTexInputs > 0){
                            
                            //if clicked shader accepts textures
                        
                            Patch p;
                            p.from = selectedShader;
                            p.to = i;
                            p.texN = shaders[i].getIncTexN();
                            //get the next texture available
                            cout << "patching to tex# " << p.texN << endl;
                            
                            bool makePatch = true;
                            
                            //check for identical patches
                            for (int j=0; j<patches.size(); j++){
                                if ((patches[j].from == selectedShader) &&
                                    (patches[j].to == i) &&
                                    (patches[j].texN == p.texN)){
                                    
                                    makePatch = false; //don't make patch
                                    
                                    cout << "patch already exists" << endl;
                                }
                            }
                        
                            //if no identical patches
                            if (makePatch){
                            
                                //check for current patch to that texture input
                                //and remove it if exists
                                
                                for (int j=0; j<patches.size(); j++){
                                    if ((patches[j].to == i) && (patches[j].texN == p.texN)){
                                        
                                        cout << "removing patch from " << patches[j].from
                                        << " to " << patches[j].to << " at tex " << patches[j].texN << endl;
                                        
                                        patches.erase(patches.begin()+j);
                                    }
                                }
                        
                                patches.push_back(p);
                                
                                cout << "made patch from " << selectedShader << " to " << i << " in texture # " << p.texN << endl;
                            }
                        }
                    
                        bPatching = false;
                    
                    }
                
                
            } else {
            
                shaderDrag = i;
                shaders[i].select(); //select this shader
                selectedShader = i;
                
                for (int j=0; j<shaders.size(); j++){
                    //deselect all other shaders
                    if (j != i){
                        shaders[j].deselect();
                    }
                }
                
                cout << "selected shader " << shaderDrag << endl;
                break;
            }
        }

        /*
        if (shaders[i].isSelected()){
            if (shaders[i].insideInBox(x,y)){
                shaders[i].bIn = true;
            }
            if (shaders[i].insideOutBox(x,y)){
                shaders[i].bOut = true;
            }
        }
        */
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    if (shaderDrag >= 0){
        shaders[shaderDrag].setPos(x-shaderDragPos.x,y-shaderDragPos.y);
        cout << "dragged shader " << shaderDrag << " to: " << x << "," << y << endl;
        shaderDrag = -1;
        shaderDragPos.set(-1,-1);
    }
    
    for (int i=0; i<shaders.size(); i++){
        if ((shaders[i].isSelected()) && (fboDrag<0)){
            if (shaders[i].insideInBox(x,y)){
                shaders[i].bIn = false;
            }
            if (shaders[i].insideOutBox(x,y)){
                shaders[i].bOut = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
    cout << "Dragged in:" << endl << "---------------" << endl;
    for (int i=0; i<dragInfo.files.size(); i++){
        cout << dragInfo.files[i] << endl;
        addShader(dragInfo.files[i]);
    }
    cout << "---------------" << endl;
    
}
