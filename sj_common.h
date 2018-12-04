/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "stdio.h"
#include <time.h>

#include "ofMain.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);
#define WARNING_MSG(); printf("Warning in %s:%d\n", __FILE__, __LINE__);


enum{
	WINDOW_WIDTH	= 200,
	WINDOW_HEIGHT	= 200,
};

enum{
	BUF_SIZE_S = 512,
	BUF_SIZE_L = 6000,
	
	BUF_SIZE_UDP = 100000,
};

/************************************************************
************************************************************/

/**************************************************
Derivation
	class MyClass : private Noncopyable {
	private:
	public:
	};
**************************************************/
class Noncopyable{
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	void operator =(const Noncopyable& src);
	Noncopyable(const Noncopyable& src);
};

