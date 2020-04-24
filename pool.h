#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>
#include <algorithm>


#include "gzh_error.h"
#include "mysql.h"
#include "gzh_error.h"
#include "mysql_exception.h"
#include "client_exception.h"
#include "null_result_exception.h"

namespace gzh
{
	class pool
	{
	public:
		/*�������ݿ�*/
		static MYSQL* connect_db(const char* db_host, int db_port, const char* user, const char* password, const char* db_name);
		/*�������ӳ�*/
		static bool start(const char* db_host, int db_port, const char* user, const char* password, const char* db_name, int thread_count);
		/*�ر����ӳ�*/
		static bool stop();

		/*�����ӷŻ����ӳ�*/
		static void release_connect(MYSQL* connect);
		/*����*/
		static bool reconnect(MYSQL* connect);
		/*�ر�����*/
		static void destory(MYSQL* connect);
		/*��ȡ����*/
		static MYSQL* get_connection(bool& is_last_connect);
		/*��������Ƿ񻹴���*/
		static bool is_closed(MYSQL* connect);
		/*���ӳ��Ƿ��Ѿ�Ϊ��*/
		static bool pool_is_empty();
	};


}