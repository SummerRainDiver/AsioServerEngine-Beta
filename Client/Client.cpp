#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/thread/mutex.hpp>


using namespace boost;
using std::cout;
using std::endl;


class Client
{
	asio::ip::tcp::endpoint endpoint;
	asio::io_context context;
	asio::ip::tcp::socket socket;
	boost::shared_ptr<asio::io_context::work> work;
	boost::thread_group thread_group;
	boost::mutex lock;

	std::string sendBuffer;
	std::string receiveBuffer;
	char buf[80];

public:
	Client(std::string ip_address, unsigned short port_num) :
		endpoint(asio::ip::address::from_string(ip_address), port_num),
		socket(context, endpoint.protocol()),
		work(new asio::io_context::work(context))
	{}

	void Start()
	{
		for (int i = 0; i < 3; i++)
			thread_group.create_thread(bind(&Client::WorkerThread, this));

		// thread 생성 시간 확보 
		this_thread::sleep_for(chrono::milliseconds(100));

		context.post(bind(&Client::TryConnect, this));

		thread_group.join_all();
	}

private:
	void WorkerThread()
	{
		lock.lock();
		cout << "[" << boost::this_thread::get_id() << "]" << " Thread Start" << endl;
		lock.unlock();

		context.run();

		lock.lock();
		cout << "[" << boost::this_thread::get_id() << "]" << " Thread End" << endl;
		lock.unlock();
	}

	void TryConnect()
	{
		cout << "[" << boost::this_thread::get_id() << "]" << " TryConnect" << endl;

		// 비동기 접속 시도
		socket.async_connect(endpoint, boost::bind(&Client::OnConnect, this, _1));
	}

	void OnConnect(const system::error_code& err)
	{
		cout << "[" << boost::this_thread::get_id() << "]" << " OnConnect" << endl;
		if (err)
		{
			cout << "connect failed: " << err.message() << endl;
			StopAll();
			return;
		}

		context.post(bind(&Client::Send, this));
		context.post(bind(&Client::Recieve, this));
	}

	void Send()
	{
		getline(std::cin, sendBuffer);

		socket.async_write_some(asio::buffer(sendBuffer), bind(&Client::SendHandle, this, _1));
	}

	void Recieve()
	{
		socket.async_read_some(asio::buffer(buf, 80), bind(&Client::ReceiveHandle, this, _1, _2));
	}

	void SendHandle(const system::error_code& err)
	{
		if (err)
		{
			cout << "async_read_some error: " << err.message() << endl;
			StopAll();
			return;
		}

		Send();
	}

	void ReceiveHandle(const system::error_code& err, size_t size)
	{
		if (err)
		{
			cout << "async_write_some error: " << err.message() << endl;
			StopAll();
			return;
		}

		if (size == 0)
		{
			cout << "Server wants to close this session" << endl;
			StopAll();
			return;
		}

		buf[size] = '\0';
		receiveBuffer = buf;

		lock.lock();
		cout << receiveBuffer << endl;
		lock.unlock();

		Recieve();
	}

	void StopAll()
	{
		socket.close();
		work.reset();
	}
};




int main()
{
	// 로컬 서버로 연결
	// port : 3333
	Client client("127.0.0.1", 3333);

	client.Start();

	return 0;
}