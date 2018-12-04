/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "Dummy.h"

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		MAX_DUMMY_GOLEMS = 10,
	};
	
	float now;
	
	ofTrueTypeFont font;
	
	const int Num_DummyGolems;
	GOLEM_DUMMY* GolemDummy[MAX_DUMMY_GOLEMS];
	
	int id_Disp_FrameNumber;
	
public:
	/****************************************
	****************************************/
	ofApp(int _Num_DummyGolems);
	~ofApp();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
