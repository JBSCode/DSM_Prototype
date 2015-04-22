#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxGenericDmx.h"
#include "DmxDevice.h"
#include "DmxRawDevice.h"
#include "ofxUDPManager.h"

//NOTE: at least on one occasion, sending all 512 channels failed to work.
//Experiments led to stable operation by sending only 494 channels.
//#define DMX_DATA_LENGTH 494
//#define DMX_DATA_LENGTH 512
#define DMX_DATA_LENGTH 600
#define UDP_DATA_SIZE 1024

// 513 values would create a maximum-sized packet (including the start code)
// This is enough data for a full frame DMX message (512 channels)
// http://en.wikipedia.org/wiki/DMX512#Protocol


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void drawColumns();
        void drawColumn( int id );
		void exit();
    
        void setDmxData();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		//pointer to our Enntec DMX USB Pro object
//        DmxDevice* dmxInterface1_;
//		DmxDevice* dmxInterface2_;
//        DmxDevice* dmxInterface_;
    
    ofEasyCam cam;


//        DmxRawDevice device1;
//        DmxRawDevice device2;
    DmxDevice* device1;
    DmxDevice* device2;
    
    int translationTable[18];
    int currentColumn;
    
    ofxUDPManager udpServer;
    
    
    ofBuffer buffer;
    ofBuffer bufferReplay;
    bool isRecording;
    int frameRecording;
    
    
    bool flip;

    //our DMX packet (which holds the channel values + 1st byte for the start code)
    unsigned char dmxData1[DMX_DATA_LENGTH];
    unsigned char dmxData2[DMX_DATA_LENGTH];
    char udpPackage[ UDP_DATA_SIZE ];
    
    map< int, int> values;

    
};

#endif
