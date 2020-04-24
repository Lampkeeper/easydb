#pragma once
#include "mysql.h"
#include "..\pool.h"

namespace lpkp
{
	class db_pro
	{
	public:
		/*��ʼ��ʱ��ȡ���ݿ�����*/
		db_pro()
		{
			//connect = pool::get_connection(is_last_connect);
		}

		/*����*/
		MYSQL* connect = NULL;
		/*�Ƿ�Ϊ���һ������*/
		bool is_last_connect = false;
		/*�Ƿ�����������*/
		bool is_in_tx = false;
		/*�ͷ�ʱ�Զ���������*/
		~db_pro()
		{
			if (!is_last_connect)
			{
				pool::release_connect(connect);
			}
		}
	};
}