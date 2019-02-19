#include "StdAfx.h"
#include "Global.h"


CGlobal::CGlobal(void)
{
	control = DD;
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

ControllerType CGlobal::getControllerType()
{
	return control;
}
