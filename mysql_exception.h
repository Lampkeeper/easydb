#pragma once
#include<exception>
#include<string>
class mysql_exception : std::exception
{
public:
	typedef struct _data
	{
		std::string err_str;
		int error_code;
	} _error_data;
	_error_data data;
	mysql_exception(const char* _error_str,int error_code) {
		data = { _error_str, error_code };
	}
	const char* what() {
		return data.err_str.c_str();
	}
	int get_code() {
		return data.error_code;
	}
	
};