#pragma once
#include "lpkp/db.h"
#include "lpkp/mysql_dialect.h"

namespace lpkp
{
	class easy_db
	{
	public:
		/*事务方法*/
		static bool tx(AtomHandle handle)
		{
			return db::tx_base(handle);
		}
		/*执行单行sql命令*/
		static int excute(const char* sql)
		{
			return db::excute_base(sql,strlen(sql));
		}
		/*执行单行sql命令*/
		static int excute(std::string& sql)
		{
			return db::excute_base(sql.c_str(), sql.length());
		}
		/*批量执行*/
		static r_vec_int excute_batch(const std::vector<std::string>& sqlList)
		{
			return db::batch_excute_base(sqlList);
		}

		/*插入数据*/
		static int save(Record& record,std::string tableName)
		{
			std::string sql = mysql_dialect::for_save(record, tableName);
			return db::excute_base(sql.c_str(),sql.length());
		}
		/*批量插入*/
		static int batch_save( std::vector<Record>& recordList,std::string tableName)
		{
			std::string sql = mysql_dialect::for_batch_save(recordList,tableName);
			return db::excute_base(sql.c_str(), sql.length());
		}
		/*更新*/
		static int update(Record& record, std::string tableName, std::string primaryKey)
		{
			std::string sql = mysql_dialect::for_update(record, tableName,primaryKey);
			return db::excute_base(sql.c_str(), sql.length());
		}
		/*批量更新*/
		static r_vec_int batch_update(std::vector<Record>& recordList, std::string tableName, std::string primaryKey)
		{
			std::vector<std::string> sqlList = mysql_dialect::for_batch_update(recordList, tableName, primaryKey);
			return db::batch_excute_base(sqlList);
		}
		/*删除数据*/
		static int delete__(Record& record, std::string tableName, std::string primaryKey)
		{
			std::string sql = mysql_dialect::for_delete(record, tableName,primaryKey);
			return db::excute_base(sql.c_str(), sql.length());
		}
		/*批量删除*/
		static r_vec_int batch_delete(std::vector<Record>& recordList, std::string tableName, std::string primaryKey)
		{
			std::vector<std::string> sqlList = mysql_dialect::for_batch_delete(recordList, tableName, primaryKey);
			return db::batch_excute_base(sqlList);
		}

		/*返回 string*/
		static std::string queryStr(const char* sql)
		{
			return db::query_base(sql, strlen(sql), 1)[0].cbegin()->second;
		}
		/*返回 boolean*/
		static bool queryBoolean(const char* sql)
		{
			return std::stoi(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second) == 0 ? false : true;
		}
		/*返回 int*/
		static int					queryInt(const char* sql)
		{
			return std::stoi( db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 unsigned int*/
		static unsigned int			queryUInt(const char* sql)
		{
			return std::stoi(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 long*/
		static long					queryLong(const char* sql)
		{
			return std::stol(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 unsigned long*/
		static unsigned long		queryULong(const char* sql)
		{
			return std::stoul(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 long long*/
		static long long			queryLLong(const char* sql)
		{
			return std::stoll(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 unsigned long long*/
		static unsigned long long	queryULLong(const char* sql)
		{
			return std::stoull(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 double*/
		static double				queryDouble(const char* sql)
		{
			return std::stod(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 long double*/
		static long double			queryLDouble(const char* sql)
		{
			return std::stold(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 float*/
		static float				queryFloat(const char* sql)
		{
			return std::stof(db::query_base(sql, strlen(sql), 1)[0].cbegin()->second);
		}
		/*返回 map*/
		static r_map				queryMap(const char* sql)
		{
			return db::query_base(sql, strlen(sql), 2)[0];
		}
		/*返回 list*/
		static r_vec_map			queryList(const char* sql)
		{
			return db::query_base(sql, strlen(sql), 3);
		}
		static Record				queryMapR(const char* sql)
		{
			return db::query_base_record(sql, strlen(sql), 2)[0];
		}
		static std::vector<Record>	queryListR(const char* sql)
		{
			return db::query_base_record(sql, strlen(sql), 3);
		}
		/*分页查询*/
		static Page					queryPage(int pageNumber, int pageSize, std::string select, std::string sqlExcept)
		{
			Page page;
			page.pageNumber = pageNumber;
			page.pageSize = pageSize;
			std::string sql = "select count(*) " + sqlExcept;
			int count = queryInt(sql);
			page.totalPage = count / pageSize + ((count % pageSize) ? 1 : 0);//总页数
			page.totalRow = count;//总记录数
			sql = mysql_dialect::for_page(pageNumber,pageSize,select,sqlExcept);
			page.resList = queryListR(sql);
			return page;
		}
		
		/*返回 string*/
		static std::string queryStr(std::string& sql)
		{
			return db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second;
		}
		/*返回 boolean*/
		static bool queryBoolean(std::string& sql)
		{
			return std::stoi(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second) == 0 ? false : true;
		}
		/*返回 int*/
		static int					queryInt(std::string& sql)
		{
			return std::stoi(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 unsigned int*/
		static unsigned int			queryUInt(std::string& sql)
		{
			return std::stoi(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 long*/
		static long					queryLong(std::string& sql)
		{
			return std::stol(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 unsigned long*/
		static unsigned long		queryULong(std::string& sql)
		{
			return std::stoul(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 long long*/
		static long long			queryLLong(std::string& sql)
		{
			return std::stoll(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 unsigned long long*/
		static unsigned long long	queryULLong(std::string& sql)
		{
			return std::stoull(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 double*/
		static double				queryDouble(std::string& sql)
		{
			return std::stod(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 long double*/
		static long double			queryLDouble(std::string& sql)
		{
			return std::stold(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 float*/
		static float				queryFloat(std::string& sql)
		{
			return std::stof(db::query_base(sql.c_str(), sql.length(), 1)[0].cbegin()->second);
		}
		/*返回 map*/
		static r_map				queryMap(std::string& sql)
		{
			return db::query_base(sql.c_str(), sql.length(), 2)[0];
		}
		/*返回 list*/
		static r_vec_map			queryList(std::string& sql)
		{
			return db::query_base(sql.c_str(), sql.length(), 3);
		}
		static Record queryMapR(std::string& sql)
		{
			return db::query_base_record(sql.c_str(), sql.length(), 2)[0];
		}
		static std::vector<Record>	queryListR(std::string& sql)
		{
			return db::query_base_record(sql.c_str(), sql.length(), 3);
		}

		/*Sql格式化模版*/
		template<typename T>
		static std::string& SQL(std::string& sql, T param)
		{
			if (sql[0] != '0')
			{
				size_t count = 0, index = 0, tp_index = 0;
				while ((index = sql.find('?', index)) != sql.npos)
					tp_index = index, index++, count++;
				if (1 != count) return sql;
				else { tp_index += 3, sql.insert(0, 1, tp_index % 256 - 128).insert(0, 1, tp_index / 256 - 128).insert(0, 1, '0'); }
			}
			if (!handleValue(sql, param))
			{
				return sql.erase(0, 3);
			}
			return sql;
		}
		template<typename T, typename ... Params>
		static std::string SQL(const char* sql, T t, Params ...params)
		{
			std::string sqlTmp(sql);
			SQL(sqlTmp, t, params...);
			return sqlTmp;
		}
		template<typename T, typename ... Params>
		static std::string& SQL(std::string& sql, T t, Params ... params)
		{
			if (sql[0] != '0')
			{
				size_t count = 0, index = 0, tp_index = 0;
				while ((index = sql.find('?', index)) != sql.npos)
					tp_index = index, index++, count++;
				if (sizeof...(params) + 1 != count) return sql;
				else { tp_index += 3, sql.insert(0, 1, tp_index % 256 - 128).insert(0, 1, tp_index / 256 - 128).insert(0, 1, '0'); }
			}
			SQL(sql, params ...);
			if (!handleValue(sql, t))
			{
				return sql.erase(0, 3);
			}
			return sql;
		}


		/*返回 string*/
		template<typename T, typename ... Params>
		static std::string			queryStr(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryStr(sqlTemp);
		}
		/*返回 boolean*/
		template<typename T, typename ... Params>
		static bool					queryBoolean(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryBoolean(sqlTemp);
		}
		/*返回 int*/
		template<typename T, typename ... Params>
		static int					queryInt(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryInt(sqlTemp);
		}
		/*返回 unsigned int*/
		template<typename T, typename ... Params>
		static unsigned int			queryUInt(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryUInt(sqlTemp);
		}
		/*返回 long*/
		template<typename T, typename ... Params>
		static long					queryUInt(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryUInt(sqlTemp);
		}
		/*返回 unsigned long*/
		template<typename T, typename ... Params>
		static unsigned long		queryULong(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryULong(sqlTemp);
		}
		/*返回 long long*/
		template<typename T, typename ... Params>
		static long long			queryULong(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryULong(sqlTemp);
		}
		/*返回 unsigned long long*/
		template<typename T, typename ... Params>
		static unsigned long long	queryULLong(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryULLong(sqlTemp);
		}
		/*返回 double*/
		template<typename T, typename ... Params>
		static double				queryDouble(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryDouble(sqlTemp);
		}
		/*返回 long double*/
		template<typename T, typename ... Params>
		static long double			queryLDouble(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryLDouble(sqlTemp);
		}
		/*返回 float*/
		template<typename T, typename ... Params>
		static float				queryLDouble(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryLDouble(sqlTemp);
		}
		/*返回 map*/
		template<typename T, typename ... Params>
		static r_map				queryMap(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryMap(sqlTemp);
		}
		/*返回 list*/
		template<typename T, typename ... Params>
		static r_vec_map			queryList(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryList(sqlTemp);
		}
		/*返回record*/
		template<typename T, typename ... Params>
		static Record				queryMapR(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryMapR(sqlTemp);
		}
		/*返回 vector<Record>*/
		template<typename T, typename ... Params>
		static std::vector<Record>	queryListR(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return queryListR(sqlTemp);
		}
		/*分页查询*/
		template<typename T, typename ... Params>
		static Page					queryPage(int pageNumber, int pageSize, std::string select, std::string sqlExcept, T t, Params ...params)
		{
			Page page;
			page.pageNumber = pageNumber;
			page.pageSize = pageSize;

			std::string sql_except_ = sqlExcept; 
			SQL(sql_except_,t,params...);

			std::string sql = "select count(*) " + sql_except_;
			int count = queryInt(sql);
			page.totalPage = count / pageSize + ((count % pageSize) ? 1 : 0);//总页数
			page.totalRow = count;//总记录数
			sql = mysql_dialect::for_page(pageNumber, pageSize, select, sql_except_);
			page.resList = queryListR(sql);
			return page;
		}
		/*执行单行sql*/
		template<typename T, typename ... Params>
		static int excute(const char* sql, T t, Params ...params)
		{
			std::string sqlTemp(sql);
			SQL(sqlTemp, t, params ...);
			return excute(sqlTemp);
		}

		private:
			static bool handleTraget(std::string& sql, const char* value, bool isStr)
			{
				size_t last_pos = -1;
				last_pos = (sql[1] + 128) * 256 + sql[2] + 128;
				std::string tmp = value;
				sql.replace(last_pos, 1, isStr ? "'" + db::sql_filter_base(tmp) + "'" : db::sql_filter_base(tmp));
				if ((last_pos = sql.find_last_of('?', last_pos--)) == sql.npos) return false;
				sql[1] = last_pos / 256 - 128, sql[2] = last_pos % 256 - 128;
				return true;
			}
			static bool handleValue(std::string& sql, int& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, unsigned int& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, long& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, unsigned long& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, long long& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, unsigned long long& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, double& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, float& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, long double& value)
			{
				return handleTraget(sql, std::to_string(value).c_str(), false);
			}
			static bool handleValue(std::string& sql, std::string& value)
			{
				return handleTraget(sql, value.c_str(), true);
			}
			static bool handleValue(std::string& sql, const char* value)
			{
				return handleTraget(sql, value, true);
			}
	};
}
