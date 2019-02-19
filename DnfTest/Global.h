#pragma once
#include <boost\serialization\singleton.hpp>
class CGlobal :public boost::serialization::singleton<CGlobal>
{
public:
	CGlobal(void);
	~CGlobal(void);
	string getMac();
	ControllerType getControllerType();
	COLORREF getBackgroundColor();
	COLORREF getButtonColor();
private:
	string mac;
	ControllerType control;
};
#define global_instance CGlobal::get_mutable_instance()

