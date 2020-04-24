#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include "mysql_exception.h"
namespace gzh
{
	
	class _error_
	{
		
	public:
		enum gzh_error_code
		{
			/*连接数据库错误*/
			GZH_DB_CONNECT_ERROR,
			GZH_DB_CONNECT_EMPTY
		};
		
		static void handle_error(const char* error_str,int error_code)
		{
			throw std::exception(error_str,error_code);
		}
		static void handle_error() {
			try {
				throw;
			}
			catch (mysql_exception& e) {
				handle_error(e.what(), e.get_code());
			}
			catch (...) {
				throw;
			}
		}
		static const char* error_client(int error_code)
		{
			static const char* _client_errors_msg[] =
			{
			"CR_UNKNOWN_ERROR",
			"CR_SOCKET_CREATE_ERROR",
			"CR_CONNECTION_ERROR",
			"CR_CONN_HOST_ERROR",
			"CR_IPSOCK_ERROR",
			"CR_UNKNOWN_HOST",
			"CR_SERVER_GONE_ERROR",
			"CR_VERSION_ERROR",
			"CR_OUT_OF_MEMORY",
			"CR_WRONG_HOST_INFO",
			"CR_LOCALHOST_CONNECTION",
			"CR_TCP_CONNECTION",
			"CR_SERVER_HANDSHAKE_ERR",
			"CR_SERVER_LOST",
			"CR_COMMANDS_OUT_OF_SYNC",
			"CR_NAMEDPIPE_CONNECTION",
			"CR_NAMEDPIPEWAIT_ERROR",
			"CR_NAMEDPIPEOPEN_ERROR",
			"CR_NAMEDPIPESETSTATE_ERROR",
			"CR_CANT_READ_CHARSET",
			"CR_NET_PACKET_TOO_LARGE",
			"CR_EMBEDDED_CONNECTION",
			"CR_PROBE_SLAVE_STATUS",
			"CR_PROBE_SLAVE_HOSTS",
			"CR_PROBE_SLAVE_CONNECT",
			"CR_PROBE_MASTER_CONNECT",
			"CR_SSL_CONNECTION_ERROR",
			"CR_MALFORMED_PACKET",
			"CR_WRONG_LICENSE",

			/* new 4.1 error codes */
			"CR_NULL_POINTER",
			"CR_NO_PREPARE_STMT",
			"CR_PARAMS_NOT_BOUND",
			"CR_DATA_TRUNCATED",
			"CR_NO_PARAMETERS_EXISTS",
			"CR_INVALID_PARAMETER_NO",
			"CR_INVALID_BUFFER_USE",
			"CR_UNSUPPORTED_PARAM_TYPE",

			"CR_SHARED_MEMORY_CONNECTION",
			"CR_SHARED_MEMORY_CONNECT_REQUEST_ERROR",
			"CR_SHARED_MEMORY_CONNECT_ANSWER_ERROR",
			"CR_SHARED_MEMORY_CONNECT_FILE_MAP_ERROR",
			"CR_SHARED_MEMORY_CONNECT_MAP_ERROR",
			"CR_SHARED_MEMORY_FILE_MAP_ERROR",
			"CR_SHARED_MEMORY_MAP_ERROR",
			"CR_SHARED_MEMORY_EVENT_ERROR",
			"CR_SHARED_MEMORY_CONNECT_ABANDONED_ERROR",
			"CR_SHARED_MEMORY_CONNECT_SET_ERROR",
			"CR_CONN_UNKNOW_PROTOCOL",
			"CR_INVALID_CONN_HANDLE",
			"CR_UNUSED_1",
			"CR_FETCH_CANCELED",
			"CR_NO_DATA",
			"CR_NO_STMT_METADATA",
			"CR_NO_RESULT_SET",
			"CR_NOT_IMPLEMENTED",
			"CR_SERVER_LOST_EXTENDED",
			"CR_STMT_CLOSED",
			"CR_NEW_STMT_METADATA",
			"CR_ALREADY_CONNECTED",
			"CR_AUTH_PLUGIN_CANNOT_LOAD",
			"CR_DUPLICATE_CONNECTION_ATTR",
			"CR_AUTH_PLUGIN_ERR",
			"CR_INSECURE_API_ERR",
			"CR_FILE_NAME_TOO_LONG",
			"CR_SSL_FIPS_MODE_ERR",
			"CR_COMPRESSION_NOT_SUPPORTED",
			"CR_COMPRESSION_WRONGLY_CONFIGURED"
			};
			error_code = error_code - 2000;
			if (error_code >= 0 && error_code <= 66)
			{
				return _client_errors_msg[error_code];
			}
			return _client_errors_msg[0];
		}
	};
}