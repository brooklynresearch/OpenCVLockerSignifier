#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    numRows = 9;
    numColumns = 17;
    dispState = 2;
    numStates = 7;
    
    // Chase colors
    CGreen = ofColor(70, 255, 32);
    COrange = ofColor(255, 69, 10);
    CYellow = ofColor(255, 218, 0);
    
    // display vars mockup
    marginX = 0;
    marginY = 0;
    dispWidth = 90;
    dispHeight = 90;
    vidOriginX = 10;
    vidOriginY = 10;
    manualSwitch = true;
    
    // lock time variables
    checkInDuration = 45000;
    pickUpDuration = 45000;
    
    
    // initialize lockers
    for(int col_i = 0 ; col_i < numColumns ; ++col_i){
        vector<Locker* > tempLockerCol;
        for(int row_i = 0; row_i < numRows; ++row_i){
            Locker* tempLocker = new Locker(col_i, row_i, piIPAddresses[col_i]);
            tempLockerCol.push_back(tempLocker);
        }
        
        lockers.push_back(tempLockerCol);
    }
    

    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            ofLog() << lockers[i][j]->ipAddress << endl;
        }
    }
    
    sourceVid.loadMovie("movies/CHASE_OPEN_17x9_ball1_v5.mov");
    sourceVid.play();
//    vidOriginX = sourceVid.getWidth();
    
    // camera feed
    sourceCam.setDeviceID(0);
    sourceCam.setDesiredFrameRate(30);
    sourceCam.initGrabber(160, 120);
//    vidOriginX = sourceCam.getWidth();
    
    ofLog() << ofGetSystemTime() << endl;
    
    // initialize sender
    sender.setup(HOST, 54321);
    
    // initalize receiver
    receiver.setup(LISTEN_PORT);
    
    // initialize timer
    stateChangeMillis = ofGetSystemTime();
    playlistIndex = 0;
    playlistLength = 8;
    frameRate = 30;
    
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    ofSetFrameRate(frameRate);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // process through received OSC messages
    processMessages();
    
    long long currentTime = ofGetSystemTime();
    // in milliseconds
    
    if(manualSwitch){
        
    }
    else {
        long long currentTime = ofGetSystemTime();
        if( currentTime - stateChangeMillis  > playlistDuration[playlistIndex] ){
            ++playlistIndex;
            playlistIndex = playlistIndex % playlistLength;
            dispState = playlistState[playlistIndex];
            stateChangeMillis = currentTime;
        }
    }
    
    switch(dispState){
        case 0:
            updateCam();
            break;
        case 1:
            updateVid();
            break;
        case 2:
            colorSweep();
            break;
        case 3:
            concentrics(8, 4);
            break;
        case 4:
            colorStripes();
            break;
        case 5:
            randomSet();
            break;
        case 6:
            randGradientColumn();
            break;
    }
    
    // Possibly override of pixel data functions
    displayOpen();
    
    // Send OSC color data
    sendMessages();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
//    sourceVid.draw(0, 0, sourceVid.getWidth(), sourceVid.getHeight());
    
    switch(dispState){
        case 0:
            sourceCam.draw(0, 0);
            break;
        case 1:
            sourceVid.draw(0, 0);
            break;
    }
    
    ofImage tempImg;
    tempImg.setFromPixels(sourceVid.getPixelsRef());
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            ofSetColor(lockers[i][j]->getColor());
            ofRect(i*dispWidth+marginX+vidOriginX, j*dispHeight+marginY+vidOriginY, dispWidth, dispHeight);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_LEFT:
            dispState = dispState - 1;
            if (dispState < 0){
                dispState = numStates - 1;
            }
            break;
        case OF_KEY_RIGHT:
            dispState = (dispState + 1) % numStates;
            break;
            
        case 'm':
        case 'M':
            manualSwitch = !manualSwitch;
            break;
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
    ofLog() << dragInfo.files[0] << endl;
    string fileExt = dragInfo.files[0].substr(dragInfo.files[0].find_last_of(".") + 1);
    
    
    if( fileExt == "mov" || fileExt == "MOV" || fileExt == "mpeg" || fileExt == "mpg" || fileExt == "mp4")
    {
        sourceVid.loadMovie(dragInfo.files[0]);
    }
    else {
        ofLog() << "not a usable file" << endl;
    }
}

// Camera functions

void ofApp::setupCam() {
    
}

void ofApp::updateCam() {

    sourceCam.update();
    ofImage tempImg;
    
    tempImg.setFromPixels(sourceCam.getPixelsRef());
    pixellate(tempImg);
}

void ofApp::drawCam() {
    
    
}


// Video functions

void ofApp::setupVid() {
    
}

void ofApp::updateVid() {
    
    sourceVid.update();
    ofImage tempImg;
    
    tempImg.setFromPixels(sourceVid.getPixelsRef());
    pixellate(tempImg);
}

void ofApp::drawVid() {
    
}

void ofApp::pixellate(ofImage tempImg){
    
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            ofColor tempColor = tempImg.getColor(i*10, j*10);
            
            ofImage croppedImg;
            
            croppedImg.cropFrom(tempImg, (tempImg.width / lockers.size() ) * i, (tempImg.height / lockers[i].size() ) * j, tempImg.width / lockers.size(), tempImg.height / lockers[i].size());
            ofxCvColorImage colorImg;
            colorImg.setFromPixels(croppedImg.getPixelsRef());
            
            IplImage* iplImg = colorImg.getCvImage();
            CvScalar mean; CvScalar stdv;
            cvAvgSdv(iplImg, &mean, &stdv);
            
            tempColor.r = (int) mean.val[0];
            tempColor.g = (int) mean.val[1];
            tempColor.b = (int) mean.val[2];
            
            lockers[i][j]->setColor(tempColor);
        }
    }
    
}

// algorithmic shows

void ofApp::colorSweep() {
    
    long long currentTime = ofGetSystemTime();
    
    int switcher = (currentTime/2 % 2550) / 10;
    
    // does a color wheel
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            lockers[i][j]->setColor(gbWheel((j*lockers.size()+i+switcher)%255));
//            lockers[i][j]->setColor(wheel((j*lockers.size()+i+switcher)%255));
//            lockers[i][j]->setColor(wheel((switcher)%255));
//            lockers[i][j]->setColor(lockers[0][0]->getColor());
        }
    }
}

void ofApp::concentrics(int _x, int _y) {
    
    long long currentTime = ofGetSystemTime();
    
    int gradients = 24;
    ofColor colors[24] = {ofColor(1, 121, 192) , ofColor(31, 138, 201) , ofColor(62, 155, 210) , ofColor(93, 172, 219) , ofColor(123, 189, 228) , ofColor(152, 206, 237) , ofColor(185, 224, 247) , ofColor(152, 196, 247) , ofColor(123, 168, 248) , ofColor(92, 140, 248) , ofColor(61, 112, 249) , ofColor(30, 84, 249) , ofColor(0, 57, 250) , ofColor(0, 58, 224) , ofColor(0, 59, 198) , ofColor(0, 61, 172) , ofColor(0, 62, 146) , ofColor(0, 62, 120) , ofColor(0, 65, 94) , ofColor(0, 74, 110) , ofColor(0, 83, 126) , ofColor(0, 93, 143) , ofColor(0, 102, 159) , ofColor(0, 111, 175)};
    
//    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    int duration = 120;
    int switcher = (currentTime % (duration*gradients)) / duration;
    
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            lockers[i][j]->setColor(colors[(abs(i - _x) + abs(j - _y) + gradients - switcher) % gradients]);
//            if( ((abs(i - _x) +abs(j - _y) + switcher) % 2 == 0)){
//                lockers[i][j]->setColor(ofColor(255,255,255));
//            }
//            else{
//                lockers[i][j]->setColor(ofColor(0,0,0));
//            }
            
        }
    }
}

void ofApp::mosaic() {
    
}

void ofApp::colorStripes() {
    
    long long currentTime = ofGetSystemTime();
    
//    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    ofColor colors[24] = {ofColor(1, 121, 192) , ofColor(31, 138, 201) , ofColor(62, 155, 210) , ofColor(93, 172, 219) , ofColor(123, 189, 228) , ofColor(152, 206, 237) , ofColor(185, 224, 247) , ofColor(152, 196, 247) , ofColor(123, 168, 248) , ofColor(92, 140, 248) , ofColor(61, 112, 249) , ofColor(30, 84, 249) , ofColor(0, 57, 250) , ofColor(0, 58, 224) , ofColor(0, 59, 198) , ofColor(0, 61, 172) , ofColor(0, 62, 146) , ofColor(0, 62, 120) , ofColor(0, 65, 94) , ofColor(0, 74, 110) , ofColor(0, 83, 126) , ofColor(0, 93, 143) , ofColor(0, 102, 159) , ofColor(0, 111, 175)};
    
    int switcher = (currentTime % 12000) / 100;
    
    // horizontal sweep
    //    for(int i = 0; i < lockers.size(); ++i) {
    //        for(int j = 0; j < lockers[i].size(); ++j){
    //            lockers[i][j]->setColor(colors[(switcher+i)%4]);
    //        }
    //    }
    
    // vertical sweep
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            lockers[i][j]->setColor(colors[(lockers[i].size() - j + switcher)%24]);
        }
    }
}

void ofApp::randomSet(){
    long long currentTime = ofGetSystemTime();
    
    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    int switcher = (currentTime % (frameRate * 30));
    
    ofLog() << "switcher is: " << ofToString(switcher) << endl;
    
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            if(switcher < frameRate){
                lockers[i][j]->setColor(colors[(rand())%4]);
            }
            
        }
    }
}

void ofApp::randGradientColumn() {
    long long currentTime = ofGetSystemTime();
    
    ofColor colors[24] = {ofColor(1, 121, 192) , ofColor(31, 138, 201) , ofColor(62, 155, 210) , ofColor(93, 172, 219) , ofColor(123, 189, 228) , ofColor(152, 206, 237) , ofColor(185, 224, 247) , ofColor(152, 196, 247) , ofColor(123, 168, 248) , ofColor(92, 140, 248) , ofColor(61, 112, 249) , ofColor(30, 84, 249) , ofColor(0, 57, 250) , ofColor(0, 58, 224) , ofColor(0, 59, 198) , ofColor(0, 61, 172) , ofColor(0, 62, 146) , ofColor(0, 62, 120) , ofColor(0, 65, 94) , ofColor(0, 74, 110) , ofColor(0, 83, 126) , ofColor(0, 93, 143) , ofColor(0, 102, 159) , ofColor(0, 111, 175)};
    
    int switcher = (currentTime % 500);
    
    if(switcher < 25){
        for(int i = 0; i < lockers.size(); ++i) {
            ofColor randSelect = colors[rand() % 24];
            for(int j = 0; j < lockers[i].size(); ++j){
                lockers[i][j]->setColor(randSelect);
            }
        }
    }
}

ofColor ofApp::wheel(int WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return ofColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return ofColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return ofColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

ofColor ofApp::gbWheel(int WheelPos){
    if(WheelPos > 127){
        WheelPos = 255 - WheelPos;
        return ofColor(0, WheelPos*2, 255 - WheelPos*2);
    }
    return ofColor(0, WheelPos*2, 255 - WheelPos*2);
    
    
}

void ofApp::displayOpen() {

    long long currentTime = ofGetSystemTime();

    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            if(lockers[i][j]->currentState == 1){
                
//                ofLog() << "currentTime " << ofToString(currentTime) << ", lockmillis " << ofToString(lockers[i][j]->lockMillis) << " and the difference is " << ofToString(currentTime - lockers[i][j]->lockMillis) << endl;
                if(currentTime - lockers[i][j]->lockMillis > checkInDuration){
                    
//                    ofLog() << "locker " << ofToString(i) << ", " << ofToString(j) << " is now closing" << endl;
                    lockers[i][j]->currentState = 0;
                    ofxOscMessage lock;
                    sender.setup(lockers[i][j]->ipAddress, PORT);
                    lock.setAddress("/lock");
                    lock.addIntArg(j);
                    lock.addIntArg(1); // lock
                    sender.sendMessage(lock);
                }
                else{
                    ofColor tempColor;
                    
                    int switcher = (currentTime % 500);
                    
                    if (switcher > 250) {

//                        tempColor = CGreen;
                        switch(lockers[i][j]->deviceID){
                            case 0:
                                tempColor = CGreen;
                                break;
                            case 1:
                                tempColor = COrange;
                                break;
                            case 2:
                                tempColor = CYellow;
                                break;
                            default:
                                break;
                        }
                    }
                    else {
                        tempColor = ofColor(185, 224, 247);
                    }
                    
//                    ofLog() << "locker " << ofToString(i) << ", " << ofToString(j) << " is open" << endl;
                    lockers[i][j]->setColor(tempColor);
                    
                    ofxOscMessage lock;
                    sender.setup(lockers[i][j]->ipAddress, PORT);
                    lock.setAddress("/lock");
                    lock.addIntArg(j);
                    lock.addIntArg(0); //unlock
                    sender.sendMessage(lock);
                }
                
            }
        }
    }
}

void ofApp::processMessages() {
    
    // OSC Receiving
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        int row = m.getArgAsInt32(0);
        int col = m.getArgAsInt32(1);
        int state = m.getArgAsInt32(2);
        int deviceID = m.getArgAsInt32(3);
        
        ofLog() << "message with address: " << m.getAddress() << " " << m.getArgAsInt32(0) << " " << m.getArgAsInt32(1) << " " << m.getArgAsInt32(2) << " " << m.getArgAsString(3) << endl;
        
//        int row = m.getArgAsInt32(0);
//        int col = m.getArgAsInt32(1);
//        int state = m.getArgAsInt32(2);
        
//        row = 1;
//        col = 0;
//        if(state == 1){
        
        lockers[col][row]->lockMillis = ofGetSystemTime();
        lockers[col][row]->currentState = 1;
        lockers[col][row]->deviceID = deviceID;
        
        // special conditions
        if(col == 0){
            if(row == 0 || row == 2 || row == 4 || row == 6){
                lockers[col+1][row]->lockMillis = ofGetSystemTime();
                lockers[col+1][row]->currentState = 1;
                lockers[col+1][row]->deviceID = deviceID;
            }
        }
        
//        }
        
    }
}

void ofApp::sendMessages() {
    
    // may possibly change this to send at the very end of going through each row of a column
    // so no more separate addresses, just a single long message
    
    // send OSC Messages
//    for(int i = 0; i < numColumns; ++i){
//        for(int j = 0; j < numRows; ++j){
//            
//            // send color state
//            ofxOscMessage m;
//            ofColor tempColor = lockers[i][j]->getColor();
////            ofLog() << "row " << ofToString(i) << " col " << ofToString(j) << endl;
//            
//            if(j < 5){
//            sender.setup(lockers[i][j]->ipAddress, PORT);
//            m.setAddress("/led");
//            m.addIntArg(j);
//            m.addIntArg(tempColor.r);
//            m.addIntArg(tempColor.g);
//            m.addIntArg(tempColor.b);
//            sender.sendMessage(m);
//            }
//        }
//    }

//     for if we switch to single messages per pi
    for(int i = 0; i < numColumns; ++i){
        ofxOscMessage m;
        m.setAddress("/led");
        sender.setup(lockers[i][0]->ipAddress, PORT);
        for(int j = 0; j < numRows; ++j){
            
            // send color state
            ofColor tempColor = lockers[i][j]->getColor();
            //            ofLog() << "row " << ofToString(i) << " col " << ofToString(j) << endl;
            
//            sender.setup(lockers[i][j]->ipAddress, PORT);
            
//            m.addIntArg(j);
            m.addIntArg(tempColor.r);
            m.addIntArg(tempColor.g);
            m.addIntArg(tempColor.b);
           
        }
        
        sender.sendMessage(m);
    }
}
