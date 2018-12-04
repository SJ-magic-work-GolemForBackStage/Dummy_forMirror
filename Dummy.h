/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxOsc_BiDirection.h"
#include "ofxNetwork.h"

#include "sj_common.h"

/************************************************************
************************************************************/
class GOLEM_DUMMY : private Noncopyable{
private:
	/****************************************
	****************************************/
	enum MESSAGE_IN{
		GET_STATUS,
		START_CALIB,
		START_TRACKING,
		STOP_TRACKING,
		CLOSE_GOLEM,
		GET_SKELTON_DEFINITION,
		
		NUM_MESSAGE_TYPES,
	};
	
	enum STATE{
		STATE_SLEEP,
		STATE_ACT,
	};
	
	/****************************************
	****************************************/
	STATE State;
	float d_BlankTime;
	float t_EnterSleep;
	
	FILE* fp_Skelton_FrameData;
	FILE* fp_Skelton_Definition;
	
	int Next_FrameId_of_MotionData;
	
	bool b_1st;
	OSC_TARGET Osc;
	
	ofxUDPManager udpConnection;
	char filaName_FrameData[BUF_SIZE_S];
	char fileName_SkeletonDefs[BUF_SIZE_S];
	
	float t_LastSend_Udp;
	
	
	/****************************************
	****************************************/
	void SendUdp_SkeltonDefinition();
	void Align_StringOfData(string& s);
	bool SendUdp_FrameData(float now);
	bool checkIf_ContentsExist(char* ret, char* buf);
	void Dice_BlankTime();
	
public:
	/****************************************
	****************************************/
	GOLEM_DUMMY(const char* Osc_IP, int Osc_Port_To, int Osc_Port_From, const char* _UdpIP_To, int _Udp_Port_To, const char* _filaName_FrameData, const char* _fileName_SkeletonDefs);
	~GOLEM_DUMMY();
	
	void update(float now);
	
	int get_NextFrameId();
	float get_TimeToAct(float now);
};

