#include "Server.h"

int main()
{
	// 서버 인스턴스 생성 : io_context
	// port : 3333
	Server server(asio::ip::address_v4::any().to_string(), 3333);

	try
	{
		server.Start();
	}
	catch (const system::error_code& err)
	{
		cout << err.message() << endl;
	}

	return 0;
}