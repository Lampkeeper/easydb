# easydb
一个c++数据库连接池,将c++下的mysql查询变得易用的项目

```c++
#include <iostream>

#include "pool.h"
#include "easy_db.h"

#define PASSWORD ""
using vec_map = std::vector<std::map<std::string, std::string>>;
void fuc(int i)
{
	//线程自动分配数据库连接，可直接使用静态方法查询
	//每个线程查询1000次
	for (size_t i = 0; i < 1000; i++)
	{
		vec_map resList = lpkp::easy_db::queryList("select * from City where CityName = ? and CityNo = ?", "成都", 8);
	}
	std::cout << std::this_thread::get_id() << "  查询1000次结束:" << i << std::endl;
}
//测试
int main()
{
	//创建100个数据库连接
	bool b = lpkp::pool::start("127.0.0.1", 3306, "root", PASSWORD, "test", 10);
	std::cout << (b ? "创建连接成功" : "创建连接失败") << std::endl;

	std::vector<lpkp::Record> resList = lpkp::easy_db::queryListR("select * from City where CityName = ?", "成都");

	std::cout << "查询条数:" << resList.size() << std::endl;
	std::cout << "开始:" << clock() << std::endl;

	std::vector<std::string> sqlList;
	std::string sql = lpkp::easy_db::SQL("insert into City(CityName,CityNo,rd) values(?,?,now())", "上海", 42);
	sqlList.push_back(sql);
	sqlList.push_back(sql);
	sqlList.push_back(sql);
	sqlList.push_back(sql);


	std::vector<lpkp::Record> recordList = lpkp::easy_db::queryListR("select * from City where CityName = ?", "上海");

	for (auto& r : recordList)
	{
		std::cout << r.getInt("CityNo") << "       " << r.getBoolean("boo") << std::endl;
	}
	//lpkp::easy_db::excute_batch(sqlList);
	//事务处理 直接 使用tx 方法，里面的sql代码会自动执行事务,
	//一行执行失败，所有操作都会回退
	try
	{
		lpkp::easy_db::tx([=]()->int {
			lpkp::easy_db::excute_batch(sqlList);
			lpkp::easy_db::excute("insert into City(CityName,CityNo,rd) values(?,?,now())", "北京", 43);
			lpkp::easy_db::excute("insert into City(CityName,CityNo,rd) values(?,?,now())", "北京", 43);
			lpkp::easy_db::excute("insert into City(CityName,CityNo,rd) values(?,?,now())", "北京", 43);
			lpkp::easy_db::excute("insert into City(CityName,CityNo,rd) values(?,?,now())", "北京", 43);
			return true;
			});
	}
	catch (lpkp::mysql_exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	//并发测试
	/*for (size_t i = 0; i < 98; i++)
	{
		std::thread td(fuc, i);
		td.detach();
	}
	for (size_t i = 0; i < 50; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << clock() << "毫秒" << std::endl;
	}*/
	

	std::this_thread::sleep_for(std::chrono::seconds(50));

}


```


