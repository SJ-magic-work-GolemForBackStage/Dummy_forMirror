#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	if(argc < 2){
		printf("exe [Num Dummy Golems]\n");
		return 1;
	}else{
		int NumDummyGolems = atoi(argv[1]);
		if(NumDummyGolems <= 0){
			printf("Invalid Numbers\n");
			return 1;
		}
		
		ofRunApp(new ofApp(NumDummyGolems));
	}
}
