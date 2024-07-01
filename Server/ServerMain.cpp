#include "Server.h"

int main()
{
	// ���� �ν��Ͻ� ���� : io_context
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