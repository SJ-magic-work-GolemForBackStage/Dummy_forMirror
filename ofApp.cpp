/************************************************************
************************************************************/
#include "ofApp.h"


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _Num_DummyGolems)
: Num_DummyGolems((MAX_DUMMY_GOLEMS < _Num_DummyGolems) ? MAX_DUMMY_GOLEMS : _Num_DummyGolems)
, id_Disp_FrameNumber(0)
{
	srand((unsigned) time(NULL));

	printf("Num_DummyGolems = %d\n", Num_DummyGolems);
	fflush(stdout);
}

/******************************
******************************/
ofApp::~ofApp()
{
	for(int i = 0; i < Num_DummyGolems; i++){
		delete GolemDummy[i];
	}
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("GolemDummy : Mirror");
	ofSetVerticalSync(false);
	ofSetFrameRate(50);
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	font.load("font/RictyDiminished-Regular.ttf", 13, true, true, true);
	
	/********************
	********************/
	const int PORT_OFFSET = 12363;
	for(int i = 0; i < Num_DummyGolems; i++){
		char fileName_FrameData[BUF_SIZE_S];
		char fileName_SkeletonDefs[BUF_SIZE_S];
		
		sprintf(fileName_FrameData, "../../../data/Golem_%d/sample_motion_data.csv", i);
		sprintf(fileName_SkeletonDefs, "../../../data/Golem_%d/sample_skeleton_definition.csv", i);
		
		GolemDummy[i] = new GOLEM_DUMMY("127.0.0.1", PORT_OFFSET + i * 3 + 1, PORT_OFFSET + i * 3 + 2, "127.0.0.1", PORT_OFFSET + i * 3 + 0, fileName_FrameData, fileName_SkeletonDefs);
	}
}

/******************************
******************************/
void ofApp::update(){
	now = ofGetElapsedTimef();
	
	for(int i = 0; i < Num_DummyGolems; i++){
		GolemDummy[i]->update(now);
	}
}

/******************************
******************************/
void ofApp::draw(){

	/********************
	********************/
	ofBackground(30);
	ofSetColor(255);
	
	/********************
	********************/
	float FontHeight = font.stringHeight("0123456789") * 1.5;
	
	char buf[BUF_SIZE_S];
	sprintf(buf, "%6.2f", ofGetFrameRate());
	font.drawString(buf, 130, 30);
	
	const float Base_y = 30 - id_Disp_FrameNumber * FontHeight;
	for(int i = 0; i < Num_DummyGolems; i++){
		if(0 < GolemDummy[i]->get_NextFrameId()){
			ofSetColor(255);
			sprintf(buf, "[%03d] %05d", i, GolemDummy[i]->get_NextFrameId() - 1);
		}else{
			ofSetColor(0, 100, 255, 255);
			sprintf(buf, "[%03d] %5.0f", i, GolemDummy[i]->get_TimeToAct(now));
		}
		
		font.drawString(buf, 10, Base_y + i * FontHeight);
	}
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case OF_KEY_RIGHT:
			break;
			
		case OF_KEY_LEFT:
			break;
			
		case OF_KEY_UP:
			id_Disp_FrameNumber++;
			if(Num_DummyGolems <= id_Disp_FrameNumber) id_Disp_FrameNumber = Num_DummyGolems - 1;
			break;
			
		case OF_KEY_DOWN:
			id_Disp_FrameNumber--;
			if(id_Disp_FrameNumber < 0) id_Disp_FrameNumber = 0;
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

}
