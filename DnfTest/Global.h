#pragma once
#include <boost\serialization\singleton.hpp>
class CGlobal :public boost::serialization::singleton<CGlobal>
{
public:
	CGlobal(void);
	~CGlobal(void);
	string getMac();
private:
	string mac;
};
#define global_instance CGlobal::get_mutable_instance()

