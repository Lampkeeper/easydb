#include "..\pool.h"
#include <condition_variable>
#include "db_pro.h"

#pragma comment(lib,"libmysql.lib")

namespace lpkp
{
	/*互斥锁*/
	std::recursive_mutex mux_lock;


	/*数据库连接池*/
	static std::list<MYSQL*> connect_pool;
	/*数据库连接池备份，用于释放连接*/
	static std::list<MYSQL*> connect_pool_tmp;

	MYSQL* pool::connect_db(const char* db_host, int db_port, const char* user, const char* password, const char* db_name)
	{
		MYSQL* connect = mysql_init(NULL);
		try
		{
			//设置自动重连
			bool a = true;
			mysql_options(connect, MYSQL_OPT_RECONNECT, &a);
			if (!mysql_real_connect(connect, db_host, user, password, db_name, db_port, NULL, NULL))
			{
				throw mysql_exception(mysql_error(connect), mysql_errno(connect));
			}
			mysql_autocommit(connect,true);
			return connect;
		}
		catch (...)
		{
			_error_::handle_error();
		}
	}

	bool pool::start(const char* db_host, int db_port, const char* user, const char* password, const char* db_name, int thread_count)
	{
		std::unique_lock<std::recursive_mutex> lck(mux_lock);
		for (size_t i = 0; i < thread_count; i++)
		{
			MYSQL* ms_conn = connect_db(db_host, db_port, user, password, db_name);
			//mysql连接池分配失败
			if (ms_conn == NULL)
			{
				stop();
				return false;
			}
			connect_pool.push_back(ms_conn);
			connect_pool_tmp.push_back(ms_conn);
		}
		return true;
	}

	bool pool::stop()
	{
		//销毁所有连接
		std::unique_lock<std::recursive_mutex> lck(mux_lock);
			for (std::list<MYSQL*>::iterator itor = connect_pool_tmp.begin();
						itor != connect_pool_tmp.end();
						itor = connect_pool_tmp.erase(connect_pool_tmp.begin())){destory((*itor));}
			//清空列表
			std::list<MYSQL*> tmp1;
			connect_pool.swap(tmp1);
		return true;
	}

	void pool::destory(MYSQL* connect)
	{
		if (connect != NULL)
		{
			mysql_close(connect);
			connect = NULL;
		}
	}

	bool pool::reconnect(MYSQL* connect)
	{
		return !mysql_ping(connect);
	}

	void pool::release_connect(MYSQL* connect)
	{
		std::unique_lock<std::recursive_mutex> lck(mux_lock);
		connect_pool.push_front(connect);
	}
	
	MYSQL* pool::get_connection(bool& is_last_connect)
	{
		std::unique_lock<std::recursive_mutex> lck(mux_lock);
		if (connect_pool.size() > 1)
		{
			MYSQL* mysql = connect_pool.front();
			connect_pool.pop_front();
			is_last_connect = false;

			return mysql;
		}
		else if (connect_pool.size() == 1)
		{
			MYSQL* mysql = connect_pool.back();
			is_last_connect = true;
			return mysql;
		}
		else 
		{

			return NULL;
		}
	}

	bool pool::is_closed(MYSQL* connect)
	{
		try
		{
			if (mysql_real_query(connect, "select 1", 8))
			{
				int errorno = mysql_errno(connect);
				if (errorno == CR_SERVER_GONE_ERROR)
				{
					if (errno = mysql_ping(connect))
					{
						_error_::handle_error(mysql_error(connect), mysql_errno(connect));
						return false;
					}
					else
					{
						return true;
					}
				}
			}
			return true;
		}
		catch (...)
		{
			_error_::handle_error();
		}
		
	}

	bool pool::pool_is_empty()
	{
		std::unique_lock<std::recursive_mutex> lck(mux_lock);
		return connect_pool.size() == 1;
	}
}