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
		/*连接数据库*/
		static MYSQL* connect_db(const char* db_host, int db_port, const char* user, const char* password, const char* db_name);
		/*启动连接池*/
		static bool start(const char* db_host, int db_port, const char* user, const char* password, const char* db_name, int thread_count);
		/*关闭连接池*/
		static bool stop();

		/*将连接放回连接池*/
		static void release_connect(MYSQL* connect);
		/*重连*/
		static bool reconnect(MYSQL* connect);
		/*关闭连接*/
		static void destory(MYSQL* connect);
		/*获取连接*/
		static MYSQL* get_connection(bool& is_last_connect);
		/*检测连接是否还存在*/
		static bool is_closed(MYSQL* connect);
		/*连接池是否已经为空*/
		static bool pool_is_empty();
	};


}