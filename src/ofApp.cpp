#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    numRows = 9;
    numColumns = 17;
    dispState = 2;
    
    // display vars mockup
    marginX = 0;
    marginY = 0;
    dispWidth = 100;
    dispHeight = 100;
    vidOriginX = 10;
    vidOriginY = 10;
    
    // lock time variables
    checkInDuration = 45000;
    pickUpDuration = 45000;
    
    string tempIPs[] = {"192.168.0.121", "192.168.0.156", "192.168.0.107", "192.168.0.104", "192.168.0.180"};
    
    // temp initialize lockers
    for(int col_i = 0 ; col_i < numColumns ; ++col_i){
        vector<Locker* > tempLockerCol;
        for(int row_i = 0; row_i < numRows; ++row_i){
            Locker* tempLocker = new Locker(col_i, row_i, "192.168.0.126");
            tempLockerCol.push_back(tempLocker);
            
            if(col_i == 0 && row_i < 5){
                tempLocker->setIP(tempIPs[row_i]);
            }
            else if(col_i == 0 && row_i == 0){
                tempLocker->setIP("192.168.0.178");
            }
            else {
                tempLocker->setIP("192.168.0.144");
            }
        }
        
        lockers.push_back(tempLockerCol);
    }
    

    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            ofLog() << lockers[i][j]->ipAddress << endl;
        }
    }
    
    sourceVid.loadMovie("movies/fingers.mov");
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
    
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // process through received OSC messages
    processMessages();
    
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
            concentrics(9, 3);
            break;
        case 4:
            colorStripes();
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
                dispState = 4;
            }
            break;
        case OF_KEY_RIGHT:
            dispState = (dispState + 1) % 5;
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

// algorithmic displays

void ofApp::colorSweep() {
    
    long long currentTime = ofGetSystemTime();
    
    int switcher = (currentTime % 2550) / 10;
    
    // does a color wheel
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            lockers[i][j]->setColor(wheel((j*lockers.size()+i+switcher)%255));
        }
    }
}

void ofApp::concentrics(int _x, int _y) {
    
    long long currentTime = ofGetSystemTime();
    
    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    
    int switcher = (currentTime % 10000) / 200;
    
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            lockers[i][j]->setColor(colors[(abs(i - _x) + abs(j - _y) + 50 - switcher) % 4]);
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
    
    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    int switcher = (currentTime % 10000) / 200;
    
    // horizontal sweep
    //    for(int i = 0; i < lockers.size(); ++i) {
    //        for(int j = 0; j < lockers[i].size(); ++j){
    //            lockers[i][j]->setColor(colors[(switcher+i)%4]);
    //        }
    //    }
    
    // vertical sweep
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            lockers[i][j]->setColor(colors[(lockers[i].size() - j + switcher)%4]);
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

void ofApp::displayOpen() {

    long long currentTime = ofGetSystemTime();

    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            if(lockers[i][j]->currentState == 1){
                
                ofLog() << "currentTime " << ofToString(currentTime) << ", lockmillis " << ofToString(lockers[i][j]->lockMillis) << " and the difference is " << ofToString(currentTime - lockers[i][j]->lockMillis) << endl;
                if(currentTime - lockers[i][j]->lockMillis > checkInDuration){
                    
                    ofLog() << "locker " << ofToString(i) << ", " << ofToString(j) << " is now closing" << endl;
                    lockers[i][j]->currentState = 0;
                }
                else{
                    ofColor tempColor(255,0,0);
                    ofLog() << "locker " << ofToString(i) << ", " << ofToString(j) << " is open" << endl;
                    lockers[i][j]->setColor(tempColor);
                    
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
        
        ofLog() << "message with address: " << m.getAddress() << " " << m.getArgAsInt32(0) << " " << m.getArgAsInt32(1) << " " << m.getArgAsInt32(2) << endl;
        
//        int row = m.getArgAsInt32(0);
//        int col = m.getArgAsInt32(1);
//        int state = m.getArgAsInt32(2);
        
        if(state == 1){
         
            lockers[col][row]->lockMillis = ofGetSystemTime();
            lockers[col][row]->currentState = 1;
            ofPoint pointer = ofPoint(row,col);
//            unlocked.push_back(pointer);

            
//            for(int i=0; i < unlocked.size(); i++){
//                ofPoint temp = unlocked[i];
//                ofLog() << ofToString(temp.x) << " and " << ofToString(temp.y) << endl;
//            }
        }
        
        else if (state == 1){
            
            
        }
        
        m.getArgAsInt32(0);
        m.getArgAsInt32(1);
        ofBackground(m.getArgAsInt32(0), m.getArgAsInt32(1), m.getArgAsInt32(2));
        
    }
}

void ofApp::sendMessages() {
    
    // send OSC Messages
    for(int i = 0; i < numColumns; ++i){
        for(int j = 0; j < numRows; ++j){
            // temp
            
            // send color state
            ofxOscMessage m;
            ofColor tempColor = lockers[i][j]->getColor();
//            ofLog() << "row " << ofToString(i) << " col " << ofToString(j) << endl;
            sender.setup(lockers[i][j]->ipAddress, PORT);
            m.setAddress("/led");
            m.addIntArg(84);
            m.addIntArg(tempColor.r);
            m.addIntArg(tempColor.g);
            m.addIntArg(tempColor.b);
            sender.sendMessage(m);
            
            // final
            /*
            ofxOscMessage m;
            ofColor tempColor = lockers[i][j]->getColor();
            //            ofLog() << "row " << ofToString(i) << " col " << ofToString(j) << endl;
            sender.setup(lockers[i][j]->ipAddress, PORT);
            m.setAddress("/" + ofToString(j));
            m.addIntArg(tempColor.r);
            m.addIntArg(tempColor.g);
            m.addIntArg(tempColor.b);
            sender.sendMessage(m);
             
             */
        }
    }
}
//
//for(int i = 0; i < numColumns; ++i){
//    for(int j = 0; j < numRows; ++j){
//        lockers[i][j];
//    }
//}
