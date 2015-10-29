#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    /*----------------------------------------
     // Setup kinect
     ----------------------------------------*/
    
    if (KINECT_ON){
        
        kinectSetup();
    }
    
    ofBackground(0);
    ofSetVerticalSync(true);
    ofSetFrameRate(60); // target fps
    
    if (debug){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }
    
    
    /*----------------------------------------
     // ofxJSON - load vines
     ----------------------------------------*/
    if (VINE_START){
        
        string url = "https://api.vineapp.com/timelines/tags/" + hashtag;
        bool parsingSuccessful = json.open(url); // get/parse the JSON
        
        ofLogVerbose("[JSON]") << url;
        if (parsingSuccessful)
        {
            ofLogVerbose("[JSON]") << json.getRawString(true);
        } else {
            ofLogVerbose("[JSON]") << "Failed to parse JSON.";
        }
        
        records = json["data"]["records"].size();
        
        for (Json::ArrayIndex i = 0; i < records; ++i){
            
            vidUrl  = json["data"]["records"][i]["videoUrl"].asString(); // get video url
            string vidPath = "vine" + ofToString(i) + ".mp4"; // set video file path
            resp = ofSaveURLTo(vidUrl, vidPath); // download video to file
            
            ofLogVerbose("Vine video url: ") << vidUrl;
            ofLogVerbose("Vine video path: ") << vidPath << endl;
            
            vidPaths.push_back(vidPath); // add vidPath to vidPaths[]
        }
    }
    
    
    /*----------------------------------------
     // Get all files in /bin/data
     ----------------------------------------*/
    
    dir.setShowHidden(false); // ignore hidden files
    dirSize = dir.listDir(""); // load ofDirectory bin/data
    files = dir.getFiles(); // load all files into vector<ofFile>
    
    
    /*----------------------------------------
     // Add video files to qVids[] vector
     ----------------------------------------*/
    
    for (int i=0; i<files.size(); i++){
        
        string path = files[i].getFileName(); // get filename
        string ext = files[i].getExtension(); // get extension
        
        // if .mov or .mp4, initialize video and add to qVids[]
        
        if ((ext == "mov") || (ext == "mp4") || (ext == "MOV") || (ext == "MP4")){
            
            LoVid vid = LoVid(path);
            
            ofVec2f centerPos;
            centerPos.x = ofRandom(ofGetWidth());
            centerPos.y = ofRandom(ofGetHeight());
            vid.setCenterPos(centerPos);
            
            qVids.push_back(vid);
            
            ofLogVerbose("Loaded video") << path;
        }
    }
    
    /*----------------------------------------
     // ofxFX shader
     ----------------------------------------*/
    
    fxLovid.allocate(480,480);
    fxLovid.brightness = 1.0;
    
    
    
    
    
    /*----------------------------------------
     // Setup arduino
     ----------------------------------------*/
    
    
    if (ARDUINO_ON){
        arduinoSetup();
        
        // set sensorVals[] to default
        
        for (int i=0; i<NUM_SENSORS; i++){
            
            sensorVals[i] = 0; //0 is default value - i.e. do nothing
            
        }
    }
    
    
    /*----------------------------------------
     // Vine Thread
     ----------------------------------------*/
    
    vineThread.lock();
    // set variables
    vineThread.hashtag = hashtag;
    vineThread.records = records;
    vineThread.unlock();
    
    ofLogVerbose("Set up Vine Thread") << "hashtag: " << hashtag << ", records: " << records;
    
    if (bVineThread){
        vineThread.startThread(true, isLogVerbose());
    }
    
    
    
    /*----------------------------------------
     // Setup sounds
     ----------------------------------------*/
    
    if (SOUND_ON){
        soundSetup();
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    /*----------------------------------------
     // Check for new video files &
     // load them to qVids[]
     ----------------------------------------*/
    
    if (!bNewVid){
        
        // check for new vides in /bin/data
        checkForNewFile();
        
        
    } else {
        
        // if there is a new video file waiting, try loading it into qVids[]
        loadNewFile();
    }
    
    
    /*----------------------------------------
     // Update dVids[]
     ----------------------------------------*/
    
    for (int i=0; i<dVids.size(); i++){
        
        dVids[i].update();
        
    }
    
    /*----------------------------------------
     // Update kinect
     ----------------------------------------*/
    
    if (KINECT_ON){
        
        kinectUpdate();
        
        
        /*----------------------------------------
         // If using kinect centroid for video pos
         // AND kinect got a centroid
         // set the center pos of selected video
         ----------------------------------------*/
        
        if (kinectCentroid && kinectGotCentroid){
            if (vidSelect < dVids.size()){
                dVids[vidSelect].setCenterPos(centroid);
            }
        }
    }
    
    /*----------------------------------------
     // Update arduino
     ----------------------------------------*/
    
    if (ARDUINO_ON){
        
        // get (ideally) readings from all 14 sensors
        
        for (int i=0; i<14; i++){
            
            arduinoUpdate(0);
            arduinoUpdate(1);
            
        }
        
        // process sensor vals
        processSensorVals();
        
    }
    
    /*----------------------------------------
     // Update sounds
     ----------------------------------------*/
    
    if (SOUND_ON){
        soundUpdate();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    
    
    for (int i=0; i<dVidsOrder.size(); i++){ // draw order according to dVidsOrder[]
        
        /*----------------------------------------
         // Draw shader
         ----------------------------------------*/
        
        int v = dVidsOrder[i];
        
        if (v < dVids.size()){ // make sure video is in dVids[]
            
            // video effects
            
            fxLovid.contrast = dVids[v].contrast;
            fxLovid.saturation = dVids[v].saturation;
            
            fxLovid.textureW = dVids[v].w;
            fxLovid.textureH = dVids[v].h;
            fxLovid.divX = dVids[v].pixelation;
            fxLovid.divY = dVids[v].pixelation;
            
            fxLovid.glitch = dVids[v].glitch;
            
            fxLovid.setTexture(dVids[v].getTextureReference());
            fxLovid.update();
            
            /*
            
            ofPushStyle();
            ofSetColor(dVids[v].tint);
            fxLovid.draw(dVids[v].pos.x, dVids[v].pos.y, dVids[v].size.x, dVids[v].size.y);
            ofPopStyle();
             
             */
            
            // pattern mask
            
            fxPatterns.dotsAmt = dVids[v].dotsAmt;
            fxPatterns.wavesAmt = dVids[v].wavesAmt;
            fxPatterns.diamondsAmt = dVids[v].diamondsAmt;
            fxPatterns.crosses1Amt = dVids[v].crosses1Amt;
            fxPatterns.crosses2Amt = dVids[v].crosses2Amt;
            fxPatterns.crosses3Amt = dVids[v].crosses3Amt;
            fxPatterns.chevronsAmt = dVids[v].chevronsAmt;
            fxPatterns.trapezoids1Amt = dVids[v].trapezoids1Amt;
            fxPatterns.trapezoids2Amt = dVids[v].trapezoids2Amt;
            fxPatterns.squeezeBoxesAmt = dVids[v].squeezeBoxesAmt;
            
            fxPatterns.setTexture(fxLovid.getTextureReference());
            fxPatterns.update();
            
            // draw shader
            
            ofPushStyle();
            ofSetColor(dVids[v].tint);
            fxPatterns.draw(dVids[v].pos.x, dVids[v].pos.y, dVids[v].size.x, dVids[v].size.y);
            ofPopStyle();
        }
        
        else {
            
            // video not in dVids[] - throw error
            ofLogError("Vid draw") << "video " << v << "not in dVids[]" << endl;
        }
    }
    
    
    
    // draw selection box on top of selected video
    if (selectDrawCnt > 0){
        drawSelection(vidSelect);
    }
    
    
    // draw kinect is drawKinect boolean true
    
    if (drawKinect){
        ofVec2f pos = ofVec2f(0,0);
        kinectDraw(pos);
    }
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    vineThread.stopThread();
}

//--------------------------------------------------------------
void ofApp::kinectSetup(){
    
    /*----------------------------------------
     // Setup kinect
     ----------------------------------------*/
    
    // depth + video image calibration
    kinect.setRegistration(true);
    
    // initialize kinect
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.init();
    
    kinect.open();		// opens first available kinect
    
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogVerbose() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogVerbose() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogVerbose() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogVerbose() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    //kinect imagery
    
    kinectColorImg.allocate(kinect.width, kinect.height);
    kinectGrayImage.allocate(kinect.width, kinect.height);
    kinectThreshNear.allocate(kinect.width, kinect.height);
    kinectThreshFar.allocate(kinect.width, kinect.height);
    
    //default centroid
    centroid.x = ofGetWidth()/2;
    centroid.y = ofGetHeight()/2;
}

//--------------------------------------------------------------
void ofApp::kinectUpdate(){
    
    /*----------------------------------------
     // Update kinect
     ----------------------------------------*/
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        kinectGrayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        kinectThreshNear = kinectGrayImage;
        kinectThreshFar = kinectGrayImage;
        kinectThreshNear.threshold(nearThreshold, true);
        kinectThreshFar.threshold(farThreshold);
        cvAnd(kinectThreshNear.getCvImage(), kinectThreshFar.getCvImage(), kinectGrayImage.getCvImage(), NULL);
        
        // update the cv images
        kinectGrayImage.flagImageChanged();
        
        // find contours which are between the size of 100 and 76800 pixels, find 1 blob, with no holes.
        contourFinder.findContours(kinectGrayImage, 100, 76800, 1, false);
        
        if (kinectCentroid){
            // get centroid of blob
            if (contourFinder.blobs.size() > 0){
                centroid = contourFinder.blobs[0].boundingRect.getCenter();
                
                // map to screen space
                centroid.x = ofMap(centroid.x, 0, kinect.getWidth(), ofGetWidth(), 0, true); // invert, clamp at screen space
                centroid.y = ofMap(centroid.y, 0, kinect.getHeight(), 0, ofGetHeight(), true); // clamp at screen space
                
                kinectGotCentroid = true;
                
            } else {
                //no centroid
                
                kinectGotCentroid = false;
            }
        }
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::kinectDraw(ofVec2f pos){
    
    /*----------------------------------------
     // Draw kinect
     ----------------------------------------*/
    
    ofPushStyle();
    
    ofSetColor(255);
    
    // draw from the live kinect
    //    kinect.drawDepth(10, 10, 400, 300);
    //    kinect.draw(420, 10, 400, 300);
    
    kinectGrayImage.draw(pos.x, pos.y);
    //contourFinder.draw(10, 320, 400, 300);
    
    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(pos.x,pos.y);
        
        // draw the centroid
        
        ofSetColor(0,255,0);
        ofVec2f _centroid = contourFinder.blobs[i].boundingRect.getCenter() + pos;
        
        ofCircle(_centroid, 5);
        
        ofSetColor(255,0,0);
        
        ofCircle(centroid, 5);
        
    }
    
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::arduinoSetup(){
    
    /*----------------------------------------
     // Setup Arduino serial connection
     ----------------------------------------*/
    
    //    serial.listDevices(); //list devices
    //
    //    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList(); //save the list
    
    int baud = 9600;
    
    //    serial.setup(0, baud); // open the first device
    
    serial_cap.setup(ARDUINO_SERIAL_CAP, baud); // open by string name of port
    serial_bb.setup(ARDUINO_SERIAL_BB, baud); //open beam breaks
    
    // ADD THIS: if device not inited, don't run arduinoUpdate()
    
    
}

//--------------------------------------------------------------
void ofApp::arduinoUpdate(int _serial){
    
    /*----------------------------------------
     // Update Arduino serial connection
     // grabs current header and value
     ----------------------------------------*/
    
    //_serial == 0 --> read capacitive serial
    //_serial == 1 --> read beam break serial
    
    
    
    int nRead  = 0;  // a temp variable to keep count per read
    
    unsigned char bytesReturned[3]; // temp bytes read storage
    
    memset(bytesReturned, 0, 3); // clear bytes storage
    
    // read serial into bytesReturned
    
    if (_serial == 0){
        while( (nRead = serial_cap.readBytes( bytesReturned, 3)) > 0){
            nBytesRead = nRead;
        };
    } else {
        while( (nRead = serial_bb.readBytes( bytesReturned, 3)) > 0){
            nBytesRead = nRead;
        };
    }
    
    
    ofLogVerbose("Arduino Serial") << "read " << nBytesRead << " bytes";
    
    
    // process serial data if correct number of bytes
    
    if (nBytesRead == 3){
        
        // convert first byte to header char
        arduinoHeader = bytesReturned[0];
        
        // convert last two bytes to int
        int int1 = (int)bytesReturned[1];
        int int2 = (int)bytesReturned[2];
        
        // convert last two bytes into one 16-bit int
        arduinoValue = int2 * 256 + int1;
        
        
        // if valid header ('A' - 'O')
        
        if ((arduinoHeader >= 65) && (arduinoHeader <= 79)) {
            
            ofLogVerbose("Arduino") << "header: " << arduinoHeader << ", value: " << arduinoValue;
            
            if (arduinoValue >= 500){
                ofLogNotice("Arduino") << "header: " << arduinoHeader << ", value: " << arduinoValue;
            }
            
            // if the header is for the cap sensors
            // clamp the values at 250 - 1000
            
            if (arduinoHeader >= 70){
                arduinoValue = ofClamp(arduinoValue, 250, 1500);
                //arduinoValue = 0;
            }
            
        } else {
            
            // Invalid header
            
            ofLogError("Arduino") << "received invalid header: " << arduinoHeader << ", value: " << arduinoValue;
            
            // set default value
            arduinoHeader = 'X';
            arduinoValue = 0;
        }
        
        
        //save valid value in sensorVals array
        
        if (arduinoHeader != 'X'){
            int ardH = (int)arduinoHeader;
            sensorVals[ardH-65] = arduinoValue;
        }
        
    }
    
    
    nBytesRead = 0; // reset
    
    if (_serial == 0){
        serial_cap.flush(); // flush serial port so it doesn't get backed up
    } else {
        serial_bb.flush();
    }
    
}

//--------------------------------------------------------------
void ofApp::soundSetup(){
    
    
    for (int i=0; i<files.size(); i++){
        
        string path = files[i].getFileName(); // get filename
        string ext = files[i].getExtension(); // get extension
        
        // if .aif, initialize video and add to sounds[]
        
        if ((ext == "aif") || (ext == "AIF")){
            
            ofSoundPlayer sound;
            sound.loadSound(path);
            
            sounds.push_back(sound);
            sounds.back().setLoop(true); // set to auto-loop
            sounds.back().play(); // init play
            sounds.back().setPaused(true); // pause
            
            ofLogVerbose("Loaded sound") << path;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::soundUpdate(){
    
    bool updateVolume = false;
    bool updateSpeed = false;
    
    // get volume based on % drawn vids / total vids
    
    float totalVids = qVids.size() + dVids.size();
    float drawVids = dVids.size();
    
    if (totalVids > 0){
        float pVol = soundVolume;
        soundVolume = drawVids / totalVids;
        
        updateVolume = (pVol != soundVolume); // only update if diff
    }
    
    // get speed based on amount of effects on vidSelect
    
    if (vidSelect < dVids.size()){ // only update if there's a dVid
        
        float pSpeed = soundSpeed;
        
        soundSpeed = dVids[vidSelect].getTotalFx() + 1.0; //scale to 1.0 - 2.0
        
        updateSpeed = (pSpeed != soundSpeed); // only update if diff
        
    }
    
    // set volume and speed on current track
    
    if (soundTrack < sounds.size()){
        if (updateVolume)
            sounds[soundTrack].setVolume(soundVolume);
        if (updateSpeed)
            sounds[soundTrack].setSpeed(soundSpeed);
    }
    
}

//--------------------------------------------------------------
void ofApp::soundSwitch(){
    
    if (SOUND_ON){
        
        if (soundTrack < sounds.size()){ // make sure we have sounds
            
            sounds[soundTrack].setPaused(true); // pause current
            
            // switch track
            
            if (soundTrack == sounds.size() - 1){
                soundTrack = 0;
            } else {
                soundTrack++;
            }
            
            sounds[soundTrack].setPaused(false); // play new current track
            
            ofLogNotice("Sound switch") << "new track: " << soundTrack;
            
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::processSensorVals(){
    
    /*----------------------------------------
     // Update effects based on sensorVals
     ----------------------------------------*/
    
    
    /*---------------------------
     // BEAM BREAKS
     ---------------------------*/
    
    // BEAM A1
    if (sensorVals[0] == 65000){
        if (!beamBroken[0]){
            addDrawVid();
            beamBroken[0] = true;
            
            soundSwitch(); // switch sound track
            
            ofLogNotice("Beam break: ") << "A1 triggered new video" << endl;
        } else {
            ofLogVerbose("Beam break: ") << "A1 beam already broken" << endl;
        }
        
    } else {
        beamBroken[0] = false;
        ofLogVerbose("Beam break: ") << "A1 beam reset" << endl;
    }
    
    // BEAM B2
    if (sensorVals[1] == 65000){
        if (!beamBroken[1]){
            addDrawVid();
            beamBroken[1] = true;
            
            soundSwitch(); // switch sound track
            
            ofLogNotice("Beam break: ") << "B2 triggered new video" << endl;
        } else {
            ofLogVerbose("Beam break: ") << "B2 beam already broken" << endl;
        }
        
    } else {
        beamBroken[1] = false;
        ofLogVerbose("Beam break: ") << "B2 beam reset" << endl;
    }
    
    // BEAM C3
    if (sensorVals[2] == 65000){
        if (!beamBroken[2]){
            stopDrawVid();
            beamBroken[2] = true;
            
            soundSwitch(); // switch sound track
            
            ofLogNotice("Beam break: ") << "C3 removed oldest video" << endl;
        } else {
            ofLogVerbose("Beam break: ") << "C3 beam already broken" << endl;
        }
        
    } else {
        beamBroken[2] = false;
        ofLogVerbose("Beam break: ") << "C3 beam reset" << endl;
    }
    
    // BEAM D4
    if (sensorVals[3] == 65000){
        
        if (!beamBroken[3]){
            
            // increment vidSelect
            
            // if there are 2+ videos being drawn
            if (dVids.size() >= 2){
                if (vidSelect < dVids.size()-1){
                    vidSelect++;                // increment or...
                } else {
                    vidSelect = 0;              // loop to first item
                }
                newDrawTop(vidSelect); // update draw order
                selectDrawCnt = 200; // reset selection draw countdown
                
                soundSwitch(); // switch sound track
                
                ofLogVerbose("Selected video") << vidSelect << ", " << dVids[vidSelect].path;
            } else {
                ofLogVerbose() << "No videos in dVids[] to select";
            }
            
            beamBroken[3] = true;
            
            ofLogNotice("Beam break: ") << "D4 incremented vidSelect" << endl;
        } else {
            ofLogVerbose("Beam break: ") << "D4 beam already broken" << endl;
        }
        
    } else {
        beamBroken[3] = false;
        ofLogVerbose("Beam break: ") << "D4 beam reset" << endl;
    }
    
    // BEAM E5
    if (sensorVals[4] == 65000){
        
        if (!beamBroken[4]){
            
            // decrement vidSelect
            
            // if there are 2+ videos being drawn
            if (dVids.size() >= 2){
                if (vidSelect > 0){
                    vidSelect--;                // decrement or...
                } else {
                    vidSelect = dVids.size()-1; // loop to last item
                }
                newDrawTop(vidSelect); // update draw order
                selectDrawCnt = 200; // reset selection draw countdown
                
                soundSwitch(); // switch sound track
                
                ofLogVerbose("Selected video") << vidSelect << ", " << dVids[vidSelect].path;
            } else {
                ofLogVerbose() << "No videos in dVids[] to select";
            }
            
            beamBroken[4] = true;
            
            ofLogNotice("Beam break: ") << "E5 decremented vidSelect" << endl;
        } else {
            ofLogVerbose("Beam break: ") << "E5 beam already broken" << endl;
        }
        
    } else {
        beamBroken[4] = false;
        ofLogVerbose("Beam break: ") << "E5 beam reset" << endl;
    }
    
    
    /*---------------------------
     // CAPACITIVE SENSORS
     ---------------------------*/
    
    // only adjust effects every 4th frame
    // fxCount counts down from 3 to 0
    
    if (fxCount <= 0){
        
        
        for (int i=5; i<NUM_SENSORS; i++){
            
            if ((sensorVals[i] > 600)){ // if sensor is being touched
                
                char head = (char)i+65; // get header
                
                adjustEffects(head); // adjust effects using header
            }
        }
        
        
        // pixelation auto lower
        
        if (sensorVals[13] < 500){ // if sensor is not being touched
            
            if (dVids.size() > 0){ // if there are any videos drawn
                
                dVids[vidSelect].incPixelation(false); // decrease pixelation
            }
        }
        
        // glitch auto lower
        
        if (sensorVals[14] < 500){ // if sensor is not being touched
            
            if (dVids.size() > 0){ // if there are any videos drawn
                
                dVids[vidSelect].incGlitch(false); // decrease glitch
            }
        }
        
        fxCount = 3;
    } else {
        fxCount--;
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::adjustEffects(char _head){
    
    if (dVids.size() > 0){
        string fx = "no effect";
        
        switch (_head){
            case 'F':
                dVids[vidSelect].incScale(false);
                fx = "decrease scale";
                break;
            case 'G':
                dVids[vidSelect].incScale(true);
                fx = "increase scale";
                break;
            case 'H':
                dVids[vidSelect].incTint(false);
                fx = "decrease tint";
                break;
            case 'I':
                dVids[vidSelect].incTint(true);
                fx = "increase tint";
                break;
            case 'J':
                dVids[vidSelect].incSaturation(false);
                fx = "decrease saturation";
                break;
            case 'K':
                dVids[vidSelect].incSaturation(true);
                fx = "increase saturation";
                break;
            case 'L':
                dVids[vidSelect].incContrast(false);
                fx = "decrease contrast";
                break;
            case 'M':
                dVids[vidSelect].incContrast(true);
                fx = "increase contrast";
                break;
            case 'N':
                dVids[vidSelect].incPixelation(true);
                fx = "increase pixelation";
                break;
            case 'O':
                dVids[vidSelect].incGlitch(true);
                fx = "increase glitch";
                break;
            default:
                ofLogError("Adjust Effect") << _head << " header invalid (only 'F'-'N' is valid)";
                break;
                
        }
        
        ofLogNotice("Adjust Effect") << _head << ": " << fx;
    }
}

//--------------------------------------------------------------
void ofApp::addDrawVid(){
    
    /*----------------------------------------
     // Move oldest qVids[] to dVids[]
     // and play it
     ----------------------------------------*/
    
    if (qVids.size() > 0){
        dVids.push_back(qVids[0]); // add first qVids[] item to dVids
        dVids.back().vid.play(); // play the vid
        qVids.erase(qVids.begin()); // erase the vid from qVids[]
        
        // set vidSelect to newest video
        vidSelect = dVids.size()-1;
        selectDrawCnt = 200;
        
        // log
        ofLogVerbose("Now drawing") << dVids.back().path;
        
        // update draw order
        newDrawTop(vidSelect);
        
    } else {
        ofLogVerbose() << "Tried to draw new vid, but qVids[] empty";
    }
    
    
}

//--------------------------------------------------------------
void ofApp::stopDrawVid(){
    
    /*----------------------------------------
     // Stop the oldest dVids[]
     // and move it to qVids[]
     ----------------------------------------*/
    
    if (dVids.size() > 0){
        
        // remove vid from draw order
        eraseDrawOrder(0);
        
        dVids.front().vid.stop(); // stop the vid
        qVids.push_back(dVids[0]); // add to qVids[]
        dVids.erase(dVids.begin()); // erase from dVids[]
        
        // update video selector
        if (vidSelect > 0){
            vidSelect--;
        }
        selectDrawCnt = 200; // start draw selection countdown
        
        ofLogVerbose("Stopped drawing ") << qVids.back().path;
        
    } else {
        ofLogVerbose() << "Tried to remove a draw vid, but dVids[] empty";
    }
    
}

//--------------------------------------------------------------
void ofApp::checkForNewFile(){
    
    /*----------------------------------------
     // Check bin/data for new files
     ----------------------------------------*/
    
    bool switchedOffVerbose = false;
    if (isLogVerbose()){
        switchedOffVerbose = true;
        ofSetLogLevel(OF_LOG_NOTICE);
    }
    
    dir.reset();
    dir.listDir(""); // reload directory
    
    if (dir.size() > dirSize){ // if more files than before
        
        vector<ofFile> dirFiles = dir.getFiles(); // list the files
        
        // Find the new file(s)
        
        for (int i=0; i<dirFiles.size(); i++){ // loop through the files
            
            string fName = dirFiles[i].getFileName(); // get the filename
            // and check if the filename matches the files we know:
            
            //compare with old files
            
            bool oldFile = false;
            for (int j=0; j<files.size(); j++){ // check filename against current files
                
                if (fName == files[j].getFileName()){ // check is file is already loaded
                    oldFile = true; // if loaded, flag
                    break; // stop checking
                }
            }
            
            // if this is a new file
            
            if (!oldFile) {
                bNewVid = true; // global new file flag
                newVidPath = fName; // global new filename
                foundVidTime = ofGetElapsedTimef(); // save time when file was found
                loadVidWait = 0; // reset counter to wait to load (accounts for copying file, etc.)
                
                dirSize++; // increase dirSize value to account for new file
                
                ofLogNotice("New file found") << newVidPath << " at elapsed time: " << foundVidTime;
                
                break; // break loop - don't keep searching for new files
                // we'll wait until this file is loaded
            }
        }
    }
    
    if (switchedOffVerbose){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }
    
}

//--------------------------------------------------------------
void ofApp::loadNewFile(){
    
    /*----------------------------------------
     // Load new video into qVids[]
     ----------------------------------------*/
    
    if (loadVidWait >= LOADWAIT){ // if done waiting to load
        
        LoVid vid = LoVid(newVidPath); // construct new LoVid object
        qVids.push_back(vid); // add to noDrawVids vector
        
        // add file to files[]
        ofFile newVid(newVidPath);
        files.push_back(newVid);
        
        ofLogVerbose("Loaded video") << newVidPath << " after waiting " << loadVidWait << " seconds";
        
        
        bNewVid = false;
        
    } else {
        loadVidWait = ofGetElapsedTimef() - foundVidTime;
    }
}

//--------------------------------------------------------------
void ofApp::drawSelection(int selection){
    
    /*----------------------------------------
     // Draw box around selected video
     ----------------------------------------*/
    
    ofVec2f pos;
    ofVec2f size;
    
    if (selection < dVids.size()){ // make sure selection is in dVids[]
        
        pos = ofVec2f(dVids[selection].pos);
        size = ofVec2f(dVids[selection].size);
        
        ofPushStyle();
        
        ofSetColor(255,selectDrawCnt); // white with fadeout alpha
        float outline = ofMap(selectDrawCnt,0,200,0,10); // outline shrinks to 0
        
        //draw selection box
        ofRectRounded(pos.x-outline*0.5,pos.y-outline*0.5,size.x+outline,size.y+outline,outline);
        
        ofPopStyle();
    }
    
    // log selection draw if new
    if (selectDrawCnt >= 200){
        ofLogVerbose("Draw selection box") << selection;
    }
    
    // decrement counter
    selectDrawCnt -= 2 ;
    if (selectDrawCnt < 0){
        selectDrawCnt = 0; // end countdown at 0
    }
    
}

//--------------------------------------------------------------
void ofApp::newDrawTop(int selection){
    
    /*----------------------------------------
     // Put video on top of draw order
     ----------------------------------------*/
    
    if (selection < dVids.size()){ // make sure selection is in dVids[]
        
        // check if selection is in dVidsOrder[]
        if (ofContains(dVidsOrder, selection)){
            
            // 1: erase from dVidsOrder[]
            dVidsOrder.erase(dVidsOrder.begin() + ofFind(dVidsOrder, selection));
            
            // 2: push to back of dVidsOrder[]
            dVidsOrder.push_back(selection);
            
        } else {
            // if selection not in dVidsOrder[] already
            
            // add selection to dVidsOrder[]
            dVidsOrder.push_back(selection);
        }
        
        ofLogVerbose("Draw on top") << selection << " now drawing on top";
        
    } else {
        // selection not in dVids[] - weird
        ofLogVerbose("Draw on top") << selection << " not in dVids[]";
    }
    
    // only bother to log draw order if verbose logging
    if (isLogVerbose()){
        string order = "";
        for (int i=0; i<dVidsOrder.size(); i++){
            order += " " + ofToString(dVidsOrder[i]);
        }
        ofLogVerbose("Draw order") << order;
    }
}

//--------------------------------------------------------------
void ofApp::eraseDrawOrder(int selection){
    
    /*----------------------------------------
     // Remove a video from the draw order
     // (i.e. stop drawing it)
     ----------------------------------------*/
    
    if (selection < dVids.size()){ // make sure selection is in dVids[]
        
        // make sure selection is in dVidsOrder[]
        if (ofContains(dVidsOrder, selection)){
            
            // erase from dVidsOrder[]
            dVidsOrder.erase(dVidsOrder.begin() + ofFind(dVidsOrder, selection));
            ofLogVerbose("Erase from draw order") << selection << " removed from dVidsOrder[]";
            
            // update dVidsOrder[] to match dVids[] #s
            for (int i=0; i<dVidsOrder.size(); i++){
                dVidsOrder[i]--;
            }
            
        } else {
            // selection not in dVidsOrder[]
            ofLogVerbose("Erase from draw order") << selection << " not in dVidsOrder[]";
        }
        
    } else {
        // selection not in dVids[] - weird
        ofLogVerbose("Erase from draw order") << selection << " not in dVids[]";
    }
    
    // only bother to log draw order if verbose logging
    if (isLogVerbose()){
        string order = "";
        for (int i=0; i<dVidsOrder.size(); i++){
            order += " " + ofToString(dVidsOrder[i]);
        }
        ofLogVerbose("Draw order") << order;
    }
}

//--------------------------------------------------------------
bool ofApp::isLogVerbose(){
    string verbose = ofGetLogLevelName(OF_LOG_VERBOSE);
    string level = ofGetLogLevelName(ofGetLogLevel());
    if (level == verbose){
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
            
        //PATTERN SHADER SELECTION: QWERTYUIOP (1-10)
        case 'q':
            patternSelect = 0;
            ofLogVerbose("Patterns") << "adjusting dots";
            break;
        case 'w':
            patternSelect = 1;
            ofLogVerbose("Patterns") << "adjusting waves";
            break;
        case 'e':
            patternSelect = 2;
            ofLogVerbose("Patterns") << "adjusting diamonds";
            break;
        case 'r':
            patternSelect = 3;
            ofLogVerbose("Patterns") << "adjusting crosses1";
            break;
        case 't':
            patternSelect = 4;
            ofLogVerbose("Patterns") << "adjusting crosses2";
            break;
        case 'y':
            patternSelect = 5;
            ofLogVerbose("Patterns") << "adjusting crosses3";
            break;
        case 'u':
            patternSelect = 6;
            ofLogVerbose("Patterns") << "adjusting chevrons";
            break;
        case 'i':
            patternSelect = 7;
            ofLogVerbose("Patterns") << "adjusting trapezoids1";
            break;
        case 'o':
            patternSelect = 8;
            ofLogVerbose("Patterns") << "adjusting trapezoids2";
            break;
        case 'p':
            patternSelect = 9;
            ofLogVerbose("Patterns") << "adjusting squeezeBoxes";
            break;
        //PATTERN SHADER ADJUSTMENT
        case '[':
        case ']':
        {
            bool increase = true;
            if (key == '['){
                increase = false;
            }
            if (dVids.size() > 0){
                dVids[vidSelect].incPattern(patternSelect,increase);
                ofLogVerbose("Pattern") << "increasing: " << patternSelect;
            }
        }
            break;
            
            
            // turn on/off debug
        case 'd':
        {
            debug = !debug;
            string d;
            if (debug){
                d = "true";
                ofSetLogLevel(OF_LOG_VERBOSE);
            } else {
                d = "false";
                ofSetLogLevel(OF_LOG_NOTICE);
            }
            ofLogNotice("Debug") << d << endl;
        }
            break;
            
            // print framerate
        case 'f':
            ofLogNotice("Framerate") << ofGetFrameRate();
            break;
            
            //switch soundTrack
        case 's':
            soundSwitch();
            break;
            
        case 'v':
        {
            bVineThread = !bVineThread;
            if (bVineThread){
                vineThread.startThread(true, isLogVerbose());
            } else {
                vineThread.stop();
            }
            string pollVine = "not ";
            if (bVineThread)
                pollVine = "";
            ofLogNotice("Vine API") << pollVine << "auto-polling Vine API";
        }
            break;
            
            
            /*----------------------------------------
             // Kinect controls
             ----------------------------------------*/
            
            // use kinect centroid for video pos
        case 'c':
        {
            kinectCentroid = !kinectCentroid;
            string useCent = "not ";
            if (kinectCentroid)
                useCent = "";
            ofLogNotice("Kinect") << useCent << "using kinect centroid for video pos";
        }
            break;
            
            // draw kinect boolean
        case 'k':
        {
            drawKinect = !drawKinect;
            string draw = "not ";
            if (drawKinect)
                draw = "";
            ofLogNotice("Kinect") << draw << "drawing kinect cv image and centroids";
        }
            break;
            
            // kinect threshold controls
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            ofLogNotice("Kinect") << "far threshold is " << farThreshold;
            break;
            
        case '<':
        case ',':
            farThreshold --;
            if (farThreshold < 0) farThreshold = 0;
            ofLogNotice("Kinect") << "far threshold is " << farThreshold;
            break;
            
        case '+':
        case '=':
            nearThreshold ++;
            if (nearThreshold > 255) nearThreshold = 255;
            ofLogNotice("Kinect") << "near threshold is " << nearThreshold;
            break;
            
        case '-':
            nearThreshold --;
            if (nearThreshold < 0) nearThreshold = 0;
            ofLogNotice("Kinect") << "near threshold is " << nearThreshold;
            break;
        
            
            
            /*----------------------------------------
             // Add to/delete videos from dVids[]
             ----------------------------------------*/
            
            // add new vid to dVids[]
        case 'n':
            addDrawVid();
            break;
            
            // delete old vid from dVids[]
        case 'x':
            stopDrawVid();
            break;
            
            
            /*----------------------------------------
             // Select video for effects adjustment
             ----------------------------------------*/
            
            /*----------------------------------------
             // Select previous video
             ----------------------------------------*/
        case OF_KEY_LEFT:
        {
            // if there are two or more videos being drawn
            if (dVids.size() >= 2){
                if (vidSelect > 0){
                    vidSelect--; // decrement selector
                } else {
                    vidSelect = dVids.size()-1; // assign to last item
                }
                newDrawTop(vidSelect); // update draw order
                selectDrawCnt = 200; // reset selection draw countdown
                
                ofLogVerbose("Selected video") << vidSelect << ", " << dVids[vidSelect].path;
            } else {
                ofLogVerbose() << "No videos in dVids[] to select";
            }
        }
            break;
            
            /*----------------------------------------
             // Select next video
             ----------------------------------------*/
        case OF_KEY_RIGHT:
        {
            // if there are two or more videos being drawn
            if (dVids.size() >= 2){
                if (vidSelect < dVids.size()-1){
                    vidSelect++;
                } else {
                    vidSelect = 0; // assign to first item
                }
                newDrawTop(vidSelect); // update draw order
                selectDrawCnt = 200; // reset selection draw countdown
                
                ofLogVerbose("Selected video") << vidSelect << ", " << dVids[vidSelect].path;
            } else {
                ofLogVerbose() << "No videos in dVids[] to select";
            }
        }
            break;
            
            /*----------------------------------------
             // Video effects
             ----------------------------------------*/
        default:
        {
            /*----------------------------------------
             // Adjust selected video effect
             ----------------------------------------*/
            if ((key == OF_KEY_DOWN) || (key == OF_KEY_UP)){
                bool increase;
                if (key == OF_KEY_DOWN){
                    increase = false;
                } else if (key == OF_KEY_UP){
                    increase = true;
                }
                if (dVids.size() > 0){
                    switch (fxSelect){
                        case 1:
                            dVids[vidSelect].incScale(increase);
                            break;
                        case 2:
                            dVids[vidSelect].incTint(increase);
                            break;
                        case 3:
                            dVids[vidSelect].incSaturation(increase);
                            break;
                        case 4:
                            dVids[vidSelect].incContrast(increase);
                            break;
                        case 5:
                            dVids[vidSelect].incPixelation(increase);
                            break;
                        case 6:
                            dVids[vidSelect].incGlitch(increase);
                            break;
                    }
                    ofLogVerbose("Framerate") << ofGetFrameRate();
                }
            }
            else {
                
                /*----------------------------------------
                 // Select video effect by number
                 ----------------------------------------*/
                int nKey = key - '0'; // get key code as int (remove ascii offset with -'0')
                
                // if key is 1 to 6
                if (nKey >= 1 && nKey <= 6){
                    fxSelect = nKey;
                    string effect = "";
                    switch (fxSelect){
                        case 1:
                            effect = "size";
                            break;
                        case 2:
                            effect = "tint";
                            break;
                        case 3:
                            effect = "saturation";
                            break;
                        case 4:
                            effect = "contrast";
                            break;
                        case 5:
                            effect = "pixelation";
                            break;
                        case 6:
                            effect = "glitch";
                            break;
                    }
                    ofLogVerbose("Selected fx") << fxSelect << ", " << effect;
                }
            }
        }
            break; //end default
            
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
    
    /*----------------------------------------
     // Move selected video
     ----------------------------------------*/
    
    if (dVids.size() > 0){
        ofVec2f cPos;
        cPos.x = x;
        cPos.y = y;
        
        dVids[vidSelect].setCenterPos(cPos);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    selectDrawCnt = 200; // show selection box
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

