#include <stdlib.h>
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate( 25 );
    
    isRecording = false;
    flip = false;

    ofEnableAntiAliasing();
    ofEnableDepthTest();
    ofSetVerticalSync(true);
    
    cam.setAutoDistance(true);
    
//    string result2 = ofSystemTextBoxDialog("hallo");
    
    
    string exec0 = "echo hallo | su Admin";
    string exec1 = "echo hallo | sudo -S kextunload -bundle com.apple.driver.AppleUSBFTDI; ";
    string exec2 = "echo hallo | sudo -S kextunload /System/Library/Extensions/FTDIUSBSerialDriver.kext";

    
    string resultExec;
    resultExec = ofSystem( exec1 + exec2);
    ofLog() << "\n\n\n RESULT 1 : " << resultExec;
//    resultExec = ofSystem( exec2 );
//    ofLog() << "\n\n\n RESULT 2 : " << resultExec;
    ofLog() << " \n\n\n";
    
//     FTDIUSBSerialDriver.kext
    
//    ofLog() <<" RESULT 1 : " << result1;
//    ofLog() <<" RESULT 2 : " << result2;
    
//    FtdiDevice::USB_PRODUCT_ID = 0x6011;
//    FtdiDevice::USB_INTERFACE = INTERFACE_B;
//    ofLog() << "FtdiDevice::USB_PRODUCT_ID :    "  << FtdiDevice::USB_PRODUCT_ID;
//    ofLog() << "FtdiDevice::USB_INTERFACE :     "  << FtdiDevice::USB_INTERFACE;

    for( int i=0;i<18;i++ ){
        translationTable[i] = i;
    }
    translationTable[0] = 2;
    translationTable[1] = 3;
    translationTable[2] = 4;
    translationTable[3] = 5;
    translationTable[4] = 6;
    translationTable[5] = 7;
    translationTable[6] = 8;
    translationTable[7] = 9;
    
    translationTable[8] = 0;
    translationTable[9] = 1;
    translationTable[10] = 2;
    translationTable[11] = 3;
    translationTable[12] = 4;
    translationTable[13] = 5;
    translationTable[14] = 6;
    translationTable[15] = 8;
    translationTable[16] = 7;
    translationTable[17] = 9;
    
    currentColumn = 0;
    
    bufferReplay = ofBufferFromFile("out_1415386004.dat"); // <---- GOOD!
//    bufferReplay = ofBufferFromFile("out_1415367983.dat");
    
    
//    udpServer.Create();
//    udpServer.SetNonBlocking( true );
//    udpServer.Bind( 6454 );

    printf("ofxGenericDmx addon version: %s.%s\n", ofxGenericDmx::VERSION_MAJOR, ofxGenericDmx::VERSION_MINOR);

    
    bool isOpen;
    
    //zero our DMX value array
    memset( dmxData1, 0, DMX_DATA_LENGTH );
    memset( dmxData2, 0, DMX_DATA_LENGTH );
    memset( udpPackage, 0, UDP_DATA_SIZE );
    
    
    FtdiDevice::USB_PRODUCT_ID = 0x6011;    // Mike new
    FtdiDevice::USB_PRODUCT_ID = 0xd8b4;    // Mike old
    FtdiDevice::USB_INTERFACE = INTERFACE_A;
//    FtdiDevice::getDeviceList();
//    FtdiDevice::freeDeviceList();
//    isOpen = device1.open( 0, "FTWDT9JH");
    device1 = ofxGenericDmx::createDevice(DmxDevice::DMX_DEVICE_RAW);
    isOpen = device1->open();
    ofLog() << " device1 isOpen : " << isOpen;
    
        
    FtdiDevice::USB_PRODUCT_ID = 0x6001;
    FtdiDevice::USB_INTERFACE = INTERFACE_A;
    FtdiDevice::getDeviceList( );
    FtdiDevice::freeDeviceList();
    device2 = ofxGenericDmx::createDevice(DmxDevice::DMX_DEVICE_RAW);
    isOpen = device2->open();
    ofLog() << " device2 isOpen : " << isOpen;
    
    values[10] = 100010;
//    //zero our DMX value array
//    memset( dmxData_, 0, DMX_DATA_LENGTH );
//    //open the device
//    dmxInterface_ = ofxGenericDmx::createDevice(DmxDevice::DMX_DEVICE_RAW);
//    bool opened = dmxInterface_->open();
//    if ( dmxInterface_ == 0 || !opened ) printf( "No FTDI Device Found\n" );
//    else printf( "isOpen: %i\n", dmxInterface_->isOpen() );
    
}

//--------------------------------------------------------------
void testApp::update(){
    
//    return;
    
//    int l = DMX_DATA_LENGTH*2;

//    flip = !flip;
    
//    for( int i=0;i<l;i++ ){
//        udpPackage[i] = (i+ofGetFrameNum())%255;
//    }

    if( false ){
        char incomingData[ UDP_DATA_SIZE ];
        memset( incomingData, 0, UDP_DATA_SIZE );
        int res = udpServer.Receive( incomingData , UDP_DATA_SIZE );
        if( res == UDP_DATA_SIZE ){
            if( isRecording ){
                frameRecording ++;
                buffer.append( incomingData, UDP_DATA_SIZE);
                if( res > 0 ) ofLog() << " ReS : " << res << "      "  << ofGetUnixTime() << " frameRecording : " << frameRecording;
//                buffer.append("\n");
            }
            for( int i=0;i<UDP_DATA_SIZE;i++ ){
                udpPackage[i] = incomingData[i];
            }
        }
        
    }else{

        char* buf = bufferReplay.getBinaryBuffer();
        string str;
        int frame = ofGetFrameNum()% (bufferReplay.size() / UDP_DATA_SIZE);
        for( int i=0;i<UDP_DATA_SIZE;i++){
            udpPackage[i] = buf[i+frame*UDP_DATA_SIZE];
        }

    }
    
    
    setDmxData();

    
    
        if( device1->isOpen() ){
            int res1 = device1->writeDmx( dmxData1, DMX_DATA_LENGTH );
        }
        if( device2->isOpen() ){
            int res2 = device2->writeDmx( dmxData2, DMX_DATA_LENGTH );
        }

    
//    cam.setPosition(100*1.5, 500, 800);
    
}

void testApp::setDmxData(){
    memset( dmxData1, 0, DMX_DATA_LENGTH );
    memset( dmxData2, 0, DMX_DATA_LENGTH );

    
    int ledsPerColumns = 57;
    int columnId, columnIdTranslated, posInColumn, translatedPos;
    for( int i=0;i<UDP_DATA_SIZE;i++ ){
        //        int val = (sin( (float)(ofGetFrameNum() + (i))/10.0 ) + 1.0 ) / 2.0 * 150;
        
        posInColumn = i%ledsPerColumns;
        columnId = i/ledsPerColumns;
        columnIdTranslated = translationTable[ columnId ];
        translatedPos = columnIdTranslated * ledsPerColumns + posInColumn + 1;
        
        int val = udpPackage[ i ];
//        val = 1;
//        if( (int)(i / 57) == currentColumn ){
//            val = posInColumn*4;
//        }
//        if(posInColumn == 0 || posInColumn == ledsPerColumns-1) val = 100;
        
        if(columnId < 8){
            dmxData1[translatedPos] = val;
        }else{
            dmxData2[translatedPos] = val;
        }
    }
    
    //force first byte to zero (it is not a channel but DMX type info - start code)
    dmxData1[0] = 0;
    dmxData2[0] = 0;

}







//--------------------------------------------------------------
void testApp::draw(){
    
//    return;
    
	//use the color for the app background
//	ofBackground( 20, 40, 20);
    ofBackgroundGradient(ofColor(65,62,50),ofColor(25,22,10) );
    ofSetColor(255, 255, 255);
    
//    cam.setPosition(-60, 500, 800);
//    cam.lookAt( ofVec3f(-100*1.5, 100, 0) );
    
//    cam.begin();
    drawColumns();
//    cam.end();
    
    if( isRecording ){
        ofSetColor(200, 0, 0);
        ofCircle(20, 20, 10);
    }

	ofDrawBitmapString("[SPACE] toggle recording.", 20, 40);
    ofDrawBitmapString("[s] save recording.", 20, 60);

}

void testApp::drawColumns(){
    float px,pz;
    bool even;
    for( int i=0;i<18;i++){
        even = i%7>3;
        pz = (int)(i/7);
        px = (i%7)%4;
        if(even) pz += .5;
        if(even) px += .5;
//        pz = 3 - pz;
        pz *= -1;
        px *= -1;
        
        ofPushMatrix();
        ofTranslate( px*100+250, 0, pz*100);
        drawColumn(i);
        ofPopMatrix();
    }
}

void testApp::drawColumn( int id ){
    int h = 4;
    int ledsPerColumn = 57;
    int grey;
    for( int i=0;i<57;i++ ){
        grey = (unsigned char) udpPackage[ id *ledsPerColumn + i];
        ofSetColor( grey, grey, grey );
        ofDrawBox(0, i*(h+3), 0, 9, h, 10);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    bool isOpen = false;
	switch (key) {
            
        case ',':
            currentColumn = (currentColumn+18-1)%18;
            break;
        case '.':
            currentColumn = (currentColumn+18+1)%18;
            ofLog() << "currentColumn : " << currentColumn;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
//        case '1':
//            if( device1->isOpen() ){
//                device1->close();
//            }else{
//                FtdiDevice::freeDeviceList();
//                isOpen = device1.open( 0, "FTWDT9JH");
//                ofLog() << " device1 isOpen : " << isOpen;
//            }
//            break;
//        case '2':
//            if( device2.isOpen() ){
//                device2.close();
//            }else{
//                FtdiDevice::freeDeviceList();
//                isOpen = device2.open( 0, "FTWDN2EF");
//                ofLog() << " device2 isOpen : " << isOpen;
//            }
//            break;
        case 'l':
            flip = !flip;
//            isRecording = !isRecording;
            break;

        case ' ':
            frameRecording = 0;
            isRecording = !isRecording;
            if(isRecording){
                buffer.clear();
            }
            break;
        case 's':
            string file = "out_"+ofToString( ofGetUnixTime() )+".dat";
            ofBufferToFile(file, buffer);
            ofLog() << " SAVED : "  <<  file ;
            break;
//        case '1':
//            bool isOpen1 = device1.open( 0, "FTWDT9JH");
//            ofLog() << " device1 isOpen : " << isOpen1;
//            break;
//        case '2':
//            bool isOpen2 = device2.open( 0, "FTWDN2EF");
//            ofLog() << " device2 isOpen : " << isOpen2;
//            break;
//        case '1':
//            bool isOpen1 = device1.open( 0, "FTWDT9JH");
//            ofLog() << " device1 isOpen : " << isOpen1;
//            break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::exit(){

    try {
        if( device1->isOpen() ){
            //        for ( int i = 0; i <= DMX_DATA_LENGTH; i++ ) dmxData1[i] = 0;
            //        device1.writeDmx( dmxData1, DMX_DATA_LENGTH );
            device1->close();
        }
        
        if( device2->isOpen() ){
            //        for ( int i = 0; i <= DMX_DATA_LENGTH; i++ ) dmxData2[i] = 0;
            //        device2.writeDmx( dmxData2, DMX_DATA_LENGTH );
            device2->close();
        }
    } catch (exception x) {
        ofLog() << "ON EXIT ERROR!";
    }
    
    udpServer.Close();
    
    
}
