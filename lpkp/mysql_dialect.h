#pragma once
#include <string>
#include <map>
#include <vector>
#include "record.h"
#include "db.h"

namespace lpkp
{
	typedef struct Page
	{
		std::vector<Record> resList;//返回结果集
		int pageNumber;//页号
		int pageSize;  //页面大小
		int totalPage; //总页数
		int totalRow;  //总行数
	}        Page;
	class mysql_dialect 
	{
	public:
		static std::string for_save(Record& record, std::string tableName)
		{
			std::string sql;
			sql = "insert into `" + tableName + "`(";
			std::string temp = ") values(";
			std::map<std::string, std::string>::iterator itor;
			for (itor = record.columns.begin(); itor != record.columns.end(); itor++)
			{
				if (itor != record.columns.begin())
				{
					sql.append(", ");
					temp.append(", ");
				}
				sql.append("`" + itor->first + "`");
				/*std::string param = itor->second;
				db::sql_filter_base(itor->second);*/
				temp.append("'" + db::sql_filter_base(itor->second) + "'");
			}
			temp.append(")");
			sql.append(temp);
			return sql;
		}
		static std::string for_batch_save(std::vector<Record>& recordList, std::string tableName)
		{
			std::string sql;
			sql = "insert into `" + tableName + "`(";
			std::string temp = ") values(";
			std::map<std::string, std::string>::iterator itor;
			for (itor = recordList[0].columns.begin(); itor != recordList[0].columns.end(); itor++)
			{
				if (itor != recordList[0].columns.begin())
				{
					sql.append(", ");
					temp.append(", ");
				}
				sql.append("`" + itor->first + "`");
				temp.append("'" + db::sql_filter_base(itor->second) + "'");
			}
			for (int i = 1; i < recordList.size(); i++)
			{
				temp.append((i != recordList.size()) ? "),(" : ")");
				for (itor = recordList[i].columns.begin(); itor != recordList[i].columns.end(); itor++)
				{
					if (itor != recordList[i].columns.begin())
					{
						temp.append(",");
					}
					temp.append("'" + db::sql_filter_base(itor->second) + "'");
				}
			}
			sql.append(temp.append(")"));
			return sql;
		}

		static std::string for_update(Record& record, std::string tableName, std::string primaryKey)
		{
			std::string sql;
			sql = "update `" + tableName + "` set";
			std::string temp = "";
			if (primaryKey.length() > 0)
			{
				temp = " where `" + primaryKey + "`='" + record.getStr(primaryKey) + "';";
			}

			std::map<std::string, std::string>::iterator itor;
			for (itor = record.columns.begin(); itor != record.columns.end(); itor++)
			{

				if (itor != record.columns.begin())
				{
					sql.append(", ");
				}
				sql.append(" `" + itor->first + "`=");
				sql.append("'" + db::sql_filter_base(itor->second) + "'");
			}
			sql.append(temp);
		}
		static std::vector<std::string> for_batch_update(std::vector<Record>& recordList, std::string tableName, std::string primaryKey)
		{
			std::vector<std::string> sqlList;
			for (auto& record : recordList)
			{
				sqlList.push_back(for_update(record, tableName, primaryKey));
			}
			return sqlList;
		}

		static std::string for_delete(Record& record, std::string tableName, std::string primaryKey)
		{
			std::string sql;
			sql = "delete from `" + tableName + "`";
			std::string temp = "";
			if (primaryKey.length() > 0)
			{
				temp = " where `" + primaryKey + "`='" + record.getStr(primaryKey) + "';";
			}
			sql.append(temp);
			return sql;
		}
		static std::vector<std::string> for_batch_delete(std::vector<Record>& recordList, std::string tableName, std::string primaryKey)
		{
			std::vector<std::string> sqlList;
			for (auto& record : recordList)
			{
				sqlList.push_back(for_delete(record, tableName, primaryKey));
			}
			return sqlList;
		}
		
		static std::string for_page(int pageNumber,int pageSize,std::string select,std::string sqlExcept)
		{
			std::string sql = select + " " + sqlExcept + " limit " +
				std::to_string((pageNumber - 1) * pageSize) + ", " +
				std::to_string(pageSize);
			return sql;
		}
	};
}