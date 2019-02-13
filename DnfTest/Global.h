#pragma once
#include <boost\serialization\singleton.hpp>
class CGlobal :public boost::serialization::singleton<CGlobal>
{
public:
	CGlobal(void);
	~CGlobal(void);
	string getMac();
	CString firstRole;
	CString firstRoleProfession;
	CString secondRole;
	CString secondRoleProfession;
	CString servername;
	CString areaname;
private:
	string mac;
};
#define global_instance CGlobal::get_mutable_instance()

