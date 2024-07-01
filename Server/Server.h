#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Library.h"
#include "EnumDefine.h"

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system.hpp"
#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"

using namespace boost;
using std::cout;
using std::endl;
using std::string;

struct Session
{
	shared_ptr<asio::ip::tcp::socket> socket;
	asio::ip::tcp::endpoint endpoint;
	string id;
	int num = -1;

	string sendBuffer;
	string receiveBuffer;
	char buf[80];
};

class Server
{
public:
	Server(string, unsigned short); // ipAddress, portNum
	~Server();

	void Start();
	void WorkerThread();
	void OpenGate();
	void StartAccept();
	void OnAccept(const system::error_code& err, Session* session);
	void Receive(Session* session);
	void PacketManager(Session* session);
	EIDState TranslatePacket(string message);
	void OnSend(const system::error_code& err);
	void SetID(Session* session);
	void CloseSession(Session* session);


private:
	asio::io_context context;					// object 
	shared_ptr<asio::io_context::work> work;	// N개의 쓰레드에서 1개의 io_context를 사용하는 실행 모델
	asio::ip::tcp::endpoint endpoint;
	asio::ip::tcp::acceptor gate;
	std::vector<Session*> sessions;
	boost::thread_group threadGroup;
	boost::mutex lock;							// lock이 하나 걸리고 다음 lock이 오면 첫번째 꺼 unlock될 때까지 대기
	std::vector<int> waitingRooms;				// 접속한 클라이언트 정보를 쌓아놓을 곳
	const int THREAD_SIZE = 4;					// 멀티스레드 갯수
};