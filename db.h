#pragma once
#include "db_pro.h"
#include "mysql.h"
#include "gzh_error.h"
#include <codecvt>
#include "record.h"
#ifdef  WIN32
#include <comutil.h>  
#pragma comment(lib, "comsuppw.lib")
#endif //  WIN32
#include "gzh_uuid.h"

namespace gzh
{
	using r_map = std::map<std::string, std::string>;
	using r_vec_map = std::vector<r_map>;
	using r_vec_int = std::vector<int>;


	/*线程指定连接*/
	thread_local db_pro ms_db_pro;
	/*事务(执行失败时调用,int第几行sql,int受影响行数 , 当前执行的sql)
	返回1将忽略当前错误,继续执行,返回0则停止执行且提交,返回-1则停止执行并roolback */
	using AtomHandle = std::function<int()>;

	//单实例递归锁
	std::recursive_mutex single_lock;
#define _HANDLE_ERROR__ error_no = mysql_errno(ms_db_pro.connect);\
(error_no >= 2000 && error_no <= 2066) ?throw client_exception(_error_::error_client(error_no),error_no):throw mysql_exception(mysql_error(ms_db_pro.connect),error_no);

	class db
	{
	public:
		/*str转utf-8*/
		static std::string s2utf8(const std::string& src)
		{
#ifdef WIN32
			_bstr_t t = src.c_str();
			wchar_t* pwchar = (wchar_t*)t;
			std::wstring tmp = pwchar;
#else
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
			std::wstring tmp = converterX.from_bytes(src);
#endif
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			return conv.to_bytes(tmp);
		}

		/*回滚操作*/
		static bool rollback()
		{
			return ms_db_pro.is_last_connect ? single_rollback() : mux_rollback();
		}

		/*过滤字符串*/
		static std::string sql_filter_base(std::string& param)
		{
			(!ms_db_pro.connect) ? ms_db_pro.connect = pool::get_connection(ms_db_pro.is_last_connect) : NULL;
			return ms_db_pro.is_last_connect ? single_sql_filter_base(param) : mux_sql_filter_base(param);
		}

		/*基础查询方法queryType:1 单值2 单行3 list*/
		static r_vec_map query_base(const char* sql, int sql_length, int queryType)
		{
			(!ms_db_pro.connect) ? ms_db_pro.connect = pool::get_connection(ms_db_pro.is_last_connect) : NULL;
			return ms_db_pro.is_last_connect ? single_query_base(sql, sql_length, queryType) : mux_query_base(sql,sql_length,queryType);
		}

		static std::vector<Record> query_base_record(const char* sql, int sql_length, int queryType)
		{
			(!ms_db_pro.connect) ? ms_db_pro.connect = pool::get_connection(ms_db_pro.is_last_connect) : NULL;
			return ms_db_pro.is_last_connect ? single_query_base_record(sql,sql_length,queryType) : mux_query_base_record(sql,sql_length,queryType);
		}

		/*执行sql基础方法 返回受影响行数*/
		static int excute_base(const char* sql, int sql_length)
		{
			(!ms_db_pro.connect) ? ms_db_pro.connect = pool::get_connection(ms_db_pro.is_last_connect) : NULL;
			return ms_db_pro.is_last_connect ? single_excute_base(sql, sql_length) : mux_excute_base(sql, sql_length);
		}

		/*事务基础方法*/
		static bool tx_base(AtomHandle handle)
		{
			(!ms_db_pro.connect) ? ms_db_pro.connect = pool::get_connection(ms_db_pro.is_last_connect) : NULL;
			return ms_db_pro.is_last_connect ? single_tx_base(handle) : mux_tx_base(handle);
		}

		/*批量执行基础方法 返回每一行sql的影响行数*/
		static r_vec_int batch_excute_base(const std::vector<std::string>& sqlList)
		{
			(!ms_db_pro.connect) ? ms_db_pro.connect = pool::get_connection(ms_db_pro.is_last_connect) : NULL;
			return ms_db_pro.is_last_connect ? single_batch_excute_base(sqlList) : mux_batch_excute_base(sqlList);
		}

		//多实例方法
		static bool mux_rollback()
		{
			return mysql_rollback(ms_db_pro.connect);
		}
		
		static std::string mux_sql_filter_base(std::string& param)
		{
			char* s = new char[param.length() * 2 + 1];
			mysql_real_escape_string(ms_db_pro.connect, s, param.c_str(), param.length());
			//mysql_escape_string(s, param.c_str(), param.length());
			std::string res(s);
			delete[] s;
			return s2utf8(res);
		}
		
		static r_vec_map mux_query_base(const char* sql, int sql_length, int queryType)
		{
			MYSQL_RES* res = NULL;
			MYSQL_ROW row;
			int error_no = 0;

			r_vec_map recordList;
			if (mysql_real_query(ms_db_pro.connect, sql, sql_length))
			{
				error_no = mysql_errno(ms_db_pro.connect);
				//连接超时处理，自动重连
				if (error_no == 2006)
				{
					if (!pool::reconnect(ms_db_pro.connect))
					{
						_HANDLE_ERROR__;
					}
					if (mysql_real_query(ms_db_pro.connect, sql, sql_length))
					{
						_HANDLE_ERROR__;
					}
				}
				else
				{
					_HANDLE_ERROR__;
				}
			}

			res = mysql_store_result(ms_db_pro.connect);
			if (res->row_count == 0)
			{
				mysql_free_result(res);
				throw null_result_exception("查询无任何结果", 100);
			}
			else
			{
				while ((row = mysql_fetch_row(res)))
				{
					r_map record;
					for (size_t i = 0; i < res->field_count; i++)
					{
						record.insert({ res->fields[i].name, row[i] ? row[i] : "" });
						if (queryType <= 1)break;
					}
					recordList.push_back(record);
					if (queryType <= 2) break;
				}
				mysql_free_result(res);
				return recordList;
			}
			return recordList;
		}

		static std::vector<Record> mux_query_base_record(const char* sql, int sql_length, int queryType)
		{
			MYSQL_RES* res = NULL;
			MYSQL_ROW row;
			int error_no = 0;

			std::vector<Record> recordList;
			if (mysql_real_query(ms_db_pro.connect, sql, sql_length))
			{
				error_no = mysql_errno(ms_db_pro.connect);
				//连接超时处理，自动重连
				if (error_no == 2006)
				{
					if (!pool::reconnect(ms_db_pro.connect))
					{
						_HANDLE_ERROR__;
					}
					if (mysql_real_query(ms_db_pro.connect, sql, sql_length))
					{
						_HANDLE_ERROR__;
					}
				}
				else
				{
					_HANDLE_ERROR__;
				}
			}

			res = mysql_store_result(ms_db_pro.connect);
			if (res->row_count == 0)
			{
				mysql_free_result(res);
				throw null_result_exception("查询无任何结果", 100);
			}
			else
			{
				Record field_record;
				for (size_t i = 0; i < res->field_count; i++)
				{
					field_record.props.insert({ res->fields[i].name,res->fields[i].type });
					//如果只查询单个值
					if (queryType <= 1) break;
				}
				while ((row = mysql_fetch_row(res)))
				{
					Record record;
					for (size_t i = 0; i < res->field_count; i++)
					{
						record.columns.insert({ res->fields[i].name, row[i] ? row[i] : "" });
						if (queryType <= 1)break;
					}
					record.props = field_record.props;
					recordList.push_back(record);
					if (queryType <= 2) break;
				}
				mysql_free_result(res);
				return recordList;
			}
			mysql_free_result(res);
			return recordList;
		}
		
		static int mux_excute_base(const char* sql, int sql_length)
		{
			if (mysql_real_query(ms_db_pro.connect, sql, sql_length))
			{
				int error_no = mysql_errno(ms_db_pro.connect);
				//连接超时处理，自动重连
				if (error_no == 2006)
				{
					//重连没有成功
					if (!pool::reconnect(ms_db_pro.connect))
					{
						_HANDLE_ERROR__;
					}
					if (mysql_real_query(ms_db_pro.connect, sql, sql_length))
					{
						_HANDLE_ERROR__;
					}
					return mysql_affected_rows(ms_db_pro.connect);
				}
				_HANDLE_ERROR__;
			}
			return mysql_affected_rows(ms_db_pro.connect);
		}

		static bool mux_tx_base(AtomHandle handle)
		{
			try
			{
				//判断当前线程是否已经进入事务
				if (ms_db_pro.is_in_tx == false)
				{
					//设置进入事务
					ms_db_pro.is_in_tx = true;
					//第一次进入事务,设置事务开始
					if (mysql_real_query(ms_db_pro.connect, "BEGIN", 6))
					{
						int error_no = mysql_errno(ms_db_pro.connect);
						if (error_no == 2006)
						{
							if (!pool::reconnect(ms_db_pro.connect)){_HANDLE_ERROR__;}
							if (mysql_real_query(ms_db_pro.connect, "BEGIN", 6))
							{_HANDLE_ERROR__;}
						}
						else
						{_HANDLE_ERROR__;}
					}
					int error_no = 0,count = 0;
					if ((count = handle()) <= 0)
					{
						ms_db_pro.is_in_tx = false;
						if(! rollback()) 
						{ _HANDLE_ERROR__; }
						if (count != 0)
						{_HANDLE_ERROR__;}
						return false;
					}
					else
					{
						ms_db_pro.is_in_tx = false;
						count = mysql_commit(ms_db_pro.connect);
						if (count != 0)
						{_HANDLE_ERROR__;}
						return true;
					}
				}
				//二次事务进入
				else
				{
					std::string uuid_str = _uuid_::generate();
						std::string save_point = "SAVEPOINT " + uuid_str;
						if (mysql_real_query(ms_db_pro.connect, save_point.c_str(), save_point.length()))
						{
							int error_no = 0;
							_HANDLE_ERROR__;
						}
						else
						{
							int error_no = 0;
							//执行handle代码
							if (handle() <= 0)
							{
								//执行事务失败,回退到事务点
								std::string roll_point = "ROLLBACK TO " + uuid_str;
								if (mysql_real_query(ms_db_pro.connect, save_point.c_str(), save_point.length()))
								{
									_HANDLE_ERROR__;
								}
								return false;
							}
							return true;
						}
				}
			}
			catch (mysql_exception& e)
			{
				ms_db_pro.is_in_tx = false;
				int error_no = 0;
				bool count = rollback();
				if (!count)
				{
					_HANDLE_ERROR__;
				}
				return false;
			}
			catch (...)
			{
				ms_db_pro.is_in_tx = false;
				throw;
				_error_::handle_error();
			}

			/*if (mysql_autocommit(ms_db_pro.connect,false))
			{
				int error_no = mysql_errno(ms_db_pro.connect);
				if (error_no == 2006)
				{
					if (!pool::reconnect(ms_db_pro.connect))
					{
						_HANDLE_ERROR__;
					}
					if (mysql_autocommit(ms_db_pro.connect, false))
					{
						_HANDLE_ERROR__;
					}
				}
				else
				{
					_HANDLE_ERROR__;
				}
			}
			try
			{
				int error_no;
				int count = 0;
				if (handle() < 0)
				{
					rollback();
					count = mysql_commit(ms_db_pro.connect);
					if (count != 0)
					{
						_HANDLE_ERROR__;
					}
					return true;
				}
				else
				{
					count = mysql_commit(ms_db_pro.connect);
					if (count != 0)
					{
						_HANDLE_ERROR__;
					}
					mysql_autocommit(ms_db_pro.connect, true);
					return true;
				}
			}
			catch (mysql_exception& e)
			{
				mysql_autocommit(ms_db_pro.connect, true);
				return false;
			}
			catch (...) {
				mysql_autocommit(ms_db_pro.connect, true);
				throw;
				_error_::handle_error();
			}*/
		}
		
		static r_vec_int mux_batch_excute_base(const std::vector<std::string>& sqlList)
		{
			std::vector<int> affected_rows_list;
			//用事务执行多行代码，可用来批量更新等
			tx_base([&]()->int {
				for (auto& sql : sqlList)
				{
					int affected_rows = excute_base(sql.c_str(), sql.length());
					affected_rows_list.push_back(affected_rows);
					if (affected_rows <= 0)
					{
						return affected_rows;
					}
				}
				return 1;
				});
			return affected_rows_list;
		}

		//单实例方法
		static bool single_rollback()
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_rollback();
		}

		static std::string single_sql_filter_base(std::string& param)
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_sql_filter_base(param);
		}

		static r_vec_map single_query_base(const char* sql, int sql_length, int queryType)
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_query_base(sql,sql_length,queryType);
		}

		static std::vector<Record> single_query_base_record(const char* sql, int sql_length, int queryType)
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_query_base_record(sql,sql_length,queryType);
		}

		static int single_excute_base(const char* sql, int sql_length)
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_excute_base(sql,sql_length);
		}

		static bool single_tx_base(AtomHandle handle)
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_tx_base(handle);
		}

		static r_vec_int single_batch_excute_base(const  std::vector<std::string>& sqlList)
		{
			std::unique_lock<std::recursive_mutex> lck(single_lock);
			return mux_batch_excute_base(sqlList);
		}

	};
}