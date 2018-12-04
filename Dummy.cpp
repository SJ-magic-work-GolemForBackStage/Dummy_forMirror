/************************************************************
************************************************************/
#include "Dummy.h"

/************************************************************
************************************************************/

/******************************
******************************/
GOLEM_DUMMY::GOLEM_DUMMY(const char* Osc_IP, int Osc_Port_To, int Osc_Port_From, const char* _UdpIP_To, int _Udp_Port_To, const char* _filaName_FrameData, const char* _fileName_SkeletonDefs)
: Osc(Osc_IP, Osc_Port_To, Osc_Port_From)
, fp_Skelton_FrameData(NULL)
, fp_Skelton_Definition(NULL)
, Next_FrameId_of_MotionData(0)
, b_1st(true)
, t_LastSend_Udp(0)
, State(STATE_SLEEP)
, t_EnterSleep(0)
{
	/********************
	********************/
	Dice_BlankTime();
	
	/********************
	********************/
	udpConnection.Create();
	udpConnection.Connect(_UdpIP_To, _Udp_Port_To);
	udpConnection.SetNonBlocking(true);
	
	/********************
	********************/
	fp_Skelton_FrameData	= fopen(_filaName_FrameData, "r");
	fp_Skelton_Definition	= fopen(_fileName_SkeletonDefs, "r");
	
	if(!fp_Skelton_FrameData || !fp_Skelton_Definition) { ERROR_MSG(); std::exit(1); }
	
	// save for Error printf.
	strcpy(filaName_FrameData, _filaName_FrameData);
	strcpy(fileName_SkeletonDefs, _fileName_SkeletonDefs);
}

/******************************
******************************/
GOLEM_DUMMY::~GOLEM_DUMMY()
{
	if(fp_Skelton_FrameData)	fclose(fp_Skelton_FrameData);
	if(fp_Skelton_Definition)	fclose(fp_Skelton_Definition);
}

/******************************
******************************/
void GOLEM_DUMMY::Dice_BlankTime()
{
	// d_BlankTime = ofRandom(15.0f, 120.0f);
	d_BlankTime = ofRandom(8.0f, 60.0f);
}

/******************************
******************************/
int GOLEM_DUMMY::get_NextFrameId(){
	return Next_FrameId_of_MotionData;
}

/******************************
******************************/
float GOLEM_DUMMY::get_TimeToAct(float now){
	return d_BlankTime - (now - t_EnterSleep);
}

/******************************
******************************/
void GOLEM_DUMMY::update(float now){
	/********************
	起動前に溜まっていたmessageは一旦clear.
	********************/
	if(b_1st){
		b_1st = false;
		
		while(Osc.OscReceive.hasWaitingMessages()){
			ofxOscMessage m_receive;
			Osc.OscReceive.getNextMessage(&m_receive);
		}

		return;
	}
	
	/********************
	********************/
	while(Osc.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/Message/ToGolem"){
			int MessageId = int(m_receive.getArgAsFloat(0));
			switch(MessageId){
				case MESSAGE_IN::GET_SKELTON_DEFINITION:
					printf("> got message = GET_SKELTON_DEFINITION\n");
					SendUdp_SkeltonDefinition();
					break;
			}
		}
	}
	
	/********************
	********************/
	switch(State){
		case STATE_SLEEP:
			if(d_BlankTime < now - t_EnterSleep){
				State = STATE_ACT;
			}
			break;
			
		case STATE_ACT:
			if(!SendUdp_FrameData(now)){
				State = STATE_SLEEP;
				Dice_BlankTime();
				t_EnterSleep = now;
			}
			break;
	}
}

/******************************
******************************/
void GOLEM_DUMMY::SendUdp_SkeltonDefinition()
{
	/********************
	********************/
	string message="";
	
	message += "/Golem/SkeletonDefinition";
	message += "<p>";
	
	/********************
	********************/
	char buf[BUF_SIZE_L];
	
	fseek(fp_Skelton_Definition, 0, SEEK_SET);
	while(fgets(buf, BUF_SIZE_L, fp_Skelton_Definition) != NULL){
		string str_Line = buf;
		Align_StringOfData(str_Line);
		
		vector<string> str_vals = ofSplitString(str_Line, ",");
		message += str_vals[0];	message += "|";		// BoneId
		message += str_vals[1];	message += "|";		// ParentId
		message += str_vals[2];	message += "|";		// BoneName
		message += str_vals[3];	message += "|";		// pos x
		message += str_vals[4];	message += "|";		// pos y
		message += str_vals[5];	message += "|";		// pos z
		message += str_vals[6];	message += "|";		// Quaternion x
		message += str_vals[7];	message += "|";		// Quaternion y
		message += str_vals[8];	message += "|";		// Quaternion z
		message += str_vals[9];	message += "<p>";	// Quaternion w
	}
	
	udpConnection.Send(message.c_str(),message.length());
	
	/********************
	********************/
	printf("> send skelton definition : Udp\n");
}

/******************************
******************************/
void GOLEM_DUMMY::Align_StringOfData(string& s)
{
	size_t pos;
	while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/******************************
******************************/
bool GOLEM_DUMMY::SendUdp_FrameData(float now)
{
	/********************
	********************/
	string message="";
	
	message += "/Golem/SkeletonData";
	message += "<p>";
	
	/********************
	********************/
	char* ret;
	char buf[BUF_SIZE_L];
	
	ret = fgets(buf, BUF_SIZE_L, fp_Skelton_FrameData);
	Next_FrameId_of_MotionData++;
	if(!checkIf_ContentsExist(ret, buf)){
		fseek(fp_Skelton_FrameData, 0, SEEK_SET);
		// printf("> fp FrameData to Top\n");
		
		Next_FrameId_of_MotionData = 0;
		
		return false;
	}
	
	/********************
	********************/
	string str_Line = buf;
	Align_StringOfData(str_Line);
	
	vector<string> str_vals = ofSplitString(str_Line, ",");
	for(int i=0; i < str_vals.size(); i++){
		message += str_vals[i];	message += "|";
	}
	// message += ofToString(Next_FrameId_of_MotionData - 1);
	
	if(0.03 < now - t_LastSend_Udp){
		udpConnection.Send(message.c_str(),message.length());
		t_LastSend_Udp = now;
	}
	
	return true;
}

/******************************
******************************/
bool GOLEM_DUMMY::checkIf_ContentsExist(char* ret, char* buf)
{
	if( (ret == NULL) || (buf == NULL)) return false;
	
	string str_Line = buf;
	Align_StringOfData(str_Line);
	vector<string> str_vals = ofSplitString(str_Line, ",");
	if( (str_vals.size() == 0) || (str_vals[0] == "") ){ // no_data or exist text but it's",,,,,,,".
		return false;
	}else{
		return true;
	}
}


