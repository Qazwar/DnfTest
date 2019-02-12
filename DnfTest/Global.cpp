#include "StdAfx.h"
#include "Global.h"


CGlobal::CGlobal(void)
{
}


CGlobal::~CGlobal(void)
{
}

string CGlobal::getMac()
{
	if(mac.size()==0){
		mac = common::getMAC();
	}
	return mac;
}
