#pragma once
#include "mysql.h"
#include "..\pool.h"

namespace lpkp
{
	class db_pro
	{
	public:
		/*初始化时获取数据库连接*/
		db_pro()
		{
			//connect = pool::get_connection(is_last_connect);
		}

		/*连接*/
		MYSQL* connect = NULL;
		/*是否为最后一个连接*/
		bool is_last_connect = false;
		/*是否正在事务中*/
		bool is_in_tx = false;
		/*释放时自动回收连接*/
		~db_pro()
		{
			if (!is_last_connect)
			{
				pool::release_connect(connect);
			}
		}
	};
}