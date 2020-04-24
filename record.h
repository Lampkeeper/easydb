#pragma once
#include <vector>
#include <string>
#include <map>
namespace gzh
{
	class Record 
	{
	public:
		std::map<std::string, std::string> columns;
		std::map<std::string, int> props;

		std::string			getStr(std::string columnName)
		{
			return columns.find(columnName)->second;
		}
		bool				getBoolean(std::string columnName)
		{
			return columns.find(columnName)->second == "0";
		}
		int					getInt(std::string columnName)
		{
			return std::stoi(columns.find(columnName)->second);
		}
		unsigned int		getUInt(std::string columnName)
		{
			return std::stoi(columns.find(columnName)->second);
		}
		long				getLong(std::string columnName)
		{
			return std::stol(columns.find(columnName)->second);
		}
		unsigned long		getULong(std::string columnName)
		{
			return std::stoul(columns.find(columnName)->second);
		}
		long long			getLLong(std::string columnName)
		{
			return std::stoll(columns.find(columnName)->second);
		}
		unsigned long long	getULLong(std::string columnName)
		{
			return std::stoull(columns.find(columnName)->second);
		}
		double				getDouble(std::string columnName)
		{
			return std::stod(columns.find(columnName)->second);
		}
		long double			getLDouble(std::string columnName)
		{
			return std::stold(columns.find(columnName)->second);
		}
		float				getFloat(std::string columnName)
		{
			return std::stof(columns.find(columnName)->second);
		}

		Record& setStr(std::string columnName,std::string value)
		{
			columns[columnName] = value;
			return *this;
		}
		Record& setBoolean(std::string columnName, bool value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setInt(std::string columnName, int value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setUInt(std::string columnName, unsigned int value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setLong(std::string columnName, long value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setULong(std::string columnName, unsigned long value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setLLong(std::string columnName, long long value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setULLong(std::string columnName, unsigned long long value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setDouble(std::string columnName, double value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setLDouble(std::string columnName, long double value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
		Record& setFloat(std::string columnName, float value)
		{
			columns[columnName] = std::to_string(value);
			return *this;
		}
	};
}